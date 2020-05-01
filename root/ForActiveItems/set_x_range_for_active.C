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
  gROOT->ProcessLine(Form(".L %s/cui/set_x_range.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  TIter next(gBrowserEx->GetListOfOrderedActiveHistos());
  TH1 * hist;
  while((hist = (TH1*)next())){
    gROOT->ProcessLine(Form("set_x_range((TH1*)%p,%f,%f)",hist,par0,par1));
  }
  return;
}