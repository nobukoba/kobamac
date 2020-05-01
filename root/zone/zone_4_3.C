void zone_4_3(){
  gROOT->ProcessLine(Form(".x %s/zone/zone_x_y.C(4,3)", gEnv->GetValue("KOBAMAC_DIR",".")));
  return;
}
