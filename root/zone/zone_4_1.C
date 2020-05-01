void zone_4_1(){
  gROOT->ProcessLine(Form(".x %s/zone/zone_x_y.C(4,1)", gEnv->GetValue("KOBAMAC_DIR",".")));
  return;
}
