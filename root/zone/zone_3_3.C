void zone_3_3(){
  gROOT->ProcessLine(Form(".x %s/zone/zone_x_y.C(3,3)", gEnv->GetValue("KOBAMAC_DIR",".")));
  return;
}
