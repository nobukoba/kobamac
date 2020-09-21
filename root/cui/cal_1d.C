void cal_1d(TH1* hist, Double_t a, Double_t b){
  gROOT->ProcessLine(Form(".L %s/root/cui/clone_with_suffix.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  TH1 *hout = (TH1*)gROOT->ProcessLine(Form("clone_with_suffix((TH1*)%p,\"%s\");",hist,"_cal"));
  hout->SetTitle(hist->GetTitle());
  hout->Reset();
  hout->SetBins(hist->GetXaxis()->GetNbins(),
		a*hist->GetXaxis()->GetXmin()+b,
		a*hist->GetXaxis()->GetXmax()+b);
  for (int i=0; i <= xbins1 + 1; i++){
    hout->SetBinContent(i,hist->GetBinContent(i));
    hout->SetBinError(i,hist->GetBinError(i));
  }
  hout->SetEntries(hist->GetEntries());
  hout->Draw("colz");
  gPad->Modified();
  gPad->Update();
  gPad->GetFrame()->SetBit(TBox::kCannotMove);
  return;
}
