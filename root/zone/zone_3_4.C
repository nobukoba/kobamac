void zone_3_4(){
  gROOT->ProcessLine(Form(".x %s/zone/zone_x_y.C(3,4)", gEnv->GetValue("KOBAMAC_DIR",".")));
  return;
}
