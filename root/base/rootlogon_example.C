{
  gEnv->SetValue("KOBAMAC_DIR","$HOME/kobamac");
  gROOT->ProcessLine(".x $HOME/kobamac/root/base/ini_kobamac.C");
  gROOT->ProcessLine("new TBrowserEx();"); /* Auto launching of the TBrowerEx. This line can be commented out. */ 
}
