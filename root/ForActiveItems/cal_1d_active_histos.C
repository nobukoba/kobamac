void cal_1d_active_histos(){
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (!gBrowserEx) {return;}
  TString str = gBrowserEx->OpenTGInputDialog("Enter calibration parameters of\ny = p0 + p1*x\nFormat: p0 p1 (%f %f)",2);
  if (str.EqualTo("")){return;}
  std::istringstream iss(str.Data());
  Double_t par0, par1;
  iss >> par0 >> par1;
  TIter next(gBrowserEx->GetListOfOrderedActiveHistos());
  TH1 *hist;
  gROOT->ProcessLine(Form(".L %s/root/cui/cal_1d.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  while((hist = (TH1 *)next())){
    if (hist->InheritsFrom("TH2")) {
      std::cout << "This script can not handle a TH2 histogram." << std::endl;
      continue;
    }
    gROOT->ProcessLine(Form("cal_1d((TH1*)%p,%f,%f)", hist,par0,par1));
  }
  return;
}
