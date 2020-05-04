void init_tbr(){
  if(!gROOT->GetClass("TBrowserEx")){
    TString str = __FILE__;
    str.Resize(str.Length()-7); /* Remove the string "/tbr.C" in the tail */
    /*gROOT->ProcessLine(Form(".x %s/ini_kobamac.C", str.Data()));*/
    gROOT->Macro(Form("%s/ini_kobamac.C", str.Data()));
  }
  gROOT->ProcessLine("tbr();");
}

