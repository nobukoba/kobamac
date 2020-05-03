void reverse(TH1* hist){
  gROOT->ProcessLine(Form(".L %s/root/cui/clone_with_suffix.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  TH1 *rev = (TH1*)gROOT->ProcessLine(Form("clone_with_suffix((TH1*)%p,\"%s\");",hist,"_rev"));
  rev->Scale(-1);
  return;
}
