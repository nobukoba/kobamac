void zone_1_1(){
  gROOT->ProcessLine(Form(".x %s/zone/zone_x_y.C(1,1)", gEnv->GetValue("KOBAMAC_DIR",".")));
  return;
}
