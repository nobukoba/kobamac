#include <iostream>
#include "TROOT.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TGListTree.h"
#include "TList.h"
#include "TKey.h"
#include "TH1.h"

void clone_active_histos(){
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (!gBrowserEx) {return;}
  TIter next(gBrowserEx->GetListOfOrderedActiveHistos());
  TH1 * hist;
  gROOT->ProcessLine(Form(".L %s/cui/clone.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  while((hist = (TH1*)next())){
    gROOT->ProcessLine(Form("clone((TH1*)%p)",hist));
  }
  return;
}