void rebin_minx_max_1d_active_histos(){
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (!gBrowserEx) {return;}
  TString str = gBrowserEx->OpenTGInputDialog("nbins xmin xmax: %d %f %f",3);
  if (str.EqualTo("")){return;}
  std::istringstream iss(str.Data());
  Int_t par0;
  Double_t par1, par2;
  iss >> par0 >> par1 >> par2;
  TIter next(gBrowserEx->GetListOfOrderedActiveHistos());
  TH1 *hist;
  while((hist = (TH1 *)next())){
    if (hist->InheritsFrom("TH2")) {
      std::cout << "This script can not handle a TH2 histogram." << std::endl;
      continue;
    }
    gROOT->ProcessLine(Form(".L %s/root/cui/rebin_minx_max_1d.C", gEnv->GetValue("KOBAMAC_DIR",".")));
    gROOT->ProcessLine(Form("rebin_minx_max_1d((TH1*)%p,%d)", hist,par0,par1,par2));
  }
  return;
}
