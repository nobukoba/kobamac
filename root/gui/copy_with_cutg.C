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

void copy_with_cutg(){
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
  TH2 *hist = 0;
  
  while ((obj = next())){
    if (obj->InheritsFrom("TH2")) {
      hist = (TH2*)obj;
      std::cout << "TH2 hist was found." << std::endl;
      break;
    }
  }
  if(hist == 0){
    std::cout << "TH2 histogram was not found in this pad." << std::endl;
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
  gROOT->ProcessLine(Form(".L %s/root/cui/clone_with_suffix.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  TH2 *hout = (TH2*)gROOT->ProcessLine(Form("clone_with_suffix((TH1*)%p,\"%s\");",hist,"_cut"));
  Double_t xx, yy;
  for (Int_t i = 0; i <= hist->GetNbinsX()+1; i++) {
    for (Int_t j = 0; j <= hist->GetNbinsY()+1; j++) {
      xx = hist->GetXaxis()->GetBinCenter(i);
      yy = hist->GetYaxis()->GetBinCenter(j);
      if (cutg->IsInside(xx,yy) != 1) {
	hout->SetBinContent(i,j,0);
      }
    }
  }
  hout->Draw("colz");
  gPad->Update();
  gPad->GetFrame()->SetBit(TBox::kCannotMove);
  return;
}
