void zone_3_2(){
  gROOT->ProcessLine(Form(".x %s/zone/zone_x_y.C(3,2)", gEnv->GetValue("KOBAMAC_DIR",".")));
  return;
}
