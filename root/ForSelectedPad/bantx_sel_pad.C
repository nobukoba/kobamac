void bantx_sel_pad(){
  gROOT->ProcessLine(Form(".L %s/root/ForSelectedPad/get_th2_in_sel_pad.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  TH2* hist = (TH2*)gROOT->ProcessLine("get_th2_in_sel_pad();");
  if(hist == 0){return;}
  
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (gBrowserEx == 0) {return;}
  TString str = gBrowserEx->OpenTGInputDialog("y=ax+b: a b_1 b_2: %f %f %f",3);
  if (str.EqualTo("")){return;}
  
  std::istringstream iss(str.Data());
  Double_t par0, par1, par2;
  iss >> par0 >> par1 >> par2;
  
  gROOT->ProcessLine(Form(".L %s/root/cui/bantx.C", gEnv->GetValue("KOBAMAC_DIR",".");
  gROOT->ProcessLine(Form("bantx((TH1*)%p,%f,%f,%f)", hist,par0,par1,par2));
}
