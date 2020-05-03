void reverse_sel_pad(){
  gROOT->ProcessLine(Form(".L %s/root/ForSelectedPad/get_th_in_sel_pad.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  TH1* hist = (TH1*)gROOT->ProcessLine("get_th_in_sel_pad();");
  if(hist == 0){return;}

  gROOT->ProcessLine(Form(".x %s/root/cui/reverse.C((TH1*)%p)",
			  gEnv->GetValue("KOBAMAC_DIR","."),
			  hist));
  return;
}
