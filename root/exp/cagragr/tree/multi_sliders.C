/* Simple macro showing capabilities of triple slider
   Authors: Bertrand Bellenot, Ilka Antcheva */
/* Modified by Nobu 2020.08.02 */
#include <iostream>
#include "TGButton.h"
#include "TRootEmbeddedCanvas.h"
#include "TGLayout.h"
#include "TH2D.h"
#include "TTree.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TGTextEntry.h"
#include "TGSlider.h"

#define NSLIDERS 2

class MultiHSliders : public TGMainFrame {
  RQ_OBJECT("MultiHSliders")
  private:
  TGLayoutHints       *fBly, *fBfly;
  TGHorizontalFrame   *fHframe[NSLIDERS];
  TGHSlider           *fHslider[NSLIDERS];
  TGTextEntry         *fTeh[NSLIDERS];
  TGTextBuffer        *fTbh[NSLIDERS];
  Double_t            fSliderMin[NSLIDERS];
  Double_t            fSliderMax[NSLIDERS];
  Double_t            fSliderPich[NSLIDERS];
  Int_t               HId[NSLIDERS];
  Int_t               HSId[NSLIDERS];
  Double_t            SliderValues[NSLIDERS];
public:
  MultiHSliders();
  virtual ~MultiHSliders();
  void CloseWindow();
  void DoText(const char *text);
  void DoSlider();
  void DoFunction();
  Double_t* GetVal();
  ClassDef(MultiHSliders, 1)
};

/*______________________________________________________________________________*/
MultiHSliders::MultiHSliders() : TGMainFrame(gClient->GetRoot(), 100, 100)
{
  char buf[32];
  SetCleanup(kDeepCleanup);
  /*--- layout for buttons: top align, equally expand horizontally */
  fBly = new TGLayoutHints(kLHintsTop | kLHintsExpandX, 5, 5, 5, 5);
  /*--- layout for the frame: place at bottom, right aligned */
  fBfly = new TGLayoutHints(kLHintsTop | kLHintsRight, 5, 5, 5, 5);

  for (Int_t islider = 0; islider < NSLIDERS; islider++) {
    HId[islider]  = islider;
    HSId[islider] = islider + NSLIDERS;
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
    fSliderMin[islider] = -10000000.;
    fSliderMax[islider] =  10000000.;
    fSliderPich[islider] = (fSliderMax[islider] - fSliderMin[islider])/1000;
    
    fTeh[islider] = new TGTextEntry(fHframe[islider], fTbh[islider] = new TGTextBuffer(5), HId[islider]);
    fTeh[islider]->SetToolTipText("Pointer Position Value");
    sprintf(buf, "%.5g", fSliderPich[islider]*fHslider[islider]->GetPosition()+fSliderMin[islider]);
    fTbh[islider]->AddText(0, buf);
    fTeh[islider]->Connect("TextChanged(char*)", "MultiHSliders", this,
			   "DoText(char*)");
    fHframe[islider]->AddFrame(fHslider[islider], fBly);
    fHframe[islider]->AddFrame(fTeh[islider], fBfly);
    AddFrame(fHframe[islider], fBly);
  }
  
  /* Set main frame name, map sub windows (buttons), initialize layout
     algorithm via Resize() and map main frame */
  SetWindowName(Form("MultiSlider: %d slider(s)", NSLIDERS));
  MapSubwindows();
  Resize(GetDefaultSize());
  MapWindow();
  DoFunction();
}

/*______________________________________________________________________________*/
MultiHSliders::~MultiHSliders()
{
  /* Clean up */
  Cleanup();
}

/*______________________________________________________________________________*/
void MultiHSliders::CloseWindow()
{
  /* Called when window is closed via the window manager. */
  delete this;
}

/*______________________________________________________________________________*/
void MultiHSliders::DoText(const char * /*text*/)
{
  /* Handle text entry widgets. */
  TGTextEntry *te = (TGTextEntry *) gTQSender;
  Int_t id = te->WidgetId();
  for (Int_t islider = 0; islider < NSLIDERS; islider++) {
    if (id == HId[islider]) {
      fHslider[islider]->SetPosition((int)((atof(fTbh[islider]->GetString())-fSliderMin[islider])/fSliderPich[islider]));
      break;
    }
  }
  DoFunction();
}

/*______________________________________________________________________________*/
void MultiHSliders::DoSlider()
{
  /* Handle slider widgets. */
  char buf[32];
  TGHSlider *sl = (TGHSlider *) gTQSender;
  Int_t id = sl->WidgetId();
  /* std::cout << "gTQSender: " << ((TObject*) gTQSender)->GetName() << std::endl;
     std::cout << "id: " << id << std::endl; */
  for (Int_t islider = 0; islider < NSLIDERS; islider++) {
    if (id == HSId[islider]) {
      sprintf(buf, "%.3f", fSliderPich[islider]*fHslider[islider]->GetPosition()+fSliderMin[islider]);
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
/*______________________________________________________________________________*/
void MultiHSliders::DoFunction() {
  Emit("DoFunction()");
}
/*______________________________________________________________________________*/
Double_t* MultiHSliders::GetVal() {
  for (Int_t islider = 0; islider < NSLIDERS; islider++) {
    SliderValues[islider] = 
      fSliderPich[islider]*fHslider[islider]->GetPosition()+fSliderMin[islider];
  }
  return SliderValues;
}

/*______________________________________________________________________________*/
class HistoManager {
private:
  TTree    *tr;
  TH2D     *hist;
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

/*______________________________________________________________________________*/
HistoManager::HistoManager(MultiHSliders * shs)
{
  /* TFile *f = TFile::Open("/home/kobayash/mnt/GRUTinizer/output/date20200813/run2214/hist_MakeGRTree_Nobu_20200811_drftok_fin_q.root");*/
  TFile *f = TFile::Open("./hist_MakeGRTree_Nobu_20200811_drftok_fin_q.root");
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
  hist = new TH2D ("h","h",200, -1000., 1000, 200, -300., 300.);
  shs->Connect("DoFunction()", "HistoManager", this, "DrawHistos()");
  shs->DoFunction();
}
HistoManager::~HistoManager(){}

void HistoManager::DrawHistos()
{
  double d2r = 3.141592653589793238/180.;
  ULong64_t NumOfEntries = tr->GetEntries();

  MultiHSliders *shs = (MultiHSliders *) gTQSender;
  Double_t a = shs->GetVal()[0];
  Double_t z = shs->GetVal()[1];
  std::cout << "tr->GetEntries(): " << NumOfEntries << std::endl;
  std::cout << "a: " << a << std::endl;
  std::cout << "z: " << z << std::endl;
  //for (Int_t i = 0; i < NumOfEntries; i++) {
  hist->Reset();
  for (Int_t i = 0; i < 100000; i++) {
    tr->GetEntry(i);
    //if (i %100000 == 0) {
    //std::cout << "Event number: " << i << std::endl;
    ////}
    //std::cout << "x: "  << x << std::endl;
    //std::cout << "y: "  << y << std::endl;
    //std::cout << "th: " << th << std::endl;
    //std::cout << "ph: " << ph << std::endl;
    hist->Fill(x+th*d2r*z,y+ph*d2r*z);
  }
  hist->Draw("colz");
  canvas->Modified();
  canvas->Update();
}

void multi_sliders()
{
  std::cout << "NSLIDERS: " << NSLIDERS << std::endl;
  MultiHSliders *shs = new MultiHSliders();
  new HistoManager(shs);
}
