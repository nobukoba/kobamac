#include "TFile.h"
#include "TH2.h"

void background_sub(){
  /*TFile *f = new TFile("hist_MakeClover_Nobu_20200427_toff2_2132_2158_th.root");*/
  TFile *f = new TFile("output/date20200525/hist_MakeClover_Nobu_20200525_cpu8_mem8_chk6_run2132_2158_th.root");
  TFile *fout = new TFile("output/date20200525/hist_MakeClover_Nobu_20200525_cpu8_mem8_chk6_run2132_2158_th_bgsub.root","RECREATE");
  TH2 *hd = (TH2*)f->Get("Nobu/EgEx_rgr_tg_thc");
  TH2 *hb = (TH2*)f->Get("Nobu/EgEx_rgr_bg_thc");
  hd->Draw("colz");
  hb->Draw("colz");
  
  TH2 *histOut = (TH2*)hd->Clone("histOut");
  //histOut->Reset();
  double weight = 0.113;
  //histOut->Add(hb,-0.113);
  //histOut->Draw("colz");
  int r = 10;
  int limit_m = 3;
  int limit_n = 5;
  for (int i = 1; i <= hd->GetNbinsX(); i++){
    for (int j = 1; j <= hd->GetNbinsY(); j++){
      for (int k = 0; k < r; k++){
	int m, n;
	for (m = k, n = 0; n < k+1; m--, n++) {
	  if ((m > limit_m) || (n > limit_n)) continue;
	  if (hb->GetBinContent(i, j)==0) break;
	  double difference = hd->GetBinContent(i+n, j+m) - weight*hb->GetBinContent(i, j);
	  double error = histOut->GetBinError(i+n, j+m) * histOut->GetBinError(i+n, j+m);
	  error += weight * weight * hb->GetBinContent(i, j);
	  error = sqrt(error);
	  histOut->SetBinError(i+n, j+m, error);
	  if (difference >= 0.0) {
	    histOut->SetBinContent(i+n, j+m, difference);
	    hb->SetBinContent(i, j, 0);
	  }else{
	    histOut->SetBinContent(i+n, j+m, 0);
	    hb->SetBinContent(i, j, -difference/weight);
	  }
	}
      }
    }  
  }
  histOut->Draw("colz");
  fout->Write();
  
  return;  
}
