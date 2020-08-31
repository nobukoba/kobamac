void set_y_range_sel_pad(){
  gROOT->ProcessLine(Form(".L %s/root/ForSelectedPad/get_th_in_sel_pad.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  TH1* hist = (TH1*)gROOT->ProcessLine("get_th_in_sel_pad();");
  if(hist == 0){return;}
  
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (gBrowserEx == 0) {return;}
  TString str = gBrowserEx->OpenTGInputDialog("Range: %f %f",2);
  if (str.EqualTo("")){return;}
  
  std::istringstream iss(str.Data());
  Double_t par0, par1;
  iss >> par0 >> par1;
  
  gROOT->ProcessLine(Form(".L %s/root/cui/set_y_range.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  gROOT->ProcessLine(Form("set_y_range((TH1*)%p,%f,%f)", hist,par0,par1));
  gPad->Modified();
  gPad->Update();
  return;
}
