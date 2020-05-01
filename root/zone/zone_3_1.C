#include "TROOT.h"

void zone_3_1(){
  gROOT->ProcessLine(Form(".L %s/zone/zone_x_y.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  gROOT->ProcessLine("zone_x_y(3,1)");
  return;
}
