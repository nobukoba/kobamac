TH1D *h_exp;
TH1D *h_BW0;
TH1D *h_BW1;
TH1D *h_BW2;

Double_t func(Double_t *x, Double_t *par){
  Int_t ibin       = h_BW0->FindBin(x[0]);
  Double_t con_BW0 = h_BW0->GetBinContent(ibin);
  Double_t con_BW1 = h_BW1->GetBinContent(ibin);
  Double_t con_BW2 = h_BW2->GetBinContent(ibin);
  Double_t ret =
    par[0]*con_BW0 +
    par[1]*con_BW1 +
    par[2]*con_BW2;
  return ret;
}

void fit_BW(){
  /* Open text files*/
  std::ifstream ifs_exp("exp.txt");
  std::ifstream ifs_BW0("BW_gs.txt");
  std::ifstream ifs_BW1("BW_1st.txt");
  std::ifstream ifs_BW2("BW_2nd.txt");
  /* Contents of the files will be stored in the folloiwing vectors*/
  std::vector<Double_t> x_exp, y_exp;
  std::vector<Double_t> x_BW0, y_BW0;
  std::vector<Double_t> x_BW1, y_BW1;
  std::vector<Double_t> x_BW2, y_BW2;
  
  std::string buf; /* Temoporary buffer string to read the text files */
  Double_t tmp1, tmp2; /*  Temporary value to read the line of the file */
  /* Reading each text file line by line */
  while(ifs_exp && getline(ifs_exp,buf)){
    std::istringstream iss(buf);
    iss >> tmp1 >> tmp2;
    x_exp.push_back(tmp1);
    y_exp.push_back(tmp2);
  }
  while(ifs_BW0 && getline(ifs_BW0,buf)){
    std::istringstream iss(buf);
    iss >> tmp1 >> tmp2;
    x_BW0.push_back(tmp1);
    y_BW0.push_back(tmp2);
  }
  while(ifs_BW1 && getline(ifs_BW1,buf)){
    std::istringstream iss(buf);
    iss >> tmp1 >> tmp2;
    x_BW1.push_back(tmp1);
    y_BW1.push_back(tmp2);
  }
  while(ifs_BW2 && getline(ifs_BW2,buf)){
    std::istringstream iss(buf);
    iss >> tmp1 >> tmp2;
    x_BW2.push_back(tmp1);
    y_BW2.push_back(tmp2);
  }
  Int_t nrows = x_exp.size();
  for (Int_t i; i < nrows; i++) {
    std::cout << x_exp[i] << ", "
	      << y_exp[i] << ", "
	      << y_BW0[i] << ", "
	      << y_BW1[i] << ", "
	      << y_BW2[i]
	      << std::endl;
  }
  /* Prepare histograms containing the text data */
  h_exp = new TH1D("h_exp", "h_exp", nrows, 0., 20.);
  h_BW0 = new TH1D("h_BW0", "h_BW0", nrows, 0., 20.);
  h_BW1 = new TH1D("h_BW1", "h_BW1", nrows, 0., 20.);
  h_BW2 = new TH1D("h_BW2", "h_BW2", nrows, 0., 20.);
  for (Int_t i = 0; i < nrows; i++) {
    h_exp->SetBinContent(i, y_exp[i]);
  }
  for (Int_t i = 0; i < nrows; i++) {
    h_BW0->SetBinContent(i, y_BW0[i]);
  }
  for (Int_t i = 0; i < nrows; i++) {
    h_BW1->SetBinContent(i, y_BW1[i]);
  }
  for (Int_t i = 0; i < nrows; i++) {
    h_BW2->SetBinContent(i, y_BW2[i]);
  }

  /* Prepare the fit function */
  TF1 *f_fit = new TF1("func",func, 11.5, 17.5, 3);
  f_fit->SetNpx(1000);                  /* Step size when the function is drawn */
  f_fit->SetParameters(0.01, 0.1, 0.1); /* Initial parameters for 3 parameters */
  f_fit->SetParLimits(0, 0.03, 1000.);  /* Set a Limits for the 1st parameter */
  f_fit->SetLineColor(kBlack);          /* Line color */
  h_exp->Draw();
  h_exp->Fit("func","R");               /* "R" restricts the fit range of 11.5 < x < 17.5 */
  Double_t par0 = f_fit->GetParameter(0); /* Get fit results */
  Double_t par1 = f_fit->GetParameter(1);
  Double_t par2 = f_fit->GetParameter(2);
  
  TF1 *f_BW0 = new TF1("f_BW0",func, 0., 20., 3); /* For drawing each fit component */
  TF1 *f_BW1 = new TF1("f_BW1",func, 0., 20., 3);
  TF1 *f_BW2 = new TF1("f_BW2",func, 0., 20., 3);
  f_BW0->SetNpx(1000);
  f_BW1->SetNpx(1000);
  f_BW2->SetNpx(1000);
  f_BW0->SetLineColor(kRed);
  f_BW1->SetLineColor(kBlue);
  f_BW2->SetLineColor(kGreen+2);
  f_BW0->SetParameters(par0, 0.,   0.  );
  f_BW1->SetParameters(0.,   par1, 0.  );
  f_BW2->SetParameters(0.,   0.,   par2);
  f_BW0->Draw("l,same");
  f_BW1->Draw("l,same");
  f_BW2->Draw("l,same");
  
  return;
}
