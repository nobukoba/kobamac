void cal_1d_gui(){
  if (!gPad) {
    std::cout << "There is no gPad. This script is terminated." << std::endl;
    return;
  }
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (gBrowserEx == 0) {return;}
  TString str = gBrowserEx->OpenTGInputDialog("Peak energies (1172 1332 etc...)");
  if (str.EqualTo("")) {return;}
  
  std::istringstream iss(str.Data());
  std::vector <Double_t> pars;
  Double_t par;
  while (iss >> par) {
    pars.push_back(par);
  }
  if (pars.size() < 2) {
    std::cout << "The number of energies should be larger than 2. Exit."  << std::endl;
    return;
  }
  std::vector <Double_t> epars(pars.size(),0.);
  std::vector <Double_t> fit_pars(pars.size(),0.);
  std::vector <Double_t> fit_epars(pars.size(),0.);
  
  /* sorting of pars */
  /* std::sort (pars.begin(), pars.end()); */
  for (Int_t i = 0; i < pars.size(); i++) {
    Double_t min  = pars[i];
    Int_t    jmin = i;
    for (Int_t j = i; j < pars.size(); j++) {
      if (pars[j] < min) {
	min  = pars[j];
	jmin = j;
      }
    }
    /* swap pars[jmin] <--> pars[i] */
    pars[jmin]  = pars[i];
    pars[i]     = min;
  }
  
  TH1* hist = 0;
  TList *funclist = 0;
  for (Int_t i=0; i < pars.size();i++){
    Double_t x0, y0;
    WaitOneClickX *primi = new WaitOneClickX(&x0, &y0); delete primi;
    TList* listofpri = gPad->GetListOfPrimitives();
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
    
    y0 = hist->GetBinContent(hist->GetBin(x0));
    y1 = hist->GetBinContent(hist->GetBin(x1));
    if (y0 == 0.){y0 = 1.;}
    if (y1 == 0.){y1 = 1.;}
    
    Double_t bwid  = hist->GetBinWidth(1);
    Double_t xrang = x1 - x0;
    
    funclist = hist->GetListOfFunctions();
    Int_t j = 0;
    while(funclist->FindObject(Form("fit_eg_%d",j))){
      j++;
    }
    
    TF1* fgaus = new TF1(Form("fit_eg_init_gaus_%d",j), "gaus", x0+0.25*xrang, x1-0.25*xrang);
    Double_t ymax = hist->GetBinContent(hist->GetMaximumBin());
    fgaus->SetParLimits(0, 0., ymax*10000.);
    fgaus->SetParLimits(1, x0, x1);
    fgaus->SetParLimits(2, xrang/10., xrang);
    hist->Fit(fgaus,"R+");
    
    Double_t par[5];
    /* par[0] = (y0*x1-y1*x0)/(x1-x0);
       par[1] = (y1-y0)/(x1-x0);*/
    par[0] = (log(y0)*x1-log(y1)*x0)/(x1-x0);
    par[1] = (log(y1)-log(y0))/(x1-x0);
    par[2] = fgaus->GetParameter(0);
    par[3] = fgaus->GetParameter(1);
    par[4] = fgaus->GetParameter(2);
    funclist->Last()->Delete();

    TF1* fit_func = new TF1(Form("fit_eg_%d",j),"expo(0)+gaus(2)",x0,x1);
    fit_func->SetParameters(&(par[0]));
    fit_func->SetParName(0,"p0");
    fit_func->SetParName(1,"p1");
    fit_func->SetParName(2,"Constant");
    fit_func->SetParName(3,"Mean");
    fit_func->SetParName(4,"Sigma");
    fit_func->SetLineWidth(1);
    hist->Fit(fit_func,"R+");
    /*gPad->Update();
      gPad->Modified();*/
    fit_pars[i]  = fit_func->GetParameter(3);
    fit_epars[i] = fit_func->GetParError(3);
  }
  
  /* sorting of fit_pars */
  for (Int_t i = 0; i < fit_pars.size(); i++) {
    Double_t min  = fit_pars[i];
    Double_t emin = fit_epars[i];
    Int_t    jmin = i;
    for (Int_t j = i; j < fit_pars.size(); j++) {
      if (fit_pars[j] < min) {
	min  = fit_pars[j];
	emin = fit_epars[j];
	jmin = j;
      }
    }
    fit_pars[jmin]  = fit_pars[i];
    fit_epars[jmin] = fit_epars[i];
    fit_pars[i]     = min;
    fit_epars[i]    = emin;    
  }
  
  TGraphErrors * gr = new TGraphErrors(pars.size(),
				       &(pars[0]),  &(fit_pars[0]),
				       &(epars[0]), &(fit_epars[0]));
  TF1* cal_func = new TF1("cal_func","pol1",pars[0], pars[pars.size()-1]);
  gr->Fit("cal_func");
  /* gr->Draw("a*");
     gPad->Update();
     gPad->Modified(); */
  
  Double_t p0 = cal_func->GetParameter(0);
  Double_t p1 = cal_func->GetParameter(1);
  Double_t a  = 1./p1;
  Double_t b  = -p0/p1;
  std::cout << "Fit parameters of y = p0 + p1*x:" << std::endl;
  std::cout << "p0 = " << p0 << std::endl;
  std::cout << "p1 = " << p1 << std::endl;
  std::cout << "Parameters of inv. func. x = a*y + b:" << std::endl;
  std::cout << "a = " << a << std::endl;
  std::cout << "b = " << b << std::endl;
  std::cout << "Different expression for b a:"<< std::endl;
  std::cout << b << " " << a << std::endl;
  gr->Delete();
  cal_func->Delete();
  /*funclist->Delete();*/
  gROOT->ProcessLine(Form(".L %s/root/cui/cal_1d.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  gROOT->ProcessLine(Form("cal_1d((TH1*)%p,%f,%f)", hist,a,b));
  return;
}
