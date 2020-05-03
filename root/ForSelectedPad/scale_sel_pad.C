void scale_sel_pad(){
  gROOT->ProcessLine(Form(".L %s/root/ForSelectedPad/get_th_in_sel_pad.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  TH1* hist = (TH2*)gROOT->ProcessLine("get_th_in_sel_pad();");
  if(hist == 0){return;}

  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (!gBrowserEx) {return;}
  TString str = gBrowserEx->OpenTGInputDialog("Please enter scale for all the histos\nFormat: %f",1);
  if (str.EqualTo("")){return;}
  std::istringstream iss(str.Data());
  Double_t par;
  iss >> par;

  gROOT->ProcessLine(Form(".L %s/root/cui/scale.C", gEnv->GetValue("KOBAMAC_DIR","."));
  gROOT->ProcessLine(Form("scale((TH1*)%p)", hist));
}
