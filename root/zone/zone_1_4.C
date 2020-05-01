void zone_1_4(){
  gROOT->ProcessLine(Form(".x %s/zone/zone_x_y.C(1,4)", gEnv->GetValue("KOBAMAC_DIR",".")));
  return;
}
