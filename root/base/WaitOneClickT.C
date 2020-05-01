#include "TMarker.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TVirtualX.h"

class WaitOneClickT{
public:
  ~WaitOneClickT(){
    canvas->Disconnect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)",this);
  }
  WaitOneClickT(Double_t *x_in, Double_t *y_in, Double_t a_in):
    x(x_in), y(y_in), a(a_in),
    fCrosshairPos(0), pxlast(0), pylast(0){
    canvas = gPad->GetCanvas();
    canvas->Connect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)",
		    "WaitOneClickT", this, "DrawFunction(Int_t,Int_t,Int_t,TObject*)");
    canvas->WaitPrimitive("TMarker");
  }
  void DrawFunction(Int_t event, Int_t x_in, Int_t y_in, TObject* obj_in){
    if (event == kButton1Down) {
      pxlast = gPad->GetEventX();
      pylast = gPad->GetEventY();
      *x = gPad->AbsPixeltoX(pxlast);
      *y = gPad->AbsPixeltoY(pylast);
      mk.PaintMarker(*x, *y);
      return;
    }
    if (event == kMouseEnter) return;
    if (obj_in == 0) return;
    if (obj_in->InheritsFrom("TVirtualPad") == 0) return;

    TList *listofpri = ((TVirtualPad*) obj_in)->GetListOfPrimitives();
    TIter next(listofpri);
    TObject *obj;
    TH1 *hist = 0;
    while ((obj = next())){
      if (obj->InheritsFrom("TH1")) {
	hist = (TH1*)obj;
	break;
      }
    }
    if(hist == 0) return;
    Double_t aprime = 0.;
    TFrame* f = ((TVirtualPad*) obj_in)->GetFrame();
    Double_t x1 = f->GetX1();
    Double_t x2 = f->GetX2();
    Double_t y1 = f->GetY1();
    Double_t y2 = f->GetY2();
    Double_t dx = x2-x1;
    Double_t dy = y2-y1;
    /* Did not work
       Rectangle_t rec = f->GetBBox();
       std::cout << "rec.fHeight, rec.fWidth:  "
       << rec.fHeight << ", " << rec.fWidth << std::endl; */
    Int_t i1 = ((TVirtualPad*) obj_in)->XtoPixel(x1);
    Int_t i2 = ((TVirtualPad*) obj_in)->XtoPixel(x2);
    Int_t j1 = ((TVirtualPad*) obj_in)->YtoPixel(y1);
    Int_t j2 = ((TVirtualPad*) obj_in)->YtoPixel(y2);
    Int_t di    = i2-i1;
    Int_t dj    = j2-j1;
    aprime = a *(dj/dy)*(dx/di);
    
    canvas->FeedbackMode(kTRUE);
    /*erase old position and draw a line at current position*/
    Int_t pxmin = 0;
    Int_t pxmax = canvas->GetWw();
    Int_t pymin = 0;
    Int_t pymax = gPad->GetWh();
    Int_t pxold = fCrosshairPos%10000;
    Int_t pyold = fCrosshairPos/10000;
    Double_t bold = pyold - aprime*pxold;
    Int_t py1old   = aprime*pxmin+bold;
    Int_t py2old   = aprime*pxmax+bold;
    if (py1old < pymin) {py1old = pymin;}
    if (py1old > pymax) {py1old = pymax;}
    if (py2old < pymin) {py2old = pymin;}
    if (py2old > pymax) {py2old = pymax;}
    Int_t px1old   = pxmin;
    Int_t px2old   = pxmax;
    if (aprime != 0) {
      px1old = (py1old-bold)/aprime;
      px2old = (py2old-bold)/aprime;
    }
    if(pxold||pyold) gVirtualX->DrawLine(px1old,py1old,px2old,py2old);
    if (event == kButton1Down ||
	event == kButton1Up   ||
	event == kMouseLeave) {
      fCrosshairPos = 0;
      return;
    }
    Int_t px    = gPad->GetEventX();
    /* Int_t py    = gPad->GetEventY();+1*/
    Int_t py    = gPad->GetEventY();
    Double_t b  = py - aprime*px;
    Int_t py1   = aprime*pxmin+b;
    Int_t py2   = aprime*pxmax+b;
    if (py1 < pymin) {py1 = pymin;}
    if (py1 > pymax) {py1 = pymax;}
    if (py2 < pymin) {py2 = pymin;}
    if (py2 > pymax) {py2 = pymax;}
    Int_t px1   = pxmin;
    Int_t px2   = pxmax;
    if (aprime != 0) {
      px1 = (py1-b)/aprime;
      px2 = (py2-b)/aprime;
    }
    gVirtualX->DrawLine(px1,py1,px2,py2);
    fCrosshairPos = px + 10000*py;
    return;
  }
private:
  Double_t *x;
  Double_t *y;
  Double_t a;
  Int_t fCrosshairPos;
  Int_t pxlast, pylast;
  TMarker mk;
  TCanvas *canvas;
};
