#include "TROOT.h"
#include "TEnv.h"
#include "TSystem.h"

void init_kobamac(){
  TString str = __FILE__; /* preprocessor command __FILE__ returns the file name of the file itself */ 
  /* std::cout << __FILE__ << std::endl; */
  str.Resize(str.Length()-14); /* Remove the string "init_kobamac.C" at the last */
  TString cmd = Form("echo $(cd %s && pwd)", str.Data());
  str = gSystem->GetFromPipe(cmd.Data());
  str.Resize(str.Length()-10); /* Remove the string "/root/base" at the last */
  gEnv->SetValue("KOBAMAC_DIR",str.Data());
  std::cout <<"ini_kobamac.C: KOBAMAC_DIR=" << gEnv->GetValue("KOBAMAC_DIR","") << std::endl;
  
  cmd = Form("find %s -type d -not -path '*/\\.*' | tr -d '\r' | tr '\n' ':' | sed -e 's/:$//'",gEnv->GetValue("KOBAMAC_DIR","."));
  str = gSystem->GetFromPipe(cmd.Data());
  gROOT->SetMacroPath(Form("%s:%s", gROOT->GetMacroPath(), str.Data()));
  gROOT->ProcessLine(Form(".L %s/root/base/TBrowserEx.C+",   gEnv->GetValue("KOBAMAC_DIR",".")));
  gROOT->ProcessLine(Form(".L %s/root/base/tbr.C",           gEnv->GetValue("KOBAMAC_DIR",".")));
  gROOT->ProcessLine(Form(".L %s/root/base/WaitOneClickX.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  gROOT->ProcessLine(Form(".L %s/root/base/WaitOneClickY.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  gROOT->ProcessLine(Form(".L %s/root/base/WaitOneClickT.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  gROOT->ProcessLine("TBrowserEx *gBrowserEx = 0;");
  return;
}
