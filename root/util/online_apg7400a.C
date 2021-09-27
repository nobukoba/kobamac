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
  TString pre_file_name_save = pre_file_name;
  TString cur_file_name_save = cur_file_name;
  pre_file_name = gSystem->BaseName(pre_file_name.Data());
  cur_file_name = gSystem->BaseName(cur_file_name.Data());
  cur_file_name.ReplaceAll(".","_");
  pre_file_name.ReplaceAll(".","_");
  TH1* cur_hist_tmp = (TH1*)gROOT->TDirectory::FindObject(Form("%s_ch1",cur_file_name.Data()));
  if (cur_hist_tmp != 0) {
    return;
  }
  if(pre_file_name != ""){
    TH1* pre_hist_tmp = (TH1*)gROOT->TDirectory::FindObject(Form("%s_ch1",pre_file_name.Data()));
    if(pre_hist_tmp == 0){
      std::cout << "Hist: " << Form("%s_ch1",pre_file_name.Data()) << "does not exist." << std::endl;
      return;
    }
  }
  std::cout << "processing " << cur_file_name.Data() << std::endl;
  TH1* cur_hist_ch1 = new TH1D(Form("%s_ch1",cur_file_name.Data()),cur_file_name.Data(),16384,0,16384);
  TH1* cur_hist_ch2 = new TH1D(Form("%s_ch2",cur_file_name.Data()),cur_file_name.Data(),16384,0,16384);
  TH1* cur_hist_ch3 = new TH1D(Form("%s_ch3",cur_file_name.Data()),cur_file_name.Data(),16384,0,16384);
  TH1* cur_hist_ch4 = new TH1D(Form("%s_ch4",cur_file_name.Data()),cur_file_name.Data(),16384,0,16384);
  std::ifstream pre_ifs(pre_file_name_save.Data());
  std::ifstream cur_ifs(cur_file_name_save.Data());
  std::string pre_buf;
  std::string cur_buf;
  TString pre_tbuf;
  TString cur_tbuf;
  while(cur_ifs && pre_ifs &&
	getline(cur_ifs, cur_buf) &&
	getline(pre_ifs, pre_buf)){ /* getline is very slow in CINT!*/
    pre_tbuf = pre_buf;
    cur_tbuf = cur_buf;
    if(cur_tbuf.BeginsWith("[Data]")){
      getline(cur_ifs, cur_buf);
      getline(pre_ifs, pre_buf);
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
  Double_t cur_val[5];
  Double_t pre_val[5];
  while(cur_ifs && pre_ifs &&
	getline(cur_ifs, cur_buf) &&
	getline(pre_ifs, pre_buf) ){
    iline++;
    pre_tbuf = pre_buf;
    cur_tbuf = cur_buf;
    pre_tbuf.ReplaceAll(","," ");
    cur_tbuf.ReplaceAll(","," ");
    std::istringstream pre_iss(pre_tbuf.Data());
    std::istringstream cur_iss(cur_tbuf.Data());
    pre_iss >> pre_val[0] >> pre_val[1] >> pre_val[2] >> pre_val[3] >> pre_val[4];
    cur_iss >> cur_val[0] >> cur_val[1] >> cur_val[2] >> cur_val[3] >> cur_val[4];
    cur_hist_ch1->SetBinContent(iline,cur_val[1]-pre_val[1]);
    cur_hist_ch2->SetBinContent(iline,cur_val[2]-pre_val[2]);
    cur_hist_ch3->SetBinContent(iline,cur_val[3]-pre_val[3]);
    cur_hist_ch4->SetBinContent(iline,cur_val[4]-pre_val[4]);
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
