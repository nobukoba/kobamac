#include "TROOT.h"

void hout6(){
  gROOT->ProcessLine(Form(".L %s/root/ws/houtn.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  gROOT->ProcessLine("houtn(6)");
  return;
}
