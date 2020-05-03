void zone_4_4(){
  gROOT->ProcessLine(Form(".x %s/root/zone/zone_x_y.C(4,4)", gEnv->GetValue("KOBAMAC_DIR",".")));
  return;
}
