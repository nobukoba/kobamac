void cal_1d(TH1* hist, Double_t a, Double_t b){
  gROOT->ProcessLine(Form(".L %s/root/cui/new_histo_with_suffix.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  TH1 *hout = (TH1*)gROOT->ProcessLine(Form("new_histo_with_suffix((TH1*)%p,\"%s\");",hist,"_cal"));
  hout->SetTitle(hist->GetTitle());
  hout->GetXaxis()->SetLimits(a*hist->GetXaxis()->GetXmin()+b,
			      a*hist->GetXaxis()->GetXmax()+b);
  hout->SetEntries(hist->GetEntries());
  hout->Draw("colz");
  gPad->Modified();
  gPad->Update();
  gPad->GetFrame()->SetBit(TBox::kCannotMove);
  return;
}
