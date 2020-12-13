void cal_1d_e_each_active_histo(){
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (!gBrowserEx) {return;}
  TObjArray* arr = gBrowserEx->GetListOfOrderedActiveHistos();
  for (Int_t i=0; i < arr->GetEntries(); i++){
    TH1 *hist = (TH1*)arr->At(i);
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
    gROOT->ProcessLine(Form(".L %s/root/cui/cal_1d_e.C", gEnv->GetValue("KOBAMAC_DIR",".")));
    gROOT->ProcessLine(Form("cal_1d_e((TH1*)%p,%f,%f)", hist,par0,par1));
  }
  return;
}
