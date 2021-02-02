#include "TROOT.h"
#include "TEnv.h"
#include "TSystem.h"

void init_kobamac(){
  TString str = __FILE__; /* preprocessor command __FILE__ returns the file name of the file itself */ 
  /* std::cout << __FILE__ << std::endl; */
  str.Resize(str.Length()-14); /* Removing the string "init_kobamac.C" at the last */
  TString cmd = Form("echo $(cd %s && pwd)", str.Data());
  str = gSystem->GetFromPipe(cmd.Data());
  str.Resize(str.Length()-10); /* Removing the string "/root/base" at the last */
  gEnv->SetValue("KOBAMAC_DIR",str.Data());
  std::cout <<"ini_kobamac.C: KOBAMAC_DIR=" << gEnv->GetValue("KOBAMAC_DIR","") << std::endl;
  
  cmd = Form("find %s -type d -not -path '*/\\.*' | tr -d '\r' | tr '\n' ':' | sed -e 's/:$//'",gEnv->GetValue("KOBAMAC_DIR","."));
  str = gSystem->GetFromPipe(cmd.Data());
  gROOT->SetMacroPath(Form("%s:%s", gROOT->GetMacroPath(), str.Data()));

  TString rootlib;
  if (gROOT->GetVersionInt() >= 60000) {
    rootlib = gROOT->GetLibDir(); 
  }else{
    rootlib = gEnv->GetValue("Unix.*.Root.DynamicPath","");
    TObjArray* objarr = rootlib.Tokenize(":");
    for (Int_t i = 0; i < objarr->GetEntries(); i++) {
      TObjString* objstr = (TObjString*) objarr->At(i);
      rootlib = objstr->GetString();
      if (rootlib.Contains("lib")){
	break;
      }
    }
  }
  cmd = Form("objdump -p %s/root/base/TBrowserEx_C.so | grep -e RUNPATH -e RPATH | awk '{print $2}'",   gEnv->GetValue("KOBAMAC_DIR","."));
  TString solib = gSystem->GetFromPipe(cmd.Data());
  
  TString sharedlib = gSystem->GetMakeSharedLib();
  TObjArray* sharedlibobjarr = sharedlib.Tokenize(":");
  Bool_t sharedlibbool = false;
  for (Int_t i = 0; i < sharedlibobjarr->GetEntries(); i++) {
    TObjString* objstr = (TObjString*) sharedlibobjarr->At(i);
    TString libstr = objstr->GetString();
    if (libstr.Contains("-Wl,-rpath,")){
      sharedlibbool = true;
      break;
    }
  }
  if (!sharedlibbool) {
    TString tmpstr = gSystem->GetMakeSharedLib();
    tmpstr.ReplaceAll("$ObjectFiles",Form("$ObjectFiles -Wl,-rpath,%s",rootlib.Data()));
    gSystem->SetMakeSharedLib(tmpstr.Data());
  }
  
  if (solib.EqualTo(rootlib)){
    gROOT->ProcessLine(Form(".L %s/root/base/TBrowserEx.C+",   gEnv->GetValue("KOBAMAC_DIR",".")));    
  }else{
    gROOT->ProcessLine(Form(".L %s/root/base/TBrowserEx.C++",   gEnv->GetValue("KOBAMAC_DIR",".")));
  }
  gROOT->ProcessLine(Form(".L %s/root/base/tbr.C",           gEnv->GetValue("KOBAMAC_DIR",".")));
  gROOT->ProcessLine(Form(".L %s/root/base/WaitOneClickX.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  gROOT->ProcessLine(Form(".L %s/root/base/WaitOneClickY.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  gROOT->ProcessLine(Form(".L %s/root/base/WaitOneClickT.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  gROOT->ProcessLine(Form(".L %s/root/base/WaitGraphicalCut.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  gROOT->ProcessLine("TBrowserEx *gBrowserEx = 0;");
  return;
}
