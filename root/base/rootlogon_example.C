{
  gEnv->SetValue("KOBAMAC_DIR","~/kobamac_dev/kobamac/root");
  gROOT->ProcessLine(".x ~/kobamac_dev/kobamac/root/base/ini_kobamac.C");
  //gROOT->ProcessLine("TBrowserEx t;"); /* Auto launching of the TBrowerEx. This line can be commented out. */ 
}
