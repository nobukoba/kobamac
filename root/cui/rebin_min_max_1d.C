void rebin_1d(TH1* hist, Int_t nbins, Int_t erropt=0){
  gROOT->ProcessLine(Form(".L %s/root/cui/clone_with_suffix.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  TH1 *hout = (TH1*)gROOT->ProcessLine(Form("clone_with_suffix((TH1*)%p,\"%s\");",hist,"_rbn"));
  hout->SetTitle(hist->GetTitle());
  hout->Reset();
  hout->SetBins(nbins,hist->GetXaxis()->GetXmin(),hist->GetXaxis()->GetXmax());
  
  double xmin   = hist->GetXaxis()->GetXmin();
  double xmax   = hist->GetXaxis()->GetXmax();
  int    xbins1 = hist->GetNbinsX();
  double dx1    = (xmax - xmin) / xbins1;
  int    xbins2 = nbins;
  double dx2    = (xmax - xmin) / xbins2;
  
  double *con_arr1 = new double[xbins1 + 2];
  double *con_arr2 = new double[xbins2 + 2];
  double *err_arr1 = new double[xbins1 + 2];
  double *err_arr2 = new double[xbins2 + 2];
  
  for (int i=0; i <= xbins1 + 1; i++){
    con_arr1[i] = hist->GetBinContent(i);
    err_arr1[i] = hist->GetBinError(i);
  }
  for (int i=0; i <= xbins2 + 1; i++){
    con_arr2[i] = 0.;
    err_arr2[i] = 0.;
  }
  
  /* Rebin in the x axis */
  int i2_cur = 1;
  for (int i1 = 1; i1 <= xbins1; i1++) {
    double xlow1 = (i1 - 1) * dx1 + xmin;
    double xup1  = i1 * dx1 + xmin;
    for (int i2 = i2_cur; i2 <= xbins2; i2++){
      double xlow2 = (i2 - 1) * dx2 + xmin;
      double xup2  = i2 * dx2 + xmin;
      double fac = 1.;
      if (xlow1 < xlow2) {
	fac = fac - ((xlow2 - xlow1) / dx1);
      }
      if (xup1 > xup2) {
	fac = fac - ((xup1 - xup2) / dx1);
      }
      if ((xlow1 == xup2) || (xup1 == xlow2)) {
	fac = 0.;
      }
      /* std::cout << "i1, i2, j, fac, "
	 << i1 << ", " << i2 << ", "  << j << ", "  << fac << std::endl;*/
      con_arr2[i2] = con_arr2[i2] + fac * con_arr1[i1];
      err_arr2[i2] = sqrt(pow(err_arr2[i2],2) + pow(fac * err_arr1[i1],2));
      if (xup1 <= xup2) {
	i2_cur = i2;
	break;
      }
    }
  }
  
  for (int i = 1; i <= xbins2; i++){
    hout->SetBinContent(i,con_arr2[i]);
    hout->SetBinError(i,err_arr2[i]);
  }
  
  delete [] con_arr1;
  delete [] con_arr2;
  delete [] err_arr1;
  delete [] err_arr2;
  
  hout->SetEntries(hist->GetEntries());
  hout->Draw("colz");
  gPad->Modified();
  gPad->Update();
  gPad->GetFrame()->SetBit(TBox::kCannotMove);
  return;
}
