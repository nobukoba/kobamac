#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include "TROOT.h"
#include "TSystem.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TDirectory.h"
#include "TList.h"
#include "TH1.h"
#include "TList.h"
#include "TGFileDialog.h"

void convert_file(TString pre_file_name, TString cur_file_name){
  TString cur_file_name_save = cur_file_name;
  pre_file_name = gSystem->BaseName(pre_file_name.Data());
  cur_file_name = gSystem->BaseName(cur_file_name.Data());
  cur_file_name.ReplaceAll(".","_");
  pre_file_name.ReplaceAll(".","_");
  TH1* cur_hist_tmp = gROOT->TDirectory::FindObject(Form("%s_ch1",cur_file_name.Data()));
  if (cur_hist_tmp != 0) {
    return;
  }
  TH1* pre_hist_ch1;
  TH1* pre_hist_ch2;
  TH1* pre_hist_ch3;
  TH1* pre_hist_ch4;
  if(pre_file_name != ""){
    pre_hist_ch1 = gROOT->TDirectory::FindObject(Form("%s_ch1",pre_file_name.Data()));
    pre_hist_ch2 = gROOT->TDirectory::FindObject(Form("%s_ch2",pre_file_name.Data()));
    pre_hist_ch3 = gROOT->TDirectory::FindObject(Form("%s_ch3",pre_file_name.Data()));
    pre_hist_ch4 = gROOT->TDirectory::FindObject(Form("%s_ch4",pre_file_name.Data()));
    if(pre_hist_ch1 == 0){
      std::cout << "Hist: " << Form("%s_ch1",pre_file_name.Data()) << "does not exist." << std::endl;
      return;
    }
  }
  TH1* cur_hist_ch1 = new TH1D(Form("%s_ch1",cur_file_name.Data()),cur_file_name.Data(),16384,0,16384);
  TH1* cur_hist_ch2 = new TH1D(Form("%s_ch2",cur_file_name.Data()),cur_file_name.Data(),16384,0,16384);
  TH1* cur_hist_ch3 = new TH1D(Form("%s_ch3",cur_file_name.Data()),cur_file_name.Data(),16384,0,16384);
  TH1* cur_hist_ch4 = new TH1D(Form("%s_ch4",cur_file_name.Data()),cur_file_name.Data(),16384,0,16384);
  std::ifstream ifs(cur_file_name_save.Data());
  std::string buf;
  TString tbuf;
  while(ifs && getline(ifs, buf)){ /* getline is very slow in CINT!*/
    tbuf = buf;
    if(tbuf.BeginsWith("[Data]")){
      getline(ifs, buf);
      break;
    }
  }
  
  /* iline++;
     if (iline <= 9){
     std::cout << "Line " << iline << ": " << buf << std::endl;
     }
     if (iline == 7){
     std::istringstream iss(buf);
     iss >> live_time[ifile] >> meas_time[ifile];
     }
     if (iline == 9){
     std::istringstream iss(buf);
     iss >> xmin >> xmax;
     Int_t nbin = xmax - xmin; */

  Int_t iline = 0;
  Double_t val[5];
  while(ifs && getline(ifs, buf)){
    iline++;
    tbuf = buf;
    tbuf.ReplaceAll(","," ");
    std::istringstream iss(tbuf.Data());
    iss >> val[0] >> val[1] >> val[2] >> val[3] >> val[4];
    double con_ch1 = 0.;
    double con_ch2 = 0.;
    double con_ch3 = 0.;
    double con_ch4 = 0.;
    if(pre_file_name != ""){
      con_ch1 = pre_hist_ch1->GetBinContent(iline);
      con_ch2 = pre_hist_ch2->GetBinContent(iline);
      con_ch3 = pre_hist_ch3->GetBinContent(iline);
      con_ch4 = pre_hist_ch4->GetBinContent(iline);
    }
    cur_hist_ch1->SetBinContent(iline,val[1]-con_ch1);
    cur_hist_ch2->SetBinContent(iline,val[2]-con_ch2);
    cur_hist_ch3->SetBinContent(iline,val[3]-con_ch3);
    cur_hist_ch4->SetBinContent(iline,val[4]-con_ch4);
  }
  return;
}

void online_apg7400a() {
  std::cout << std::endl << "Macro: online_apg7400a.C" << std::endl;
  TString result = gSystem->GetFromPipe("ls *.csv| sort");
  std::stringstream ss(result.Data());
  std::string str = "";
  TString pre_file_name = "";
  TString cur_file_name = "";
  while(getline(ss, str)){
    cur_file_name = str.c_str();
    convert_file(pre_file_name, cur_file_name);
    pre_file_name = cur_file_name;
  }
  return;
}
