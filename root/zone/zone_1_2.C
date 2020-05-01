void zone_1_2(){
  gROOT->ProcessLine(Form(".x %s/zone/zone_x_y.C(1,2)", gEnv->GetValue("KOBAMAC_DIR",".")));
  return;
}
