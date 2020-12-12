void rebin_min_max_1d(TH1* hist, Int_t nbins, Double_t xmin, Double_t xmax, Int_t erropt=0){
  gROOT->ProcessLine(Form(".L %s/root/cui/clone_with_suffix.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  TH1 *hout = (TH1*)gROOT->ProcessLine(Form("clone_with_suffix((TH1*)%p,\"%s\");",hist,"_rbn"));
  hout->SetTitle(hist->GetTitle());
  hout->Reset();
  hout->SetBins(nbins, xmin, xmax);
  for (Int_t j = 1; j <= nbins; j++) {
    Double_t xlow_hout = hout->GetXaxis()->GetBinLowEdge(j);
    Double_t xup_hout  = hout->GetXaxis()->GetBinUpEdge(j);
    Int_t    ilow_hist = hist->FindBin(xlow_hout);
    Int_t    iup_hist  = hist->FindBin(xup_hout);
    Double_t sum   = 0.;
    Double_t err   = 0.;
    for (Int_t i = ilow_hist; i <= iup_hist; i++){
      Double_t fac   = 1.;
      if (i == ilow_hist) {
	Double_t dx_hist   = hist->GetXaxis()->GetBinWidth(i);
	Double_t xlow_hist = hist->GetXaxis()->GetBinLowEdge(i);
	fac = fac - (xlow_hout - xlow_hist) / dx_hist;
      }
      if (i == iup_hist) {
	Double_t dx_hist  = hist->GetXaxis()->GetBinWidth(i);
	Double_t xup_hist = hist->GetXaxis()->GetBinUpEdge(i);
	fac = fac - (xup_hist - xup_hout) / dx_hist;
      }
      sum += fac * hist->GetBinContent(i);
      if (erropt == 1) {
	err =  sqrt(pow(err,2) + pow(fac * hist->GetBinError(i),2));
      }
    }
    hout->SetBinContent(j, sum);
    if (erropt == 1) {
      hout->SetBinError(j, err);
    }
  }
  hout->SetEntries(hist->GetEntries());
  hout->Draw("colz");
  gPad->Modified();
  gPad->Update();
  gPad->GetFrame()->SetBit(TBox::kCannotMove);
  return;
}
