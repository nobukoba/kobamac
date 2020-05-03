void zone_3_1(){
  gROOT->ProcessLine(Form(".x %s/root/zone/zone_x_y.C(3,1)", gEnv->GetValue("KOBAMAC_DIR",".")));
  return;
}
