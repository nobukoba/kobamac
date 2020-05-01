void zone_2_2(){
  gROOT->ProcessLine(Form(".x %s/zone/zone_x_y.C(2,2)", gEnv->GetValue("KOBAMAC_DIR",".")));
  return;
}
