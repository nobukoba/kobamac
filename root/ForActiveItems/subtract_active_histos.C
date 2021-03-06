#include <iostream>
#include "TROOT.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TGListTree.h"
#include "TList.h"
#include "TKey.h"
#include "TH1.h"

void subtract_active_histos(){
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (!gBrowserEx) {return;}
  TObjArray* histos = gBrowserEx->GetListOfOrderedActiveHistos();
  if (histos->GetEntries() <= 1) {
    std::cout << "Entries is less than 2. Exit." << std::endl;
    return;
  }
  Int_t k = 0;
  TH1 *subtracted = 0;
  gROOT->ProcessLine(Form(".L %s/root/cui/sub.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  for (Int_t i=0; i < histos->GetEntries(); i++){
    TH1 *hist = (TH1*)histos->At(i);
    if (k==0) {
      subtracted = (TH1*)gROOT->ProcessLine(Form("sub((TH1*)%p,(TH1*)%p,1.0,0.0);",hist,hist));
    }else{
      subtracted->Add(hist,-1.0);
    }
    k++;
  }
  return;
}
