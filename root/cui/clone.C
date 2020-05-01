void clone(TH1* hist){
  gROOT->ProcessLine(Form(".L %s/cui/clone_with_suffix.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  gROOT->ProcessLine(Form("clone_with_suffix((TH1*)%p,\"%s\");",hist,"_cln"));
  return;
}
