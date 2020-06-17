{
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

  /* Color pallete */
  TString root_ver_str = gROOT->GetVersion();
  root_ver_str.Resize(4);
  Double_t root_ver = root_ver_str.Atof();
  if (root_ver >= 6) {
    gStyle->SetPalette(kBird);
  }else{
    /* kBird pallete */
    Double_t red[9]   = { 0.2082, 0.0592, 0.0780, 0.0232, 0.1802, 0.5301, 0.8186, 0.9956, 0.9764};
    Double_t green[9] = { 0.1664, 0.3599, 0.5041, 0.6419, 0.7178, 0.7492, 0.7328, 0.7862, 0.9832};
    Double_t blue[9]  = { 0.5293, 0.8684, 0.8385, 0.7914, 0.6425, 0.4662, 0.3499, 0.1968, 0.0539};
    Double_t stops[9] = { 0.0000, 0.1250, 0.2500, 0.3750, 0.5000, 0.6250, 0.7500, 0.8750, 1.0000};
    Int_t FI = TColor::CreateGradientColorTable(9, stops, red, green, blue, 255);
    Int_t MyPalette[256];
    for (int i=0;i<255;i++) MyPalette[i] = FI+i;
    gStyle->SetPalette(255, MyPalette);
  }
  
  /* For grutinizer (full path w/o $HOME or ~/) */  
  if (gSystem->AccessPathName("/home/kobayash/GRUTinizer/lib/libAllGrutinizer.so")==0) {
    gSystem->Load("/home/kobayash/GRUTinizer/lib/libAllGrutinizer.so");
  }

  /* For kobamac */
  gROOT->ProcessLine(".x $HOME/nobukoba/kobamac/root/base/init_kobamac.C");
  gROOT->ProcessLine("new TBrowserEx();"); /* Auto launching of the TBrowerEx. This line can be commented out. */ 
}
