{
  gEnv->SetValue("KOBAMAC_DIR","$HOME/kobamac");
  gROOT->ProcessLine(".x $HOME/kobamac/root/base/ini_kobamac.C");
  gROOT->ProcessLine("TBrowserEx t;"); /* Auto launching of the TBrowerEx. This line can be commented out. */ 
}
