void get_histo_stat_for_active(){
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (!gBrowserEx) {return;}
  gROOT->ProcessLine(Form(".L %s/root/cui/get_histo_stat.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  TObjArray* arr = gBrowserEx->GetListOfOrderedActiveHistos();
  for (Int_t i=0; i < arr->GetEntries(); i++){
    TH1 *hist = (TH1*)arr->At(i);
    gROOT->ProcessLine(Form("get_histo_stat((TH1*)%p)",hist));
  }
  return;
}
