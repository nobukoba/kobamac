void run_basias_scirpt(){
  TFile *f = new TFile("/home/kobayash/GRUTinizer/output/date20200525/hist_MakeClover_Nobu_20200525_cpu8_mem8_chk6_run2132_2158_th.root");
  TFile *fout = new TFile("/home/kobayash/GRUTinizer/output/date20200525/hist_MakeClover_Nobu_20200525_cpu8_mem8_chk6_run2132_2158_th_bgsub_basia.root","RECREATE");
  TH2 *hd = (TH2*)f->Get("Nobu/EgEx_rgr_tg_thc");
  TH2 *hb = (TH2*)f->Get("Nobu/EgEx_rgr_bg_thc");
  hd->Draw("colz");
  hb->Draw("colz");
  
  return;
}
