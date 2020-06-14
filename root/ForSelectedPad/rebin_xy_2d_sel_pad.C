void rebin_xy_2d_sel_pad(){
  gROOT->ProcessLine(Form(".L %s/root/ForSelectedPad/get_th2_in_sel_pad.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  TH2* hist = (TH2*)gROOT->ProcessLine("get_th2_in_sel_pad();");
  if(hist == 0){return;}
  
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (gBrowserEx == 0) {return;}
  TString str = gBrowserEx->OpenTGInputDialog("xbins ybins: %d %d",2);
  if (str.EqualTo("")){return;}
  
  std::istringstream iss(str.Data());
  Int_t par0, par1;
  iss >> par0 >> par1;
  
  gROOT->ProcessLine(Form(".L %s/root/cui/rebin_xy_2d.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  gROOT->ProcessLine(Form("rebin_xy_2d((TH2*)%p,%d,%d)", hist,par0,par1));
}
