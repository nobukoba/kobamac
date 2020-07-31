void get_histo_stat(TH1* hist){
  std::cout << std::endl;
  std::cout << "Histo name: " << hist->GetName() << ", title: " <<  hist->GetTitle() << std::endl;
  std::cout << "Histo name: " << hist->GetName() << ", title: " <<  hist->GetTitle() << std::endl;

  Int_t xfirst = hist->GetXaxis()->GetFirst();
  Int_t xlast  = hist->GetXaxis()->GetLast();
  Int_t nbins = hist->GetXaxis()->GetNbins();
  Double_t integ_sel = hist->Integral(xfirst,xlast);
  Double_t integ_ful = hist->Integral(0,nbins);
  
  std::cout << "Integral (selected range): " << integ_sel << std::endl;
  std::cout << "Integral (full range):     " << integ_ful << std::endl;
  
  return;
}
