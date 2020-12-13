#include <fstream>
#include <sstream>
#include <iostream>
#include "TROOT.h"
#include "TFile.h"
#include "TH1.h"
#include "TString.h"
#include "TDirectory.h"
#include "TDirectoryFile.h"
#include "TObjArray.h"
#include "TObjString.h"

void convert(TString path, TString fname, TDirectory *dir[]){
  TString fullpath = "/np1a/v05/qst/exp2019/ws_laptop1_backup_201911061149/HPGe/";
  fullpath += path;
  std::cout << fullpath.Data() << ":" << fname.Data() << std::endl;
  std::ifstream ifs(fullpath.Data());
  std::string buf;
  TString tbuf;
  TH1 *hist[4];
  for (Int_t i=0; i<4;i++) {
    dir[i]->cd();
    hist[i] = new TH1D(fname.Data(),fname.Data(),16384,0,16384);
  }
  while(ifs && getline(ifs, buf)){ /* getline is very slow in CINT!*/
    tbuf = buf;
    if(tbuf.BeginsWith("[Data]")){
      break;
    }
  }
  getline(ifs, buf);
  Double_t val[5];
  Int_t iline = 0;
  while(ifs && getline(ifs, buf)){
    iline++;
    tbuf = buf;
    tbuf.ReplaceAll(","," ");
    std::istringstream iss(tbuf.Data());
    iss >> val[0] >> val[1] >> val[2] >> val[3] >> val[4];
    for (Int_t i=0; i<4;i++) {
      hist[i]->SetBinContent(iline,val[i+1]);
    }
  }
  return;
}

void mkhist(){
  std::cout << "mkhist" << std::endl;
  std::ifstream ifs("mca_file.txt");
  std::string buf;
  Int_t iline = 0;
  TFile *f = TFile::Open("mca_file.root","recreate");
  TDirectory *raw =  new TDirectoryFile("raw","raw","",f); 
  raw->cd();
  TDirectory *ch[4];
  for (Int_t i=0; i<4; i++) {
    ch[i] = new TDirectoryFile(Form("ch%d",i+1),Form("ch%d",i+1),"",raw);
  }
  TDirectory *histdir[4];
  while(ifs && getline(ifs, buf)){ /* getline is very slow in CINT!*/
    std::cout << iline << ": "<< buf << std::endl;
    iline++;
    TString buf_str(buf);
    TObjArray *arr = buf_str.Tokenize("/");
    for (Int_t i=0; i<4; i++) {
      TDirectory *cur = ch[i];
      cur->cd();
      for (Int_t j=0; j < arr->GetEntries()-1; j++) {
	TObjString *str = (TObjString*)arr->At(j);
	/* std::cout << cur->GetName() << ":" << str->GetString() << std::endl; */
	cur = (TDirectory*)cur->FindObject(str->GetString().Data());
	if (!cur) {
	  cur = new TDirectoryFile(str->GetString().Data(),str->GetString().Data(),"",cur);
	}
	cur->cd();	  
      }
      histdir[i] = cur;
    }
    convert(buf_str.Data(),((TObjString*)arr->Last())->GetString().Data(),histdir);
  }
  f->Write();
  f->Close();
  return;
}
