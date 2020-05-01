#include "TROOT.h"

void zone_2_3(){
  gROOT->ProcessLine(Form(".L %s/zone/zone_x_y.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  gROOT->ProcessLine("zone_x_y(2,3)");
  return;
}
