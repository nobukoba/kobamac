#include <iostream>
#include "TList.h"
#include "TString.h"
#include "TH1.h"
class TBrowserEx;

void set_x_range_for_active(){
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (!gBrowserEx) {return;}
  TString str = gBrowserEx->OpenTGInputDialog("Range: %f %f",2);
  if (str.EqualTo("")){return;}
  std::istringstream iss(str.Data());
  Double_t par0, par1;
  iss >> par0 >> par1;
  gROOT->ProcessLine(Form(".L %s/root/cui/set_x_range.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  TObjArray* arr = gBrowserEx->GetListOfOrderedActiveHistos();
  for (Int_t i=0; i < arr->GetEntries(); i++){
    TH1 *hist = (TH1*)arr->At(i);
    gROOT->ProcessLine(Form("set_x_range((TH1*)%p,%f,%f)",hist,par0,par1));
  }
  return;
}
