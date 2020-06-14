void rebin_1d_sel_pad(){
  gROOT->ProcessLine(Form(".L %s/root/ForSelectedPad/get_th1_in_sel_pad.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  TH1* hist = (TH1*)gROOT->ProcessLine("get_th1_in_sel_pad();");
  if(hist == 0){return;}
  
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (gBrowserEx == 0) {return;}
  TString str = gBrowserEx->OpenTGInputDialog("nbins: %d",1);
  if (str.EqualTo("")){return;}
  
  std::istringstream iss(str.Data());
  Int_t par0;
  iss >> par0;
  
  gROOT->ProcessLine(Form(".L %s/root/cui/rebin_1d.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  gROOT->ProcessLine(Form("rebin_1d((TH1*)%p,%d)", hist,par0));
}
