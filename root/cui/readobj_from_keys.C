TH1* readobj_from_keys(TH1* hist){
  TDirectory *save = gDirectory;
  gROOT->cd();
  TH1* htmp = (TH1*)hist->Clone();
  save->cd();
  return htmp;
}
