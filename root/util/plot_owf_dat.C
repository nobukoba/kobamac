void plot_owf_dat() {
  std::cout << std::endl << "Macro: plot_owf_dat.C" << std::endl;
  std::ifstream ifs("");
    std::string buf;
    Int_t iline = 0;
    Int_t norbit = 0;
    getline(ifs, buf);
    std::istringstream iss(buf);
    iss >> norbit;
    
//    while(ifs && ){ /* getline is very slow in CINT!*/
//      iline++;
//      if (iline == 1) {
//	std::istringstream iss(buf);
//	iss >> val[0] >> val[1] >> val[2] >> val[3] >> val[4];
//
//	
//      }
//      
//    }  
}
