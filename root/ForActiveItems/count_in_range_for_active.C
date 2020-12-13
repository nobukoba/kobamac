#include <iostream>
#include "TROOT.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TList.h"
#include "TKey.h"
#include "TH1.h"

void count_in_range_for_active(){
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (!gBrowserEx) {return;}
  gROOT->ProcessLine(Form(".L %s/root/cui/count_in_range.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  TObjArray* arr = gBrowserEx->GetListOfOrderedActiveHistos();
  for (Int_t i=0; i < arr->GetEntries(); i++){
    TH1 *hist = (TH1*)arr->At(i);
    gROOT->ProcessLine(Form("count_in_range((TH1*)%p)",hist));
  }
  return;
}
