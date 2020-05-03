void zone_3_3(){
  gROOT->ProcessLine(Form(".L %s/root/zone/zone_x_y.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  gROOT->ProcessLine("zone_x_y(3,3)");
}
