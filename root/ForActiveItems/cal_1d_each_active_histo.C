void cal_1d_each_active_histo(){
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (!gBrowserEx) {return;}
  TIter next(gBrowserEx->GetListOfOrderedActiveHistos());
  TH1 * hist;
  gROOT->ProcessLine(Form(".L %s/root/cui/cal_1d.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  while((hist = (TH1*)next())){
    if (hist->InheritsFrom("TH2")) {
      std::cout << "This script can not handle a TH2 histogram." << std::endl;
      continue;
    }
    TString disstr = "Enter calibration parameters of\ny = p0 + p1*x for\n";
    disstr += hist->GetName();
    disstr += "; ";
    disstr += hist->GetTitle();
    disstr += "\nFormat: p0 p1 (%f %f)";
    TString retstr = gBrowserEx->OpenTGInputDialog(disstr.Data(),2);
    if (retstr.EqualTo("")){return;}
    std::istringstream iss(retstr.Data());
    Double_t par0, par1;
    iss >> par0 >> par1;
    gROOT->ProcessLine(Form("cal_1d((TH1*)%p,%f,%f)", hist,par0,par1));
  }
  return;
}
