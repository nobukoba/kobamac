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
  TIter next(gBrowserEx->GetListOfOrderedActiveHistos());
  TH1 *hist;
  gROOT->ProcessLine(Form(".L %s/cui/scale.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  while((hist = (TH1 *)next())){
    gROOT->ProcessLine(Form("scale((TH1*)%p,%f)",hist,par));
  }
  return;
}
