void zone_2_1(){
  gROOT->ProcessLine(Form(".x %s/root/zone/zone_x_y.C(2,1)", gEnv->GetValue("KOBAMAC_DIR",".")));
  return;
}
