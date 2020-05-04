void tbr(){
  if(!gROOT->GetClass("TBrowserEx")){
    TString str = __FILE__;
    str.Resize(str.Length()-5); /* Remove the string "tbr.C" in the tail */
    gROOT->ProcessLine(Form(".x %s/ini_kobamc;", str.Data()));
  }
  gROOT->ProcessLine("new TBrowserEx();");
}
