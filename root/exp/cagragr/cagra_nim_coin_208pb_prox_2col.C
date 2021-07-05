void init() {
  gROOT->SetStyle("Plain");
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
  //gStyle->SetPaperSize(10,10);
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetCanvasBorderSize(0);
  gStyle->SetFrameBorderMode(0);
  gStyle->SetFrameBorderSize(0);
  gStyle->SetFrameLineWidth(1);
  gStyle->SetFuncWidth(1);
  gStyle->SetHistLineWidth(1);
  gStyle->SetGridWidth(1);
  gStyle->SetLineWidth(1);
  gStyle->SetTickLength(0.04,"x");
  gStyle->SetTickLength(0.02,"y");
  gStyle->SetPadTickX(0); // 1: top tick on 
  gStyle->SetPadTickY(0); // 1: left tick on
  gStyle->SetTitleAlign(23);
  //gStyle->SetTitleX(0.02); // graph title y position
  //gStyle->SetTitleY(0.02); // graph title x position
  gStyle->SetTitleXOffset(0.85); // length : y title <--> y axis
  gStyle->SetTitleYOffset(0.04); // length : x title <--> x axis
  gStyle->SetTitleBorderSize(0);
  gStyle->SetTitleFillColor(0);
  gStyle->SetTitleColor(1);
  gStyle->SetLabelFont(132,"xyz");
  gStyle->SetTitleFont(132,"xyz");
  gStyle->SetTitleFont(132,"");
  gStyle->SetTextFont(132);
  gStyle->SetStatFont(132);
  gStyle->SetNdivisions(505,"XYZ");
  gStyle->SetNdivisions(305,"Y");
  //gStyle->SetLabelSize(0.5,"xyz");
  //gStyle->SetTitleSize(0.5,"xyz");
  //gStyle->SetTitleSize(0.3,"");
  gStyle->SetPadTopMargin(0.06);
  gStyle->SetPadRightMargin(0.12);
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPadBottomMargin(0.15);
  //gStyle->SetLabelOffset(0.015,"xy");
  gStyle->SetOptStat(0);
  gStyle->SetOptLogz(1);
}
/* Memo
First 3-: 2.614522 MeV --> X: -3.70022e+02 at GR of 8.8 deg.
Last 1-: 7.3324 MeV --> X: 3.46397e+02 at GR of 8.8 deg.
E_x = 0.006585*X + 5.051251
*/

void cagra_nim_coin_208pb_prox_2col() {
  init();
  gROOT->ForceStyle(); 
  //TFile *f = new TFile("date20190727_merged2_w_bgsub.root");
  //TH2 *hcln = (TH2*)f->Get("histOut");
  //TFile *f = new TFile("/home/kobayash/GRUTinizer/output/date20200603/hist_MakeClover_Nobu_20200603_e_above_2132_2158_nogrut_basia.root");
  TFile *f = new TFile("/home/kobayash/mnt/GRUTinizer/output/date20200603/hist_MakeClover_Nobu_20200603_e_above_2132_2158_nogrut_basia.root");
  //TH2 *hcln = (TH2*)f->Get("histOut");
  TH2 *hcln = (TH2*)f->Get("EgEx_rgr_tg_thc");
  std::cout << "hcln: " << hcln << std::endl;
  TH2 *hcln2 = (TH2*)hcln->Clone("hcln2");
  TGraph *gr = new TGraph();
  Double_t hwid = 0.05;
  gr->SetPoint(0, 0.,  hwid);
  gr->SetPoint(1, 0., -hwid);
  gr->SetPoint(2,10.,10.-hwid);
  gr->SetPoint(3,10.,10.+hwid);
  gr->SetPoint(4, 0.,  0.1);
  TGraph *gr2 = new TGraph();
  Double_t hwid = 0.05;
  gr2->SetPoint(0, 2.614522, 2.614522+hwid-2.614522);
  gr2->SetPoint(1, 2.614522, 2.614522-hwid-2.614522);
  gr2->SetPoint(2,10.,10.-hwid-2.614522);
  gr2->SetPoint(3,10.,10.+hwid-2.614522);
  gr2->SetPoint(4, 2.614522, 2.614522+hwid-2.614522);
  gr2->SetLineStyle(7);
  TCutG *cutg = new TCutG("CUTG",gr->GetN(),
			  gr->GetX(),
			  gr->GetY());
  for (Int_t i = 0; i <= hcln->GetNbinsX()+1; i++) {
    Double_t xx = hcln->GetXaxis()->GetBinCenter(i);
    for (Int_t j = 0; j <= hcln->GetNbinsY()+1; j++) {
      Double_t yy = hcln->GetYaxis()->GetBinCenter(j);
      if (cutg->IsInside(xx,yy) != 1) {
	hcln2->SetBinContent(i,j,0);
      }
    }
  }
  TH1D *hprx = new TH1D("prox", hcln2->GetTitle(), hcln2->GetNbinsX(),
			hcln2->GetXaxis()->GetXmin(), hcln2->GetXaxis()->GetXmax());
  for (Int_t i = 0; i <= hcln2->GetNbinsX()+1; i++) {
    for (Int_t j = 1; j <= hcln2->GetNbinsY(); j++) {
      hprx->Fill(hcln2->GetXaxis()->GetBinCenter(i),
		 hcln2->GetBinContent(i,j));
    }
  }
  hprx->SetEntries(hcln2->GetEntries());
  hcln->GetXaxis()->SetRangeUser(1.5,8.5);
  hprx->GetXaxis()->SetRangeUser(1.5,8.5);
  hcln2->Draw("colz");
  init();
  //gROOT->ForceStyle();
  hcln->GetYaxis()->SetNdivisions(505);
  
  hcln->SetStats(0);
  hprx->SetStats(0);
  hprx->SetLineColor(1);
  hprx->SetTitleSize(0.06,"xyz");
  hprx->SetLabelSize(0.06,"xyz");
  hcln->SetTitleSize(0.06,"xyz");
  hcln->SetLabelSize(0.06,"xyz");
  //hcln->SetLabelSize(0.,"x");
  hprx->GetXaxis()->SetTitleOffset(1.0);
  hcln->GetXaxis()->SetTitleOffset(1.1);
  hprx->GetYaxis()->SetTitleOffset(1.2);
  hcln->GetYaxis()->SetTitleOffset(1.0);
  hcln->GetZaxis()->SetTitleOffset(0.3);
  hcln->GetXaxis()->SetTitle("Excitation energy #it{E_{x}} (MeV)");
  hprx->GetXaxis()->SetTitle("Excitation energy #it{E_{x}} (MeV)");
  hprx->GetYaxis()->SetTitle("Counts / 10 keV");
  hcln->GetYaxis()->SetTitle("#gamma-ray energy #it{E_{#gamma}} (MeV)");
  hprx->GetXaxis()->CenterTitle();
  hprx->GetYaxis()->CenterTitle();
  hcln->GetYaxis()->CenterTitle();
  hprx->SetTitle("");
  hcln->SetTitle("");

  Double_t r = 0.54;
  Double_t epsilon = 2-1./r;
  
  TCanvas *c1 = new TCanvas("c1","c1",1000,500);
  TPad *p1 = new TPad("p1", "p1", 0.0, 0.0, 0.5, 1.0);
  //p1->SetBottomMargin(epsilon);
  p1->SetLogz(1);
  p1->Draw();
  
  TPad *p2 = new TPad("p2", "p2", 0.5, 0.0, 1.0, 1.0);
  //p2->SetTopMargin(0);
  p2->SetLogy(1);
  p2->Draw();
  p2->SetFillColor(0);
  p2->SetFillStyle(0);
  init();
  
  p1->cd();
  hcln->Draw("");
  c1->Update();
  hcln->GetZaxis()->SetLabelOffset(-0.01);
  hcln->Draw("colz");
  c1->Update();
  TPaletteAxis *palette = (TPaletteAxis*)hcln->GetListOfFunctions()->FindObject("palette");
  gr->Draw("l");
  gr2->Draw("l");
  //palette->SetLabelOffset(0.1);
  //palette->SetTitleOffset(0.1);
  palette->SetTitleSize(0.1);
  //hcln->Draw("colz");
  p2->cd();
  //hprx->GetXaxis()->SetRange(2.,9.);
  hprx->Draw("hist");
  //gPad->Update();
  //hprx->Draw();
  //TCanvas
  //(GXCEg_rgr_tg_thc_lw_add1_cln1);
  TLatex latex;
  latex.SetTextSize(0.05);
  latex.SetTextAlign(13);  //align at top
  p1->cd();
  latex.DrawLatexNDC(.17,.92,"a) {}^{208}Pb(#it{p}#kern[-0.5]{,}#kern[0.1]{#it{p'}#gamma})");
  latex.DrawLatexNDC(.17,.86,"   #it{E}_{#it{p}} = 80 MeV");
  latex.DrawLatexNDC(.17,.80,"   #it{#theta}_{#it{s}} = 8.8^{o}");
  p2->cd();
  latex.DrawLatexNDC(.17,.92,"b)");
  latex.SetTextSize(0.04);
  latex.SetTextAlign(21);
  latex.DrawLatex(4.84+0.05,40,"1^{-}");
  latex.DrawLatex(5.51+0.05,35,"1^{-}");
  latex.DrawLatex(6.26+0.05,25,"1^{-}");
  latex.DrawLatex(7.33+0.05,10,"1^{-}");
  c1->Print("coin_matrix_208pb_2col_20210705.pdf");
  return;
}
