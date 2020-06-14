void cout_all_2d(TH2* hist){
  Double_t sum = 0.;
  for (int i=1; i <= hist->GetNbinsX(); i++){
    for (int j=0; j <= hist->GetNbinsY(); j++){
      sum = sum + hist->GetBinContent(i,j);
    }
  }
  std::cout << "sum: " << sum << std::endl;
  return;
}
