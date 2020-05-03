#include "TROOT.h"
#include "TEnv.h"
#include "TStyle.h"
#include "TSystem.h"

void ini_kobamac(){
  gEnv->SetValue("Print.Command","lpr -P%p");
  //gEnv->SetValue("Print.Printer","-Pa3c4476 -o PageSize=A4 -o PageRegion=A4");
  gEnv->SetValue("Print.Printer","a3c4476");
  gEnv->SetValue("Print.FileType","ps");
  gEnv->SetValue("Unix.*.Root.UseTTFonts","false"); // If TTFonts are disabled, the TH2::SetShowProjectionX() will causes segv.
  //gEnv->SetValue("Unix.*.Root.UseTTFonts","true");
  //gEnv->SetValue("Unix.*.Root.TTFontPath","");
  gEnv->SetValue("Canvas.ShowEventStatus", "false");
  gEnv->SetValue("Canvas.ShowToolTips", "true");
  gEnv->SetValue("Canvas.ShowToolBar", "true");
  gEnv->SetValue("Canvas.ShowEditor", "false");
  
  gROOT->SetStyle("Plain");
  gStyle->SetHistFillColor(0);
  gStyle->SetHistLineColor(kBlue);
  gStyle->SetFuncColor(kRed);
  gStyle->SetFrameLineWidth(1);
  gStyle->SetPadGridX(0);
  gStyle->SetPadGridY(0);
  gStyle->SetPadColor(0);
  gStyle->SetPadBorderMode(1);
  gStyle->SetCanvasColor(0);
  gStyle->SetTitleFillColor(0);
  gStyle->SetTitleStyle(0);
  gStyle->SetStatColor(0);
  gStyle->SetStatStyle(0);
  gStyle->SetStatX(0.9);  
  gStyle->SetStatY(0.9);
  gStyle->SetStatH(0.4);
  gStyle->SetStatW(0.4);
  gStyle->SetPalette(1);
  //gStyle->SetOptLogz(1);
  //gStyle->SetOptTitle(0);
  gStyle->SetOptStat(1111111);
  gStyle->SetOptFit(1);
  gStyle->SetOptDate(1);
  gStyle->SetEndErrorSize(0);
  gStyle->SetPaperSize(20,26);
  gStyle->SetNdivisions(505,"X");
  gStyle->SetLabelFont(132,"XYZ");
  gStyle->SetTitleFont(132,"XYZ");
  gStyle->SetTitleFont(132,"");
  gStyle->SetTextFont(132);
  gStyle->SetStatFont(132);
  gStyle->GetAttDate()->SetTextFont(132);
  gStyle->SetTitleSize(0.04,"XYZ");
  gStyle->SetTitleSize(0.04,"");
  gStyle->SetLabelSize(0.04,"XYZ");
  gStyle->SetTextSize(0.04);
  gStyle->SetStatFontSize(0.04);
  gStyle->SetTitleFontSize(0.04);  
  /*gROOT->ForceStyle();*/
  
  if (gSystem->AccessPathName("lib/libAllGrutinizer.so")==0) {
    gSystem->Load("lib/libAllGrutinizer.so");
  }


  TString cmd = Form("echo $(cd %s && pwd)", gEnv->GetValue("KOBAMAC_DIR","."));
  TString s = gSystem->GetFromPipe(cmd.Data());
  gEnv->SetValue("KOBAMAC_DIR",s.Data());
  std::cout <<"ini_kobamac.C: KOBAMAC_DIR=" << gEnv->GetValue("KOBAMAC_DIR","") << std::endl;
  
  cmd = Form("find %s -type d -not -path '*/\\.*' | tr -d '\r' | tr '\n' ':' | sed -e 's/:$//'",gEnv->GetValue("KOBAMAC_DIR","."));
  s = gSystem->GetFromPipe(cmd.Data());
  s.Prepend(":");
  s.Prepend(gROOT->GetMacroPath());
  gROOT->SetMacroPath(s.Data());
  gROOT->ProcessLine(Form(".L %s/root/base/TBrowserEx.C+",   gEnv->GetValue("KOBAMAC_DIR",".")));
  gROOT->ProcessLine("TBrowserEx *gBrowserEx = 0;");
  gROOT->ProcessLine(Form(".L %s/root/base/tbr.C",           gEnv->GetValue("KOBAMAC_DIR",".")));
  gROOT->ProcessLine(Form(".L %s/root/base/WaitOneClickX.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  gROOT->ProcessLine(Form(".L %s/root/base/WaitOneClickY.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  gROOT->ProcessLine(Form(".L %s/root/base/WaitOneClickT.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  return;
}
