void get_histo_stat_for_active(){
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (!gBrowserEx) {return;}
  gROOT->ProcessLine(Form(".L %s/root/cui/get_histo_stat.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  TIter next(gBrowserEx->GetListOfOrderedActiveHistos());
  TH1 * hist;
  while((hist = (TH1*)next())){
    gROOT->ProcessLine(Form("get_histo_stat((TH1*)%p)",hist));
  }
  return;
}
