#include <iostream>
#include "TROOT.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TGListTree.h"
#include "TList.h"
#include "TKey.h"
#include "TGInputDialog.h"
#include "TH1.h"
void multi_fit_photo_peak_for_active_clear(){
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (!gBrowserEx) {return;}
  TGListTree *hist_fListTree = gBrowserEx->GetHistListTree();
  gROOT->ProcessLine(Form(".L %s/root/fit/fit_photo_peak_clear.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  TIter next(gBrowserEx->GetHistListTreeActiveHistos());
  TObjString* objstr;
  while((objstr = (TObjString*)next())){
    TGListTreeItem *cur_ListTreeItem = (TGListTreeItem *) objstr->GetString().Atoll();
    hist_fListTree->DoubleClicked(cur_ListTreeItem,1);
    gROOT->ProcessLine("fit_photo_peak_clear();");
  }
  return;
}
