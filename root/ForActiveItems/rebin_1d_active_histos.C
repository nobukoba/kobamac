void rebin_1d_active_histos(){
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (!gBrowserEx) {return;}
  TString str = gBrowserEx->OpenTGInputDialog("nbins: %d",1);
  if (str.EqualTo("")){return;}
  std::istringstream iss(str.Data());
  Int_t par0;
  iss >> par0;
  TIter next(gBrowserEx->GetListOfOrderedActiveHistos());
  TH1 *hist;
  while((hist = (TH1 *)next())){
    if (hist->InheritsFrom("TH2")) {
      std::cout << "This script can not handle a TH2 histogram." << std::endl;
      continue;
    }
    gROOT->ProcessLine(Form(".L %s/root/cui/rebin_1d.C", gEnv->GetValue("KOBAMAC_DIR",".")));
    gROOT->ProcessLine(Form("rebin_1d((TH1*)%p,%d)", hist,par0));
  }
  return;
}
