#include <iostream>
#include "TROOT.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TGListTree.h"
#include "TList.h"
#include "TKey.h"
#include "TH1.h"

void add_active_histos(){
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (!gBrowserEx) {return;}
  TObjArray* arr = gBrowserEx->GetListOfOrderedActiveHistos();
  Int_t k = 0;
  gROOT->ProcessLine(Form(".L %s/root/cui/add.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  TH1 *hadded = 0;
  for (Int_t i=0; i < arr->GetEntries(); i++){
    TH1 *hist = (TH1*)arr->At(i);
    if (k==0) {
      hadded = (TH1*)gROOT->ProcessLine(Form("add((TH1*)%p,(TH1*)%p,1.0,0.0);",hist,hist));
    }else{
      hadded->Add(hist);
    }
    k++;
  }
  return;
}
