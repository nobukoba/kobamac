void rebin_1d_e_active_histos(){
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (!gBrowserEx) {return;}
  TString str = gBrowserEx->OpenTGInputDialog("nbins: %d",1);
  if (str.EqualTo("")){return;}
  std::istringstream iss(str.Data());
  Int_t par0;
  iss >> par0;
  TObjArray* arr = gBrowserEx->GetListOfOrderedActiveHistos();
  for (Int_t i=0; i < arr->GetEntries(); i++){
    TH1 *hist = (TH1*)arr->At(i);
    if (hist->InheritsFrom("TH2")) {
      std::cout << "This script can not handle a TH2 histogram." << std::endl;
      continue;
    }
    gROOT->ProcessLine(Form(".L %s/root/cui/rebin_1d_e.C", gEnv->GetValue("KOBAMAC_DIR",".")));
    gROOT->ProcessLine(Form("rebin_1d_e((TH1*)%p,%d)", hist,par0));
  }
  return;
}
