void chk_run2214_20200828(){
  std::cout << "Macro: chk_run2214_2020813.C" << std::endl;
  /*TFile *f = TFile::Open("/home/kobayash/mnt/GRUTinizer/output/date20200813/run2214/hist_MakeGRTree_Nobu_20200811_drftok_fin_q.root"); */
  TFile *f = TFile::Open("/home/kobayash/mnt/GRUTinizer/output/date20200813/run2214/hist_MakeGRTree_Nobu_20200811_drftok_fin_q.root");
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
  double r2d = 180./3.141592653589793238;

    double corr1[2] = {5.72311, 0.00930317};
    double corr2[2] = {9.29558, 0.00571366};
    double corr3[2] = {0.483517, -0.00404179};
    double corr4[2] = {-0.624866, -0.00235683};
  
  TH2D * hist  = new TH2D ("h","h",200, -2000., 2000, 200, -600., 600.);
  TH2D * hist2 = new TH2D ("h2","h2",200, -600., 600, 200, -3., 3.);
  for (Int_t i = 0; i < NumOfEntries; i++) {
    //for (Int_t i = 0; i < 10000; i++) {
    tr->GetEntry(i);

    double thc_rad = -0.401*(th/r2d-3.7264e-4-5.403e-5*x);
    double thc     = thc_rad*r2d;
    double para1 = corr1[0] + corr1[1]*x; 
    double para2 = corr2[0] + corr2[1]*x; 
    double para3 = corr3[0] + corr3[1]*x; 
    double para4 = corr4[0] + corr4[1]*x; 
    //double corrValue = para1*th + para2*th*th + para3*th*th*th + para4*th*th*th*th;
    double corrValue = para1*thc + para2*thc*thc + para3*thc*thc*thc + para4*thc*thc*thc*thc;
    double x_corr = x + corrValue;

    
    //if (i %100000 == 0) {
    //std::cout << "Event number: " << i << std::endl;
    ////}
    //std::cout << "x: "  << x << std::endl;
    //std::cout << "y: "  << y << std::endl;
    //std::cout << "th: " << th << std::endl;
    //std::cout << "ph: " << ph << std::endl;
    if ((rf+22*(th-0.0065*x))<920&&(rf+22*(th-0.0065*x))>860){
      hist2->Fill(x_corr,thc);
    }
    hist->Fill(x+th*d2r*z,y+ph*d2r*z);
  }
  hist->Draw("colz");
  hist2->Draw("colz");
  
  return;
}
