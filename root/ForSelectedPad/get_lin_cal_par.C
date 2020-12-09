void get_lin_cal_par(){
  if (!gPad) {
    std::cout << "There is no gPad. This script is terminated." << std::endl;
    return;
  }

  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (!gBrowserEx) {return;}
  TString str = gBrowserEx->OpenTGInputDialog("Please enter scale for all the histos\nFormat: %f %f [%f]",2);
  if (str.EqualTo("")){return;}
  std::istringstream iss(str.Data());
  Int_t npar = 0;
  Double_t par;
  while (iss >> par) {
    npar++;
  }
  Double_t *pars  = new double[npar];
  Double_t *cnt   = new double[npar];
  Double_t *cnte  = new double[npar];
  
  std::istringstream iss2(str.Data());
  for (Int_t i=0; i<npar; i++) {
    iss2 >> pars[i];
    std::cout << "i, pars[i]:" << i << ", " << pars[i] << std::endl;
  }
  std::cout << "Fit " << i << " peaks." << std::endl;
  std::cout << "Select region by 2 lines for a single peak." << std::endl;
  
  for (Int_t i=0; i<npar; i++) {
    Double_t x0, y0;
    WaitOneClickX *primi = new WaitOneClickX(&x0, &y0); delete primi;

    TList* listofpri = gPad->GetListOfPrimitives();
    TH1* hist = 0;
    TIter next(listofpri); TObject *obj;
    while ((obj = next())){
      if (obj->InheritsFrom("TH2")) {
	std::cout << "This script can not handle TH2 histograms." << std::endl;
	gPad->SetCrosshair(0);
	return;
      }
      if (obj->InheritsFrom("TH1")) {
	hist = (TH1*)obj;
	break;
      }
    }
    if (!hist) {
      std::cout << "TH1 histogram was not found in this pad." << std::endl;
      gPad->SetCrosshair(0);
      return;
    }
    TLine line;
    line.DrawLine(x0,hist->GetMinimum(),x0,hist->GetMaximum());
    Double_t x1, y1;
    primi = new WaitOneClickX(&x1, &y1); delete primi;
    line.DrawLine(x1,hist->GetMinimum(),x1,hist->GetMaximum());
    
    if (x0 > x1) {
      Double_t tmpx = x0;
      x0 = x1;
      x1 = tmpx;
    }
    
    y0 = hist->GetBinContent(hist->FindBin(x0));
    y1 = hist->GetBinContent(hist->FindBin(x1));
    if (y0 == 0.){y0 = 1.;}
    if (y1 == 0.){y1 = 1.;}
    
    Double_t bwid  = hist->GetBinWidth(1);
    Double_t xrang = x1 - x0;
    
    TList *funclist = hist->GetListOfFunctions();
    Int_t j = 0;
    while(funclist->FindObject(Form("fit_g_%d",j))){
      j++;
    }
    
    TF1* fgaus = new TF1(Form("fit_g_%d",j), "gaus", x0+0.25*xrang, x1-0.25*xrang);
    Double_t ymax = hist->GetBinContent(hist->GetMaximumBin());
    fgaus->SetParLimits(0, 0., ymax*10000.);
    fgaus->SetParLimits(1, x0, x1);
    fgaus->SetParLimits(2, xrang/10., xrang);
    hist->Fit(fgaus,"R+");
    cnt[i] = fgaus->GetParameter(1);
    cnte[i] = fgaus->GetParError(1);
  }
  
  for (Int_t i=0; i<npar; i++) {
    std::cout << "i, pars[i], cnt[i], cnte[i]:" << i << ", " << pars[i] << ", " << cnt[i] << ", " << cnte[i] << std::endl;
  }
  
  Double_t xy, x, x2, y, one;
  
  
  /* gROOT->ProcessLine(Form(".L %s/root/cui/scale.C", gEnv->GetValue("KOBAMAC_DIR","."))); */
  /* gROOT->ProcessLine(Form("scale((TH1*)%p,%f)", hist,par)); */
  
}
