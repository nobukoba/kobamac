void readobj_from_keys_active_histos(){
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (!gBrowserEx) {return;}
  TIter next(gBrowserEx->GetListOfOrderedActiveHistos());
  TH1 * hist;
  gROOT->ProcessLine(Form(".L %s/root/cui/readobj_from_keys.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  while((hist = (TH1*)next())){
    gROOT->ProcessLine(Form("readobj_from_keys((TH1*)%p)",hist));
  }
  return;
}
