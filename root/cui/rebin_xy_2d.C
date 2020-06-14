void rebin_xy_2d(TH2* hist, Int_t xbins, Int_t ybins){
  gROOT->ProcessLine(Form(".L %s/root/cui/clone_with_suffix.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  TH2 *hout = (TH2*)gROOT->ProcessLine(Form("clone_with_suffix((TH1*)%p,\"%s\");",hist,"_rbn"));
  hout->SetTitle(hist->GetTitle());
  hout->Reset();
  hout->SetBins(xbins,hist->GetXaxis()->GetXmin(),hist->GetXaxis()->GetXmax(),
		ybins,hist->GetYaxis()->GetXmin(),hist->GetYaxis()->GetXmax());
  
  double xmin   = hist->GetXaxis()->GetXmin();
  double xmax   = hist->GetXaxis()->GetXmax();
  int    xbins1 = hist->GetNbinsX();
  double dx1    = (xmax - xmin) / xbins1;
  int    xbins2 = xbins;
  double dx2    = (xmax - xmin) / xbins2;
  double ymin   = hist->GetYaxis()->GetXmin();
  double ymax   = hist->GetYaxis()->GetXmax();
  int    ybins1 = hist->GetNbinsY();
  double dy1    = (ymax - ymin) / ybins1;
  int    ybins2 = ybins;
  double dy2    = (ymax - ymin) / ybins2;
  
  double **con_arr0 = new double*[xbins2 + 2];
  double **con_arr1 = new double*[xbins1 + 2];
  double **con_arr2 = new double*[xbins2 + 2];
  double **err_arr0 = new double*[xbins2 + 2];
  double **err_arr1 = new double*[xbins1 + 2];
  double **err_arr2 = new double*[xbins2 + 2];
  for(int i = 0; i < xbins2 + 2; i++) {con_arr0[i] = new double[ybins1 + 2];}
  for(int i = 0; i < xbins1 + 2; i++) {con_arr1[i] = new double[ybins1 + 2];}
  for(int i = 0; i < xbins2 + 2; i++) {con_arr2[i] = new double[ybins2 + 2];}
  for(int i = 0; i < xbins2 + 2; i++) {err_arr0[i] = new double[ybins1 + 2];}
  for(int i = 0; i < xbins1 + 2; i++) {err_arr1[i] = new double[ybins1 + 2];}
  for(int i = 0; i < xbins2 + 2; i++) {err_arr2[i] = new double[ybins2 + 2];}
  
  for (int i=0; i <= xbins2 + 1; i++){
    for (int j=0; j <= ybins1 + 1; j++){
      con_arr0[i][j] = 0.;
      err_arr0[i][j] = 0.;
    }
  }
  for (int i=0; i <= xbins1 + 1; i++){
    for (int j=0; j <= ybins1 + 1; j++){
      con_arr1[i][j] = hist->GetBinContent(i,j);
      err_arr1[i][j] = hist->GetBinError(i,j);
    }
  }
  for (int i=0; i <= xbins2 + 1; i++){
    for (int j=0; j <= ybins2 + 1; j++){
      con_arr2[i][j] = 0.;
      err_arr2[i][j] = 0.;
    }
  }
  
  /* Rebin in the x axis */
  for (int j = 1; j <= ybins1; j++) {
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
	con_arr0[i2][j] = con_arr0[i2][j] + fac * con_arr1[i1][j];
	err_arr0[i2][j] = sqrt(pow(err_arr0[i2][j],2) + pow(fac * err_arr1[i1][j],2));
	if (xup1 <= xup2) {
	  i2_cur = i2;
	  break;
	}
      }
    }
  }
   
  /* Rebin in the y axis */
  for (int i = 1; i <= xbins2; i++){
    int j2_cur = 1;
    for (int j1 = 1; j1 <= ybins1; j1++){
      double ylow1 = (j1 - 1) * dy1 + ymin;
      double yup1  = j1 * dy1 + ymin;
      for (int j2 = j2_cur; j2 <= ybins2; j2++){
	double ylow2 = (j2 - 1) * dy2 + ymin;
	double yup2  = (j2) * dy2 + ymin;
	double  fac = 1.;
	if (ylow1 < ylow2) {
	  fac = fac - ((ylow2 - ylow1) / dy1);
	}
	if (yup1 > yup2) {
	  fac = fac - ((yup1 - yup2) / dy1);
	}
	if ((ylow1 == yup2) || (yup1 == ylow2)) {
	  fac = 0.;
	}
	/* std::cout << "i, j1, j2, fac, "
	   << i << ", " << j1 << ", "  << j2 << ", "  << fac << std::endl;*/
	con_arr2[i][j2] = con_arr2[i][j2] + fac * con_arr0[i][j1];
	err_arr2[i][j2] = sqrt(pow(err_arr2[i][j2],2) + pow(fac * err_arr0[i][j1],2));
	if (yup1 <= yup2) {
	  j2_cur = j2;
	  break;
	}
      }
    }
  }
  
  for (int i = 1; i <= xbins2; i++){
    for (int j = 1; j <= ybins2; j++){
      hout->SetBinContent(i,j,con_arr2[i][j]);
      hout->SetBinError(i,j,err_arr2[i][j]);
    }
  }
  
  for(int i = 0; i < xbins2 + 2; i++) {delete [] con_arr0[i];}
  for(int i = 0; i < xbins1 + 2; i++) {delete [] con_arr1[i];}
  for(int i = 0; i < xbins2 + 2; i++) {delete [] con_arr2[i];}
  for(int i = 0; i < xbins2 + 2; i++) {delete [] err_arr0[i];}
  for(int i = 0; i < xbins1 + 2; i++) {delete [] err_arr1[i];}
  for(int i = 0; i < xbins2 + 2; i++) {delete [] err_arr2[i];}
  delete [] con_arr0;
  delete [] con_arr1;
  delete [] con_arr2;
  delete [] err_arr0;
  delete [] err_arr1;
  delete [] err_arr2;
  
  hout->SetEntries(hist->GetEntries());
  hout->Draw("colz");
  gPad->Modified();
  gPad->Update();
  gPad->GetFrame()->SetBit(TBox::kCannotMove);
  return;
}
