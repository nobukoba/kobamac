void zone_4_2(){
  gROOT->ProcessLine(Form(".x %s/root/zone/zone_x_y.C(4,2)", gEnv->GetValue("KOBAMAC_DIR",".")));
  return;
}
