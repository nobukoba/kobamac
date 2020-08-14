
void chk_run2214_20200813(){
  std::cout << "Macro: chk_run2214_2020813.C" << std::endl;
<<<<<<< HEAD
  TFile *f = TFile::Open("hist_MakeGRTree_Nobu_20200801_chk_q_nogrut.root");
  TTree *tr = (TTree *)f->Get("tr");
  tr->Print();

=======
  TFile *f = TFile::Open("/home/kobayash/mnt/GRUTinizer/output/date20200813/run2214/hist_MakeGRTree_Nobu_20200811_drftok_fin_q_nogrut.root");
  TTree *tr = (TTree *)f->Get("tr");
  tr->Print();
  
>>>>>>> 7a3e6067cb635cf0262e04f0045153139e8fb651
  return;
}
