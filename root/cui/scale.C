void scale(TH1* hist, Double_t par){
  gROOT->ProcessLine(Form(".L %s/root/cui/clone_with_suffix.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  TH1 *scl = (TH1*)gROOT->ProcessLine(Form("clone_with_suffix((TH1*)%p,\"%s\");",hist,"_scl"));
  scl->Scale(par);
  return;
}
