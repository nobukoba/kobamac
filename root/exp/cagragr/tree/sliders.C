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

enum ECommandIdentifiers { HId0, HId1, HSId0, HSId1 };

class SimpleHSliders : public TGMainFrame {
  RQ_OBJECT("SimpleHSliders")
private:
  TGHorizontalFrame   *fHframe0, *fHframe1;
  TGLayoutHints       *fBly, *fBfly;
  TGHSlider           *fHslider0, *fHslider1;
  TGTextEntry         *fTeh0, *fTeh1;
  TGTextBuffer        *fTbh0, *fTbh1;
  Double_t            fSliderMin0, fSliderMax0, fSliderPich0;
  Double_t            fSliderMin1, fSliderMax1, fSliderPich1;
public:
  SimpleHSliders();
  virtual ~SimpleHSliders();
  void CloseWindow();
  void DoText(const char *text);
  void DoSlider();
  void DoFunction();
  Double_t GetA();
  Double_t GetZ();
  ClassDef(SimpleHSliders, 1)
};

/*______________________________________________________________________________*/
SimpleHSliders::SimpleHSliders() : TGMainFrame(gClient->GetRoot(), 100, 100)
{
  char buf[32];
  SetCleanup(kDeepCleanup);
  fHframe0 = new TGHorizontalFrame(this, 0, 0, 0);
  fHslider0 = new TGHSlider(fHframe0, 190, kSlider1 | kScaleDownRight, HSId0,
			    kHorizontalFrame,
			    GetDefaultFrameBackground());
  fHslider0->Connect("PositionChanged(Int_t)", "SimpleHSliders", 
		     this, "DoSlider()");
  fHslider0->SetRange(0,1000);
  fHslider0->SetPosition(500);
  fHframe0->Resize(200, 25);
  fSliderMin0 = -10000.;
  fSliderMax0 =  10000.;
  fSliderPich0 = (fSliderMax0 - fSliderMin0)/1000;

  fHframe1 = new TGHorizontalFrame(this, 0, 0, 0);
  fHslider1 = new TGHSlider(fHframe1, 190, kSlider1 | kScaleDownRight, HSId1,
			    kHorizontalFrame,
			    GetDefaultFrameBackground());
  fHslider1->Connect("PositionChanged(Int_t)", "SimpleHSliders", 
		     this, "DoSlider()");
  fHslider1->SetRange(0,1000);
  fHslider1->SetPosition(500);
  fHframe1->Resize(200, 25);
  fSliderMin1 = -10000.;
  fSliderMax1 =  10000.;
  fSliderPich1 = (fSliderMax1 - fSliderMin1)/1000;

  fTeh0 = new TGTextEntry(fHframe0, fTbh0 = new TGTextBuffer(5), HId0);
  fTeh0->SetToolTipText("Pointer Position Value");
  sprintf(buf, "%.3f", fSliderPich0*fHslider0->GetPosition()+fSliderMin0);
  fTbh0->AddText(0, buf);
  fTeh0->Connect("TextChanged(char*)", "SimpleHSliders", this,
		 "DoText(char*)");
  
  fTeh1 = new TGTextEntry(fHframe1, fTbh1 = new TGTextBuffer(5), HId1);
  fTeh1->SetToolTipText("Pointer Position Value");
  sprintf(buf, "%.3f", fSliderPich1*fHslider1->GetPosition()+fSliderMin1);
  fTbh1->AddText(0, buf);
  fTeh1->Connect("TextChanged(char*)", "SimpleHSliders", this,
		 "DoText(char*)");
  
  /*--- layout for buttons: top align, equally expand horizontally */
  fBly = new TGLayoutHints(kLHintsTop | kLHintsExpandX, 5, 5, 5, 5);
  /*--- layout for the frame: place at bottom, right aligned */
  fBfly = new TGLayoutHints(kLHintsTop | kLHintsRight, 5, 5, 5, 5);
  
  fHframe0->AddFrame(fHslider0, fBly);
  fHframe0->AddFrame(fTeh0, fBfly);
  fHframe1->AddFrame(fHslider1, fBly);
  fHframe1->AddFrame(fTeh1, fBfly);
  AddFrame(fHframe0, fBly);
  AddFrame(fHframe1, fBly);
  
  /* Set main frame name, map sub windows (buttons), initialize layout
     algorithm via Resize() and map main frame */
  SetWindowName("Slider Demo");
  MapSubwindows();
  Resize(GetDefaultSize());
  MapWindow();
  DoFunction();
}

/*______________________________________________________________________________*/
SimpleHSliders::~SimpleHSliders()
{
  /* Clean up */
  Cleanup();
}

/*______________________________________________________________________________*/
void SimpleHSliders::CloseWindow()
{
  /* Called when window is closed via the window manager. */
  delete this;
}

/*______________________________________________________________________________*/
void SimpleHSliders::DoText(const char * /*text*/)
{
  /* Handle text entry widgets. */
  TGTextEntry *te = (TGTextEntry *) gTQSender;
  Int_t id = te->WidgetId();
  
  switch (id) {
  case HId0:
    fHslider0->SetPosition((int)((atof(fTbh0->GetString())-fSliderMin0)/fSliderPich0));
    break;
  case HId1:
    fHslider1->SetPosition((int)((atof(fTbh1->GetString())-fSliderMin1)/fSliderPich1));
    break;
  default:
    break;
  }
  DoFunction();
}

/*______________________________________________________________________________*/
void SimpleHSliders::DoSlider()
{
  /* Handle slider widgets. */
  char buf[32];
  TGHSlider *sl = (TGHSlider *) gTQSender;
  Int_t id = sl->WidgetId();
  
  /* std::cout << "gTQSender: " << ((TObject*) gTQSender)->GetName() << std::endl;
     std::cout << "id: " << id << std::endl; */

  switch (id) {
  case HSId0:
    sprintf(buf, "%.3f", fSliderPich0*fHslider0->GetPosition()+fSliderMin0);
    /* std::cout << "buf: " << buf << std::endl; */
    fTbh0->Clear();
    fTbh0->AddText(0, buf);
    fTeh0->SetCursorPosition(fTeh0->GetCursorPosition());
    fTeh0->Deselect();
    gClient->NeedRedraw(fTeh0);
    break;
  case HSId1:
    sprintf(buf, "%.3f", fSliderPich1*fHslider1->GetPosition()+fSliderMin1);
    fTbh1->Clear();
    fTbh1->AddText(0, buf);
    fTeh1->SetCursorPosition(fTeh1->GetCursorPosition());
    fTeh1->Deselect();
    gClient->NeedRedraw(fTeh1);
    break;
  default:
    break;
  }
  DoFunction();
}

/*______________________________________________________________________________*/
void SimpleHSliders::DoFunction() {
  Emit("DoFunction()");
}
/*______________________________________________________________________________*/
Double_t SimpleHSliders::GetA() {
  return fSliderPich0*fHslider0->GetPosition()+fSliderMin0;
}
/*______________________________________________________________________________*/
Double_t SimpleHSliders::GetZ() {
  return fSliderPich1*fHslider1->GetPosition()+fSliderMin1;
}

/*______________________________________________________________________________*/
/* This works in ROOT v6, but does not work in ROOT v5.34.38
at the distination, the double value becomes 4.94066e-322 or so
void SimpleHSliders::DoFunction(double val0, double val1)
{
  std::cout << "val0" << val0 <<std::endl;
  std::cout << "val1" << val1 <<std::endl;
  Long_t args[2];
  args[0] = (Long_t)val0;
  args[1] = (Long_t)val1;
  
  Emit("DoFunction(double, double)",args);
}
*/
/*______________________________________________________________________________*/
class HistoManager {
private:
  TTree    *tr;
  TH2D     *hist;
  TCanvas  *canvas;
  Int_t nhits;
  Double_t rf, x, y, th, ph, de1, de2;
public:
  HistoManager(SimpleHSliders * );
  ~HistoManager();
  void Initialize();
  void DrawHistos();
  ClassDef(HistoManager, 1)
};

/*______________________________________________________________________________*/
HistoManager::HistoManager(SimpleHSliders * shs)
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
HistoManager::~HistoManager()
{
}

void HistoManager::DrawHistos()
{
  double d2r = 3.141592653589793238/180.;
  ULong64_t NumOfEntries = tr->GetEntries();

  SimpleHSliders *shs = (SimpleHSliders *) gTQSender;
  Double_t a = shs->GetA();
  Double_t z = shs->GetZ();
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

void sliders()
{
  SimpleHSliders *shs = new SimpleHSliders();
  new HistoManager(shs);
}
