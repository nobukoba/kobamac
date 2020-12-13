#include <iostream>
#include "TROOT.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TGListTree.h"
#include "TList.h"
#include "TKey.h"
#include "TH1.h"

void scale_active_histos(){
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (!gBrowserEx) {return;}
  TString str = gBrowserEx->OpenTGInputDialog("Please enter scale for all the histos\nFormat: %f",1);
  if (str.EqualTo("")){return;}
  std::istringstream iss(str.Data());
  Double_t par;
  iss >> par;
  gROOT->ProcessLine(Form(".L %s/root/cui/scale.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  TObjArray* arr = gBrowserEx->GetListOfOrderedActiveHistos();
  for (Int_t i=0; i < arr->GetEntries(); i++){
    TH1 *hist = (TH1*)arr->At(i);
    gROOT->ProcessLine(Form("scale((TH1*)%p,%f)",hist,par));
  }
  return;
}
