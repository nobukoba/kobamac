void zone_1_3(){
  gROOT->ProcessLine(Form(".x %s/root/zone/zone_x_y.C(1,3)", gEnv->GetValue("KOBAMAC_DIR",".")));
  return;
}
