void mkhist(){
  std::cout << "mkhist" << std::endl;
  std::ifstream ifs("mca_file.txt");
  std::string buf;
  Int_t iline = 0;
  TFile *f = TFile::Open("mca_file.root","recreate");
  while(ifs && getline(ifs, buf)){ /* getline is very slow in CINT!*/
    std::cout << iline << ": "<< buf << std::endl;
    iline++;
  }
  
  return;
}
