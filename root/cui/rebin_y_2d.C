void rebin_y_2d(TH2* hist, Int_t nbins){
  gROOT->ProcessLine(Form(".L %s/root/cui/clone_with_suffix.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  TH2 *hout = (TH2*)gROOT->ProcessLine(Form("clone_with_suffix((TH1*)%p,\"%s\");",hist,"_rbn"));
  hout->SetTitle(hist->GetTitle());
  hout->Reset();
  hout->SetBins(hist->GetNbinsX(),hist->GetXaxis()->GetXmin(),hist->GetXaxis()->GetXmax(),
		nbins,hist->GetYaxis()->GetXmin(),hist->GetYaxis()->GetXmax());
  
  double xmin   = hist->GetXaxis()->GetXmin();
  double xmax   = hist->GetXaxis()->GetXmax();
  int    xbins  = hist->GetNbinsX();
  double dx    = (xmax - xmin) / xbins;
  double ymin   = hist->GetYaxis()->GetXmin();
  double ymax   = hist->GetYaxis()->GetXmax();
  int    ybins1 = hist->GetNbinsY();
  double dy1    = (ymax - ymin) / ybins1;
  int    ybins2 = nbins;
  double dy2    = (ymax - ymin) / ybins2;
  
  double **con_arr1 = new double*[xbins + 2];
  double **con_arr2 = new double*[xbins + 2];
  double **err_arr1 = new double*[xbins + 2];
  double **err_arr2 = new double*[xbins + 2];
  for(int i = 0; i < xbins + 2; i++) {con_arr1[i] = new double[ybins1 + 2];}
  for(int i = 0; i < xbins + 2; i++) {con_arr2[i] = new double[ybins2 + 2];}
  for(int i = 0; i < xbins + 2; i++) {err_arr1[i] = new double[ybins1 + 2];}
  for(int i = 0; i < xbins + 2; i++) {err_arr2[i] = new double[ybins2 + 2];}
  
  for (int i=0; i <= xbins + 1; i++){
    for (int j=0; j <= ybins1 + 1; j++){
      con_arr1[i][j] = hist->GetBinContent(i,j);
      err_arr1[i][j] = hist->GetBinError(i,j);
    }
  }
  for (int i=0; i <= xbins + 1; i++){
    for (int j=0; j <= ybins2 + 1; j++){
      con_arr2[i][j] = 0.;
      err_arr2[i][j] = 0.;
    }
  }
  
  /* Rebin in the y axis */
  for (int i = 1; i <= xbins; i++){
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
	con_arr2[i][j2] = con_arr2[i][j2] + fac * con_arr1[i][j1];
	err_arr2[i][j2] = sqrt(pow(err_arr2[i][j2],2) + pow(fac * err_arr1[i][j1],2));
	if (yup1 <= yup2) {
	  j2_cur = j2;
	  break;
	}
      }
    }
  }
  
  for (int i = 1; i <= xbins; i++){
    for (int j = 1; j <= ybins2; j++){
      hout->SetBinContent(i,j,con_arr2[i][j]);
      hout->SetBinError(i,j,err_arr2[i][j]);
    }
  }
  
  for(int i = 0; i < xbins + 2; i++) {delete [] con_arr1[i];}
  for(int i = 0; i < xbins + 2; i++) {delete [] con_arr2[i];}
  for(int i = 0; i < xbins + 2; i++) {delete [] err_arr1[i];}
  for(int i = 0; i < xbins + 2; i++) {delete [] err_arr2[i];}
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
