void zone_2_3(){
  gROOT->ProcessLine(Form(".x %s/zone/zone_x_y.C(2,3)", gEnv->GetValue("KOBAMAC_DIR",".")));
  return;
}
