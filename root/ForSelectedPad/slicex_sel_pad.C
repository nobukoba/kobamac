void slicex_sel_pad(){
  gROOT->ProcessLine(Form(".L %s/root/ForSelectedPad/get_th2_in_sel_pad.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  TH2* hist = (TH2*)gROOT->ProcessLine("get_th2_in_sel_pad();");
  if(hist == 0){return;}

  gROOT->ProcessLine(Form(".x %s/root/cui/slicex.C((TH1*)%p)",
			  gEnv->GetValue("KOBAMAC_DIR","."),
			  hist));
  return;
}
