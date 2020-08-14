void chk_run2214_20200813(){
  std::cout << "Macro: chk_run2214_2020813.C" << std::endl;
  TFile *f = TFile::Open("hist_MakeGRTree_Nobu_20200801_chk_q_nogrut.root");
  TTree *tr = (TTree *)f->Get("tr");
  tr->Print();

  return;
}
