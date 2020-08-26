void chk_run2214_20200813(){
  std::cout << "Macro: chk_run2214_2020813.C" << std::endl;
  /*TFile *f = TFile::Open("/home/kobayash/mnt/GRUTinizer/output/date20200813/run2214/hist_MakeGRTree_Nobu_20200811_drftok_fin_q.root"); */
  TFile *f = TFile::Open("hist_MakeGRTree_Nobu_20200811_drftok_fin_q.root");
  TTree *tr = (TTree *)f->Get("tr");
  tr->Print();

  Int_t nhits;
  Double_t rf, x, y, th, ph, de1, de2;
  
  tr->SetBranchAddress("nhits",   &nhits);
  tr->SetBranchAddress("rf",      &rf);
  tr->SetBranchAddress("x",       &x);
  tr->SetBranchAddress("y",       &y);
  tr->SetBranchAddress("th",      &th);
  tr->SetBranchAddress("ph",      &ph);
  tr->SetBranchAddress("de1",     &de1);
  tr->SetBranchAddress("de2",     &de2);

  ULong64_t NumOfEntries = tr->GetEntries();
  std::cout << "tr->GetEntries(): " << NumOfEntries << std::endl;
  Double_t z = -10000.;
  double d2r = 3.141592653589793238/180.;
  TH2D * hist  = new TH2D ("h","h",200, -2000., 2000, 200, -600., 600.);
  TH2D * hist2 = new TH2D ("h2","h2",200, -600., 600, 200, -10., 10.);
  for (Int_t i = 0; i < NumOfEntries; i++) {
    //for (Int_t i = 0; i < 10000; i++) {
    tr->GetEntry(i);
    //if (i %100000 == 0) {
    //std::cout << "Event number: " << i << std::endl;
    ////}
    //std::cout << "x: "  << x << std::endl;
    //std::cout << "y: "  << y << std::endl;
    //std::cout << "th: " << th << std::endl;
    //std::cout << "ph: " << ph << std::endl;
    hist2->Fill(x,th);
    hist->Fill(x+th*d2r*z,y+ph*d2r*z);
  }
  hist->Draw("colz");
  hist2->Draw("colz");
  
  return;
}
