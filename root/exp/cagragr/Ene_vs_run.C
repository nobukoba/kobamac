#if defined(__CINT__) && !defined(__MAKECINT__)
{
  TNamed *named = (TNamed*)gROOT->FindObjectAny("initial_working_dir");
  if (named) {gSystem->cd(named->GetTitle());}
  gSystem->CompileMacro("./kobaprg/root/exp/cagragr/fit_gaus_run_by_run.C","k");
  fit_gaus_run_by_run();
}
#else
#include <iostream>
#include "TROOT.h"
#include "TSystem.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TROOT.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TList.h"
#include "TKey.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TMath.h"

void fit_gaus_run_by_run(){
  std::cout << std::endl << "Macro: fit_gaus_run_by_run.C "<< std::endl;
  TNamed *named = (TNamed*)gROOT->FindObjectAny("initial_working_dir");
  if (named) {gSystem->cd(named->GetTitle());}
  std::cout << "gSystem->pwd(): " << gSystem->pwd() << std::endl;

  TList * lof = (TList *)gROOT->GetListOfFiles();
  TIter next(lof);
  TFile *file;
  while (file = (TFile*)next()){
    TString newfile = file->GetName();
    if(newfile.EndsWith(".root")){
      newfile.Resize(newfile.Length()-5);
    }
    newfile += "_fitted.root";
    std::cout << "newfile name: " << newfile << std::endl;
    TH2 * hist = (TH2*)file->Get("Nobu/IDEneNS"); 
    //std::cout << << std::endl;

    if (!hist){
      std::cout << "hist is null. skip."<< std::endl;
      continue;
    }
    
    TFile *h_newfile = TFile::Open(newfile.Data(),"recreate");
    TList *file_list = h_newfile->GetList();
    
    TString str = hist->GetName();
    str += "_sly_";
    TString str_n = str;
    Int_t num = 1;
    while (1) {
      TIter next2(file_list);
      TObject *obj;
      Int_t found = 0;
      while (obj = next2()) {
	if (!(obj->InheritsFrom("TH1"))) {
	  continue;
	} 
	TString name = obj->GetName();
	if (name.BeginsWith(str_n)){
	  found = 1;
	  break;
	}
      }
      if (found == 1) {
	str_n = Form("%s_sly%d_",hist->GetName(),num);
	num++;
      }else{
	break;
      }
    }
    Int_t nbins = hist->GetNbinsX();
    Int_t ndig = 1;
    while (nbins > (TMath::Power(10, ndig)-1)) {
      ndig++;
    }
    for (Int_t i = 1; i <= hist->GetNbinsX(); i++) {
      TString formatstr = Form("%s%%0%dd",str_n.Data(),ndig);
      TString hname = Form(formatstr.Data(),i);
      TH1D *hout = new TH1D(hname, hist->GetTitle(), hist->GetNbinsY(),
			    hist->GetYaxis()->GetXmin(), hist->GetYaxis()->GetXmax());
      for (Int_t j = 0; j <= hist->GetNbinsY()+1; j++) {
	hout->Fill(hist->GetYaxis()->GetBinCenter(j),
		   hist->GetBinContent(i,j));
	
	
      }
      hout->GetXaxis()->SetRangeUser(480,550);
      Double_t maxbin = hout->GetMaximumBin();
      Double_t maxcen = hout->GetBinCenter(maxbin);
      Double_t bwid   = hout->GetBinWidth(maxbin);
      Double_t xlow   = maxcen - 5*bwid;
      Double_t xup    = maxcen + 5*bwid;
      Double_t xrang  = xup - xlow;
      TF1* fgaus = new TF1("fit_narrow_gaus_init_0", "gaus", xlow, xup);
      Double_t ymax = hout->GetBinContent(hout->GetMaximumBin());
      if (ymax == 0.) {
	std::cout << hout->GetName() << ": ymax == 0. skip."<< std::endl;
	continue;
      }
      fgaus->SetParLimits(0, 0., ymax*10000.);
      fgaus->SetParLimits(1, xlow, xup);
      fgaus->SetParLimits(2, xrang/10., xrang);
      hout->Fit(fgaus,"R+");
      
      Double_t par[3];
      par[0] = fgaus->GetParameter(0);
      par[1] = fgaus->GetParameter(1);
      par[2] = fgaus->GetParameter(2);
      TList *funclist = hout->GetListOfFunctions();
      funclist->Last()->Delete();
      xlow = par[1] - 0.75 * par[2];
      xup  = par[1] + 0.75 * par[2];
      TF1* fit_narrow_gaus = new TF1("fit_narrow_gaus_0","gaus",xlow,xup);
      fit_narrow_gaus->SetLineWidth(1);
      hout->Fit(fit_narrow_gaus,"R+");
      hout->Draw();
      gPad->Update();
      gPad->Modified();
    }
    h_newfile->Write();
    h_newfile->Close();
  }
  return;
}

#endif
