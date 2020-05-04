void fit_gaus_run_by_run_h41(){
  std::cout << std::endl << "Macro: fit_gaus_run_by_run.C "<< std::endl;
  TNamed *named = (TNamed*)gROOT->FindObjectAny("initial_working_dir");
  if (named) {gSystem->cd(named->GetTitle());}
  std::cout << "gSystem->pwd(): " << gSystem->pwd() << std::endl;
  
  TFile *h_newfile = TFile::Open("run_by_run_parameter_dep.root");
  TList *file_list = h_newfile->GetListOfKeys();
  std::cout << file_list << std::endl;
  std::cout << file_list->GetName()<< std::endl;
  TIter next(file_list);
  TObject *key;
  TObject *obj;
  
  
  while (key = next()) {
    obj = ((TKey*)key)->ReadObj();
    if (!(obj->InheritsFrom("TH1"))) {
      continue;
    }
    std::cout << obj->GetName()<<std::endl;
    
    ((TH1*)obj)->GetXaxis()->SetRangeUser(2500,2750);
    
    
    obj->Draw();
    gPad->Update();
    gPad->Modified();
  }
  
//    Int_t nbins = hist->GetNbinsX();
//    Int_t ndig = 1;
//    while (nbins > (TMath::Power(10, ndig)-1)) {
//      ndig++;
//    }
//    for (Int_t i = 1; i <= hist->GetNbinsX(); i++) {
//      TString formatstr = Form("%s%%0%dd",str_n.Data(),ndig);
//      TString hname = Form(formatstr.Data(),i);
//      TH1D *hout = new TH1D(hname, hist->GetTitle(), hist->GetNbinsY(),
//			    hist->GetYaxis()->GetXmin(), hist->GetYaxis()->GetXmax());
//      for (Int_t j = 0; j <= hist->GetNbinsY()+1; j++) {
//	hout->Fill(hist->GetYaxis()->GetBinCenter(j),
//		   hist->GetBinContent(i,j));
//	
//	
//      }
//      Double_t maxbin = hout->GetMaximumBin();
//      Double_t maxcen = hout->GetBinCenter(maxbin);
//      Double_t bwid   = hout->GetBinWidth(maxbin);
//      Double_t xlow   = maxcen - 5*bwid;
//      Double_t xup    = maxcen + 5*bwid;
//      Double_t xrang  = xup - xlow;
//      TF1* fgaus = new TF1("fit_narrow_gaus_init_0", "gaus", xlow, xup);
//      Double_t ymax = hout->GetBinContent(hout->GetMaximumBin());
//      if (ymax == 0.) {
//	std::cout << hout->GetName() << ": ymax == 0. skip."<< std::endl;
//	continue;
//      }
//      fgaus->SetParLimits(0, 0., ymax*10000.);
//      fgaus->SetParLimits(1, xlow, xup);
//      fgaus->SetParLimits(2, xrang/10., xrang);
//      hout->Fit(fgaus,"R+");
//      
//      Double_t par[3];
//      par[0] = fgaus->GetParameter(0);
//      par[1] = fgaus->GetParameter(1);
//      par[2] = fgaus->GetParameter(2);
//      TList *funclist = hout->GetListOfFunctions();
//      funclist->Last()->Delete();
//      xlow = par[1] - 0.75 * par[2];
//      xup  = par[1] + 0.75 * par[2];
//      TF1* fit_narrow_gaus = new TF1("fit_narrow_gaus_0","gaus",xlow,xup);
//      fit_narrow_gaus->SetLineWidth(1);
//      hout->Fit(fit_narrow_gaus,"R+");
//      hout->Draw();
//      gPad->Update();
//      gPad->Modified();
//    }
//    h_newfile->Write();
//    h_newfile->Close();
//  }
//  return;
}
