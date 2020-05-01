#include "TROOT.h"

void zone_3_2(){
  gROOT->ProcessLine(Form(".L %s/zone/zone_x_y.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  gROOT->ProcessLine("zone_x_y(3,2)");
  return;
}
