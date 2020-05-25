void init() {
  gROOT->SetStyle("Plain");
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
  TFile *f = new TFile("output/date20200525/hist_MakeClover_Nobu_20200525_cpu8_mem8_chk6_run2132_2158_th_bgsub.root");
  TH2 *hcln = (TH2*)f->Get("histOut");
  TH2 *hcln2 = (TH2*)hcln->Clone("hcln2");
  TGraph *gr = new TGraph();
  Double_t hwid = 0.1;
  gr->SetPoint(0, 0.,  hwid);
  gr->SetPoint(1, 0., -hwid);
  gr->SetPoint(2,10.,10.-hwid);
  gr->SetPoint(3,10.,10.+hwid);
  gr->SetPoint(4, 0.,  0.1);
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
  latex.SetTextSize(0.06);
  latex.SetTextAlign(13);  //align at top
  p1->cd();
  latex.DrawLatexNDC(.17,.92,"a)");
  p2->cd();
  latex.DrawLatexNDC(.17,.92,"b)");
  c1->Print("coin_matrix_208pb_2col.pdf");
  return;
}
