void init_tbr(){
  if(!gROOT->GetClass("TBrowserEx")){
    TString str = __FILE__;
    str.Resize(str.Length()-11); /* Remove the string "/init_tbr.C" in the tail */
    gROOT->ProcessLine(Form(".x %s/init_kobamac.C", str.Data()));
  }
  gROOT->ProcessLine("tbr();");
}

