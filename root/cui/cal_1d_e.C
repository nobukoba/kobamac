void cal_1d_e(TH1* hist, Double_t par0, Double_t par1){
  gROOT->ProcessLine(Form(".L %s/root/cui/cal_1d.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  gROOT->ProcessLine(Form("cal_1d((TH1*)%p,%f,%f,1);",hist,par0,par1));
  return;
}
