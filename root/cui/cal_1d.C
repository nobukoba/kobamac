void cal_1d(TH1* hist, Double_t par0, Double_t par1, Int_t erropt=0){
  TString str;
  Int_t num = 1;
  while (1) {
    str = Form("%s_cal%d",hist->GetName(),num);
    if (gROOT->Get(str.Data())==0) {break;}
    num++;
  }
  if (hist->InheritsFrom("TH2")) {
    std::cout << "The histogram should be 1D. Exit."<< std::endl;
    return;
  }
  if (!hist->InheritsFrom("TH1")) {
    std::cout << "The histogram is not 1D. Exit."<< std::endl;
    return;
  }
  TDirectory *save = gDirectory;
  gROOT->cd();
  TH1* hout = new TH1D(str.Data(),hist->GetTitle(),
		       hist->GetXaxis()->GetNbins(),
		       par0+par1*hist->GetXaxis()->GetXmin(),
		       par0+par1*hist->GetXaxis()->GetXmax());
  save->cd();
  for (int i=0; i <= hist->GetXaxis()->GetNbins() + 1; i++){
    hout->SetBinContent(i,hist->GetBinContent(i));
    if (erropt == 1) {
      hout->SetBinError(i,hist->GetBinError(i));
    }
  }
  hout->SetEntries(hist->GetEntries());
  hout->Draw("colz");
  gPad->Modified();
  gPad->Update();
  gPad->GetFrame()->SetBit(TBox::kCannotMove);
  return;
}
