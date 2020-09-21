TH1* new_histo_with_suffix(TH1* hist, const char *suffix){
  TString str;
  Int_t num = 1;
  while (1) {
    str = Form("%s%s%d",hist->GetName(),suffix,num);
    if (gROOT->Get(str.Data())==0) {break;}
    num++;
  }
  TDirectory *save = gDirectory;
  gROOT->cd();
  TH1* hout = 0;
  if (hist->InheritsFrom("TH2")) {
    hout = new TH2D(str.Data(),hist->GetTitle(),
		    hist->GetXaxis()->GetNbins(),hist->GetXaxis()->GetXmin(),hist->GetXaxis()->GetXmax(),
		    hist->GetYaxis()->GetNbins(),hist->GetYaxis()->GetXmin(),hist->GetYaxis()->GetXmax());
    for (int i=0; i <= hist->GetXaxis()->GetNbins() + 1; i++){
      for (int j=0; j <= hist->GetYaxis()->GetNbins() + 1; j++){
	hout->SetBinContent(i,j,hist->GetBinContent(i,j));
	/*hout->SetBinError(i,j,hist->GetBinError(i,j));*/
      }
    }
  }else if (hist->InheritsFrom("TH1")) {
    hout = new TH1D(str.Data(),hist->GetTitle(),
		    hist->GetXaxis()->GetNbins(),hist->GetXaxis()->GetXmin(),hist->GetXaxis()->GetXmax());
    for (int i=0; i <= hist->GetXaxis()->GetNbins() + 1; i++){
      hout->SetBinContent(i,hist->GetBinContent(i));
      /*hout->SetBinError(i,hist->GetBinError(i));*/
    }
  }
  save->cd();
  return hout;
}
