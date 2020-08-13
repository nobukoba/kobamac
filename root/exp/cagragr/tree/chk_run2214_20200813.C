
void chk_run2214_20200813(){
  std::cout << "Macro: chk_run2214_2020813.C" << std::endl;
  TFile *f = TFile::Open("/home/kobayash/mnt/GRUTinizer/output/date20200813/run2214/hist_MakeGRTree_Nobu_20200811_drftok_fin_q_nogrut.root");
  TTree *tr = (TTree *)f->Get("tr");
  tr->Print();
  
  return;
}
