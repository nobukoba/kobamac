void rebin_min_max_1d_e_each_active_histo(){
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (!gBrowserEx) {return;}
  TIter next(gBrowserEx->GetListOfOrderedActiveHistos());
  TH1 * hist;
  while((hist = (TH1*)next())){
    if (hist->InheritsFrom("TH2")) {
      std::cout << "This script can not handle a TH2 histogram." << std::endl;
      continue;
    }
    TString disstr = "Enter nbins xmin xmax for";
    disstr += hist->GetName();
    disstr += "; ";
    disstr += hist->GetTitle();
    disstr += "\nFormat: %d %f %f";
    TString retstr = gBrowserEx->OpenTGInputDialog(disstr.Data(),2);
    if (retstr.EqualTo("")){return;}
    std::istringstream iss(retstr.Data());
    Int_t par0;
    Double_t par1, par2;
    iss >> par0 >> par1 >> par2;
    gROOT->ProcessLine(Form(".L %s/root/cui/rebin_min_max_1d_e.C", gEnv->GetValue("KOBAMAC_DIR",".")));
    gROOT->ProcessLine(Form("rebin_min_max_1d_e((TH1*)%p,%d,%f,%f)", hist,par0,par1,par2));
  }
  return;
}
