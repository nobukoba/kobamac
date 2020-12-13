void rebin_1d_e(TH1* hist, Int_t nbins){
  Double_t xmin = hist->GetXaxis()->GetXmin();
  Double_t xmax = hist->GetXaxis()->GetXmax();
  gROOT->ProcessLine(Form(".L %s/root/cui/rebin_min_max_1d.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  gROOT->ProcessLine(Form("rebin_min_max_1d((TH1*)%p,%d,%f,%f,1);",hist,nbins,xmin,xmax));
  return;
}
