#include "TROOT.h"

void hout4(){
  gROOT->ProcessLine(Form(".L %s/root/ws/houtn.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  gROOT->ProcessLine("houtn(4)");
  return;
}
