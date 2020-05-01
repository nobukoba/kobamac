#include "TROOT.h"

void zone_1_3(){
  gROOT->ProcessLine(Form(".L %s/zone/zone_x_y.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  gROOT->ProcessLine("zone_x_y(1,3)");
  return;
}
