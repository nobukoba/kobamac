#include <iostream>
#include "TROOT.h"
#include "TSystem.h"
#include "TBox.h"
#include "TFrame.h"
#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
#include "TGraph.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TList.h"
#include "TLine.h"
#include "TCutG.h"
#include "TString.h"

void cutg_lin_func(){
  TCanvas* canvas = gPad->GetCanvas();
  if (!canvas) {
    std::cout << "There is no canvas." << std::endl;
    return;
  }
  
  gPad->SetCrosshair();
  TGraph * graphical_cut = new TGraph();
  WaitGraphicalCut primi(0, graphical_cut);
  gPad->SetCrosshair(0);
  
  if (!graphical_cut){
    std::cout << "Some problem occured. Exit." << std::endl;
    return;
  }
  TList *listofpri = gPad->GetListOfPrimitives();
  TIter next(listofpri);
  TObject *obj;
  TH1 *hist = 0;
  
  while ((obj = next())){
    if (obj->InheritsFrom("TH1")) {
      hist = (TH1*)obj;
      std::cout << "TH1 hist was found." << std::endl;
      break;
    }
  }
  if(hist == 0){
    std::cout << "TH1 histogram was not found in this pad." << std::endl;
    graphical_cut->Delete();
    gPad->Update();
    return;
  }
  TCutG *cutg = (TCutG*)listofpri->FindObject("CUTG");
  if (cutg != 0){
    cutg->Delete();
  }
  cutg = new TCutG("CUTG",graphical_cut->GetN(),
		   graphical_cut->GetX(),
		   graphical_cut->GetY());
  Int_t nop = cutg->GetN();
  if (nop <= 1){
    std::cout << "Number of points:" << nop << std::endl;
    return;
  }
  Double_t *x = cutg->GetX();
  Double_t *y = cutg->GetY();
  for (Int_t i=0; i < nop - 1; i++) {
    Double_t a = (y[i+1] - y[i]) / (x[i+1] - x[i]);
    Double_t b = y[i] - a * x[i];
    std::cout << "Btw points " << i << " and " << i + 1 << ": ";
    std::cout << "a = " << a << ", b = " << b << std::endl;
  }
  return;
}
