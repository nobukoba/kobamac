void rebin_x_2d(TH2* hist, Int_t nbins){
  gROOT->ProcessLine(Form(".L %s/root/cui/clone_with_suffix.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  TH2 *hout = (TH2*)gROOT->ProcessLine(Form("clone_with_suffix((TH1*)%p,\"%s\");",hist,"_rbn"));
  hout->SetTitle(hist->GetTitle());
  hout->Reset();
  hout->SetBins(nbins,hist->GetXaxis()->GetXmin(),hist->GetXaxis()->GetXmax(),
		hist->GetNbinsY(),hist->GetYaxis()->GetXmin(),hist->GetYaxis()->GetXmax());
  
  double xmin   = hist->GetXaxis()->GetXmin();
  double xmax   = hist->GetXaxis()->GetXmax();
  int    xbins1 = hist->GetNbinsX();
  double dx1    = (xmax - xmin) / xbins1;
  int    xbins2 = nbins;
  double dx2    = (xmax - xmin) / xbins2;
  double ymin   = hist->GetYaxis()->GetXmin();
  double ymax   = hist->GetYaxis()->GetXmax();
  int    ybins  = hist->GetNbinsY();
  double dy     = (ymax - ymin) / ybins;
  
  double **con_arr1 = new double*[xbins1 + 2];
  double **con_arr2 = new double*[xbins2 + 2];
  double **err_arr1 = new double*[xbins1 + 2];
  double **err_arr2 = new double*[xbins2 + 2];
  for(int i = 0; i < xbins1 + 2; i++) {con_arr1[i] = new double[ybins + 2];}
  for(int i = 0; i < xbins2 + 2; i++) {con_arr2[i] = new double[ybins + 2];}
  for(int i = 0; i < xbins1 + 2; i++) {err_arr1[i] = new double[ybins + 2];}
  for(int i = 0; i < xbins2 + 2; i++) {err_arr2[i] = new double[ybins + 2];}
  
  for (int i=0; i <= xbins1 + 1; i++){
    for (int j=0; j <= ybins + 1; j++){
      con_arr1[i][j] = hist->GetBinContent(i,j);
      err_arr1[i][j] = hist->GetBinError(i,j);
    }
  }
  for (int i=0; i <= xbins2 + 1; i++){
    for (int j=0; j <= ybins + 1; j++){
      con_arr2[i][j] = 0.;
      err_arr2[i][j] = 0.;
    }
  }
  
  /* Rebin in the x axis */
  for (int j = 1; j <= ybins; j++) {
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
	con_arr2[i2][j] = con_arr2[i2][j] + fac * con_arr1[i1][j];
	err_arr2[i2][j] = sqrt(pow(err_arr2[i2][j],2) + pow(fac * err_arr1[i1][j],2));
	if (xup1 <= xup2) {
	  i2_cur = i2;
	  break;
	}
      }
    }
  }
  
  for (int i = 1; i <= xbins2; i++){
    for (int j = 1; j <= ybins; j++){
      hout->SetBinContent(i,j,con_arr2[i][j]);
      hout->SetBinError(i,j,err_arr2[i][j]);
    }
  }
  
  for(int i = 0; i < xbins1 + 2; i++) {delete [] con_arr1[i];}
  for(int i = 0; i < xbins2 + 2; i++) {delete [] con_arr2[i];}
  for(int i = 0; i < xbins1 + 2; i++) {delete [] err_arr1[i];}
  for(int i = 0; i < xbins2 + 2; i++) {delete [] err_arr2[i];}
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
