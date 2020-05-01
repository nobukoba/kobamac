void reverse_sel_pad(){
  gROOT->ProcessLine(Form(".L %s/ForSelectedPad/get_th_in_sel_pad.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  TH1* hist = (TH1*)gROOT->ProcessLine("get_th_in_sel_pad();");
  if(hist == 0){return;}

  gROOT->ProcessLine(Form(".L %s/cui/reverse.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  gROOT->ProcessLine(Form("reverse((TH1*)%p)",hist));

  return;
}
