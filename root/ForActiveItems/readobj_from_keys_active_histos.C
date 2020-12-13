void readobj_from_keys_active_histos(){
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (!gBrowserEx) {return;}
  gROOT->ProcessLine(Form(".L %s/root/cui/readobj_from_keys.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  TObjArray* arr = gBrowserEx->GetListOfOrderedActiveHistos();
  for (Int_t i=0; i < arr->GetEntries(); i++){
    TH1 *hist = (TH1*)arr->At(i);
    gROOT->ProcessLine(Form("readobj_from_keys((TH1*)%p)",hist));
  }
  return;
}
