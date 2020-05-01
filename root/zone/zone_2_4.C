void zone_2_4(){
  gROOT->ProcessLine(Form(".x %s/zone/zone_x_y.C(2,4)", gEnv->GetValue("KOBAMAC_DIR",".")));
  return;
}
