#include <iostream>
#include "TROOT.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TGListTree.h"
#include "TString.h"
#include "TList.h"
#include "TKey.h"
#include "TH1.h"

void scale_each_active_histo(){
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (!gBrowserEx) {return;}
  gROOT->ProcessLine(Form(".L %s/root/cui/scale.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  TObjArray* arr = gBrowserEx->GetListOfOrderedActiveHistos();
  for (Int_t i=0; i < arr->GetEntries(); i++){
    TH1 *hist = (TH1*)arr->At(i);
    TString disstr = "Please enter scale for ";
    disstr += hist->GetName();
    disstr += "; ";
    disstr += hist->GetTitle();
    disstr += "\nFormat: %f";
    TString retstr = gBrowserEx->OpenTGInputDialog(disstr.Data(),1);
    if (retstr.EqualTo("")){return;}
    std::istringstream iss(retstr.Data());
    Double_t par;
    iss >> par;
    gROOT->ProcessLine(Form("scale((TH1*)%p,%f)",hist,par));
  }
  return;
}
