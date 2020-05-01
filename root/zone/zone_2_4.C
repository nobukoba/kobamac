#include "TROOT.h"

void zone_2_4(){
  gROOT->ProcessLine(Form(".L %s/zone/zone_x_y.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  gROOT->ProcessLine("zone_x_y(2,4)");
  return;
}
