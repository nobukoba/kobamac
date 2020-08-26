/* Simple macro showing capabilities of triple slider
   Authors: Bertrand Bellenot, Ilka Antcheva */
/* Modified by Nobu 2020.08.02 */
#include <iostream>
#include "TGButton.h"
#include "TGLayout.h"
#include "TH2.h"
#include "TTree.h"
#include "TFile.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TGTextEntry.h"
#include "TGSlider.h"

void dh_dy(TH1* hist, TH1* hout){
  for (Int_t i = 1; i <= hist->GetNbinsX(); i++) {
    for (Int_t j = 2; j <= hist->GetNbinsY(); j++) {
      hout->SetBinContent(i,j,hist->GetBinContent(i,j)
			  -hist->GetBinContent(i,j-1));
    }
  }
  return;
}

class MultiHSliders : public TGMainFrame {
  /*  RQ_OBJECT("MultiHSliders") */
private:
  Int_t               nsliders;
  TGLayoutHints       *fBly, *fBfly;
  TGHorizontalFrame   **fHframe;
  TGHSlider           **fHslider;
  TGTextEntry         **fTeh;
  TGTextBuffer        **fTbh;
  Double_t            *fSliderMin;
  Double_t            *fSliderMax;
  Double_t            *fSliderPich;
  Int_t               *HId;
  Int_t               *HSId;
public:
  MultiHSliders(Int_t);
  virtual ~MultiHSliders();
  void SetMinMax(Int_t, Double_t, Double_t);
  void CloseWindow();
  void DoText(const char *);
  void DoSlider();
  void DoFunction();
  Double_t  GetVal(Int_t);
  ClassDef(MultiHSliders, 1)
};

/*____________________________________________________________________________*/
MultiHSliders::MultiHSliders(Int_t nsli) :
  TGMainFrame(gClient->GetRoot(), 100, 100), nsliders(nsli)
{
  fHframe      = new TGHorizontalFrame*[nsliders];
  fHslider     = new TGHSlider*[nsliders];
  fTeh         = new TGTextEntry*[nsliders];
  fTbh         = new TGTextBuffer*[nsliders];
  fSliderMin   = new Double_t[nsliders];
  fSliderMax   = new Double_t[nsliders];
  fSliderPich  = new Double_t[nsliders];
  HId          = new Int_t[nsliders];
  HSId         = new Int_t[nsliders];

  SetCleanup(kDeepCleanup);
  /*--- layout for buttons: top align, equally expand horizontally */
  fBly = new TGLayoutHints(kLHintsTop | kLHintsExpandX, 5, 5, 5, 5);
  /*--- layout for the frame: place at bottom, right aligned */
  fBfly = new TGLayoutHints(kLHintsTop | kLHintsRight, 5, 5, 5, 5);
  
  for (Int_t islider = 0; islider < nsliders; islider++) {
    HId[islider]  = islider;
    HSId[islider] = islider + nsliders;
    fHframe[islider] = new TGHorizontalFrame(this, 0, 0, 0);
    fHslider[islider] = new TGHSlider(fHframe[islider], 500,
				      kSlider1 | kScaleDownRight, HSId[islider],
				      kHorizontalFrame,
				      GetDefaultFrameBackground());
    fHslider[islider]->Connect("PositionChanged(Int_t)", "MultiHSliders", 
			       this, "DoSlider()");
    fHslider[islider]->SetRange(0,1000);
    fHslider[islider]->SetPosition(500);
    fHframe[islider]->Resize(200, 25);
    fTbh[islider] = new TGTextBuffer(5);
    fTeh[islider] = new TGTextEntry(fHframe[islider], fTbh[islider], HId[islider]);
    SetMinMax(islider, 0., 1.);
    fTeh[islider]->SetToolTipText("Pointer Position Value");
    fTeh[islider]->Connect("TextChanged(char*)", "MultiHSliders", this,
			   "DoText(char*)");
    fHframe[islider]->AddFrame(fHslider[islider], fBly);
    fHframe[islider]->AddFrame(fTeh[islider], fBfly);
    AddFrame(fHframe[islider], fBly);
  }
  
  /* Set main frame name, map sub windows (buttons), initialize layout
     algorithm via Resize() and map main frame */
  SetWindowName(Form("MultiSlider: %d slider(s)", nsliders));
  MapSubwindows();
  Resize(GetDefaultSize());
  MapWindow();
  DoFunction();
}

/*____________________________________________________________________________*/
void MultiHSliders::SetMinMax(Int_t islider, Double_t min, Double_t max)
{
  if (islider >= nsliders) {
    std::cout << "islider: " << islider << " is larger than or equal to nsliders: " << nsliders << std::endl;
    return;
  }
  fSliderMin[islider] = min;
  fSliderMax[islider] = max;
  fSliderPich[islider] = (fSliderMax[islider] - fSliderMin[islider])/1000;
  char buf[32];
  sprintf(buf, "%.5g", fSliderPich[islider]*fHslider[islider]->GetPosition()+fSliderMin[islider]);
  fTbh[islider]->Clear();
  fTbh[islider]->AddText(0, buf);
}
/*____________________________________________________________________________*/
MultiHSliders::~MultiHSliders()
{
  /* Clean up */
  Cleanup();
  delete [] fHframe;
  delete [] fHslider;
  delete [] fTeh;
  delete [] fTbh;
  delete [] fSliderMin;
  delete [] fSliderMax;
  delete [] fSliderPich;
  delete [] HId;
  delete [] HSId;
}

/*____________________________________________________________________________*/
void MultiHSliders::CloseWindow()
{
  /* Called when window is closed via the window manager. */
  delete this;
}

/*____________________________________________________________________________*/
void MultiHSliders::DoText(const char * /*text*/)
{
  /* Handle text entry widgets. */
  TGTextEntry *te = (TGTextEntry *) gTQSender;
  Int_t id = te->WidgetId();
  for (Int_t islider = 0; islider < nsliders; islider++) {
    if (id == HId[islider]) {
      fHslider[islider]->SetPosition((int)((atof(fTbh[islider]->GetString())-fSliderMin[islider])/fSliderPich[islider]));
      break;
    }
  }
  DoFunction();
}

/*____________________________________________________________________________*/
void MultiHSliders::DoSlider()
{
  /* Handle slider widgets. */
  char buf[32];
  TGHSlider *sl = (TGHSlider *) gTQSender;
  Int_t id = sl->WidgetId();
  /* std::cout << "gTQSender: " << ((TObject*) gTQSender)->GetName() << std::endl;
     std::cout << "id: " << id << std::endl; */
  for (Int_t islider = 0; islider < nsliders; islider++) {
    if (id == HSId[islider]) {
      sprintf(buf, "%.5g", fSliderPich[islider]*fHslider[islider]->GetPosition()+fSliderMin[islider]);
      /* std::cout << "buf: " << buf << std::endl; */
      fTbh[islider]->Clear();
      fTbh[islider]->AddText(0, buf);
      fTeh[islider]->SetCursorPosition(fTeh[islider]->GetCursorPosition());
      fTeh[islider]->Deselect();
      gClient->NeedRedraw(fTeh[islider]);
      break;
    }
  }
  DoFunction();
}
/*____________________________________________________________________________*/
void MultiHSliders::DoFunction() {
  Emit("DoFunction()");
}
/*____________________________________________________________________________*/
Double_t MultiHSliders::GetVal(Int_t islider) {
  if (islider >= nsliders) {
    std::cout << "islider: " << islider << " is larger than or equal to nsliders: " << nsliders << std::endl;
    return 0.;
  }
  return fSliderPich[islider]*fHslider[islider]->GetPosition()+fSliderMin[islider];
}

/*____________________________________________________________________________*/
class HistoManager {
private:
  TTree    *tr;
  TH2D     *hist;
  TH2D     *hist_g;
  TH2D     *hist_d;
  TH1D     *hpry;
  TH1D     *hpry2;
  TCanvas  *canvas;
  Int_t nhits;
  Double_t rf, x, y, th, ph, de1, de2;
public:
  HistoManager(MultiHSliders * );
  ~HistoManager();
  void Initialize();
  void DrawHistos();
  ClassDef(HistoManager, 1)
};

/*____________________________________________________________________________*/
HistoManager::HistoManager(MultiHSliders * mhs)
{
  /* TFile *f = TFile::Open("/home/kobayash/mnt/GRUTinizer/output/date20200813/run2214/hist_MakeGRTree_Nobu_20200811_drftok_fin_q.root");*/
  TFile *f = TFile::Open("./hist_MakeGRTree_Nobu_20200811_drftok_fin_q_nogrut.root");
  tr = (TTree *)f->Get("tr");
  tr->Print();
  tr->SetBranchAddress("nhits",   &nhits);
  tr->SetBranchAddress("rf",      &rf);
  tr->SetBranchAddress("x",       &x);
  tr->SetBranchAddress("y",       &y);
  tr->SetBranchAddress("th",      &th);
  tr->SetBranchAddress("ph",      &ph);
  tr->SetBranchAddress("de1",     &de1);
  tr->SetBranchAddress("de2",     &de2);
  canvas = new TCanvas("cV3D","PolyLine3D & PolyMarker3D Window",200,10,500,500);
  canvas->Divide(2,2);
  hist   = new TH2D ("h","h",200, -1000., 1000, 200, -10., 10.);
  hist_g = new TH2D ("hg","hg",200, -1000., 1000, 200, -10., 10.);
  hist_d = new TH2D ("hd","hd",200, -1000., 1000, 200, -10., 10.);
  hpry = new TH1D ("hpry","hpry",200, -10., 10.);
  hpry2 = new TH1D ("hpry2","hpry2",200, -10., 10.);
  mhs->Connect("DoFunction()", "HistoManager", this, "DrawHistos()");
  mhs->DoFunction();
}
HistoManager::~HistoManager(){}

void HistoManager::DrawHistos()
{
  double d2r = 3.141592653589793238/180.;
  ULong64_t NumOfEntries = tr->GetEntries();

  MultiHSliders *mhs = (MultiHSliders *) gTQSender;
  Double_t v0 = mhs->GetVal(0);
  Double_t v1 = mhs->GetVal(1);
  //std::cout << "tr->GetEntries(): " << NumOfEntries << std::endl;
  hist->Reset();
  hist_g->Reset();
  for (Int_t i = 0; i < NumOfEntries; i++) {
    //for (Int_t i = 0; i < 100000; i++) {
    tr->GetEntry(i);
    //hist->Fill(x,th-0.0031*x);
    if ((rf+22*(th-0.0065*x))<920&&(rf+22*(th-0.0065*x))>860){
      hist_g->Fill(x+v0*th*th,th-0.0031*x);
    }
    //if (i %100000 == 0) {
    //std::cout << "Event number: " << i << std::endl;
    ////}
    //std::cout << "x: "  << x << std::endl;
    //std::cout << "y: "  << y << std::endl;
    //std::cout << "th: " << th << std::endl;
    //std::cout << "ph: " << ph << std::endl;
  }

//  dh_dy(hist_g,hist_d);
//  
//  for (Int_t i = 1; i <= hist_d->GetNbinsX(); i++) {
//    for (Int_t j = 1; j <= hist_d->GetNbinsY(); j++) {
//      hpry->SetBinContent(j,hpry->GetBinContent(j)
//			  +hist_d->GetBinContent(i,j));
//    }
//  }
//
//  for (Int_t i = 1; i <= hpry->GetNbinsX(); i++) {
//      hpry2->SetBinContent(i,hpry->GetBinContent(i)*hpry->GetBinContent(i));
//  }

  
  canvas->cd(1);
  hpry2->Draw("colz");
  canvas->cd(2);
  hist_d->Draw("colz");
  canvas->cd(3);
  hist_g->Draw("colz");
  canvas->cd(4);
  hpry->Draw("");
  canvas->Modified();
  canvas->Update();
}


void multi_sliders()
{
  MultiHSliders *mhs = new MultiHSliders(10);
  mhs->SetMinMax(0,0.,1.);
  mhs->SetMinMax(1,-10000.,10000.);
  new HistoManager(mhs);
}
