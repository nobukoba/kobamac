void slicey_sel_pad(){
  gROOT->ProcessLine(Form(".L %s/ForSelectedPad/get_th2_in_sel_pad.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  TH2* hist = (TH2*)gROOT->ProcessLine("get_th2_in_sel_pad();");
  if(hist == 0){return;}

  gROOT->ProcessLine(Form(".L %s/cui/slicey.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  gROOT->ProcessLine(Form("slicey((TH1*)%p)",hist));

  return;
}
