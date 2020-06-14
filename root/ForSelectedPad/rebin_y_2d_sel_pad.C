void rebin_y_2d_sel_pad(){
  gROOT->ProcessLine(Form(".L %s/root/ForSelectedPad/get_th2_in_sel_pad.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  TH2* hist = (TH2*)gROOT->ProcessLine("get_th2_in_sel_pad();");
  if(hist == 0){return;}
  
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (gBrowserEx == 0) {return;}
  TString str = gBrowserEx->OpenTGInputDialog("nbins: %d",1);
  if (str.EqualTo("")){return;}
  
  std::istringstream iss(str.Data());
  Int_t par0;
  iss >> par0;
  
  gROOT->ProcessLine(Form(".L %s/root/cui/rebin_y_2d.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  gROOT->ProcessLine(Form("rebin_y_2d((TH2*)%p,%d)", hist,par0));
}
