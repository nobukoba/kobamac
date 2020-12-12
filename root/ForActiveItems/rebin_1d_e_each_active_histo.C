void rebin_1d_e_each_active_histo(){
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (!gBrowserEx) {return;}
  TObjArray* arr = gBrowserEx->GetListOfOrderedActiveHistos();
  for (Int_t i=0; i < arr->GetEntries(); i++){
    TH1 *hist = (TH1*)arr->At(i);
    if (hist->InheritsFrom("TH2")) {
      std::cout << "This script can not handle a TH2 histogram." << std::endl;
      continue;
    }
    TString disstr = "Enter nbins for";
    disstr += hist->GetName();
    disstr += "; ";
    disstr += hist->GetTitle();
    disstr += "\nFormat: %d";
    TString retstr = gBrowserEx->OpenTGInputDialog(disstr.Data(),2);
    if (retstr.EqualTo("")){return;}
    std::istringstream iss(retstr.Data());
    Int_t par0;
    iss >> par0;
    gROOT->ProcessLine(Form(".L %s/root/cui/rebin_1d_e.C", gEnv->GetValue("KOBAMAC_DIR",".")));
    gROOT->ProcessLine(Form("rebin_1d_e((TH1*)%p,%d)", hist,par0));
  }
  return;
}
