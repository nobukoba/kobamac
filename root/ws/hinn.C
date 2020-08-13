void hinn() {
  std::cout << std::endl << "Macro: hinn.C" << std::endl;
  TBrowserEx *gBrowserEx = (TBrowserEx *)gROOT->ProcessLine("gBrowserEx;");
  if (gBrowserEx) {
    gSystem->cd((gBrowserEx->GetInitialWorkingDir()).Data());
  }else{return;}

  TString dir("."); 
  TGFileInfo fileinfo;
  char *filetypes[6];
  filetypes[0] = StrDup("hst files");
  filetypes[1] = StrDup("*.hst");
  filetypes[2] = StrDup("All files");
  filetypes[3] = StrDup("*");
  filetypes[4] = 0;
  filetypes[5] = 0;
  fileinfo.fFileTypes = (const char**)filetypes;
  fileinfo.fIniDir    = StrDup(dir);
  fileinfo.SetMultipleSelection(kTRUE);
  TGFileDialog* dialog = new TGFileDialog(gClient->GetRoot(),gClient->GetRoot(),kFDOpen,&fileinfo);
  if (fileinfo.fIniDir == 0) {
    std::cout << "The directory is null. Exit." << std::endl;
    return;
  }
  if ((!fileinfo.fFileNamesList)&&(!fileinfo.fFilename)) {
    std::cout << "Canceled. Exit." << std::endl;
    return;
  }
  TList *listptr;
  if (fileinfo.fFilename) {
    listptr = new TList();
    listptr->Add(new TObjString(fileinfo.fFilename));
  }else{
    listptr = (TList*)fileinfo.fFileNamesList;
  }
  Int_t nfiles = listptr->GetEntries();
  
  TIter next(listptr);
  TObjString *ostr;
  Int_t ifile = 0;
  gROOT->cd();
  while ((ostr = (TObjString*)next())){
    TString str = ostr->GetString();
    str.ReplaceAll("\"","");
    str.ReplaceAll(" ","");
    std::cout << std::endl;
    std::string filename = str.Data();
    std::cout << "Selected file: " << filename << std::endl;
    std::string basename = gSystem->BaseName(str);
    std::cout << "Base name: " << basename << std::endl;

    Int_t ncolumns = 0;
    Int_t nlines   = 0;
    std::string buf;
    TString buf_tstr;
    
    std::ifstream ifs(filename.c_str());
    if(ifs && getline(ifs,buf)){
      nlines++;
    }else{
      std::cout << "The file could not be read." << std::endl;
      continue;
    }
    buf_tstr = buf;
    buf_tstr.ReplaceAll(","," ");
    std::istringstream iss(buf_tstr.Data());
    Double_t tmp;
    while(iss >> tmp){
      ncolumns++;
    }
    while(ifs && getline(ifs, buf)){ /* getline is very slow in CINT!*/
      nlines++;
    }    

    TH1D **hpp = new TH1D*[ncolumns];
    for (Int_t icolumn = 0; icolumn < ncolumns; icolumn++) {
	TString str_n = "h1";
	Int_t num = 2;
	while (gROOT->Get(str_n.Data())) {
	  str_n = Form("h%d",num);
	  num++;
	}
      	hpp[icolumn] = new TH1D(str_n,basename.c_str(),nlines,0,nlines);
    }
    
    ifs.clear();
    ifs.seekg(0);
    for (Int_t iline = 0; iline < nlines; iline++) {
      getline(ifs,buf);
      buf_tstr = buf;
      buf_tstr.ReplaceAll(","," ");
      std::istringstream iss2(buf_tstr.Data());
      Double_t tmp2;
      for (Int_t icolumn = 0; icolumn < ncolumns; icolumn++) {
	iss2 >> tmp2;
	hpp[icolumn]->SetBinContent(iline+1,tmp2);
      }
    }
  }
  return;
}
