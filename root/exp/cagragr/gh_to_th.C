#include <iostream>
#include "TROOT.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TString.h"
#include "TList.h"
#include "TKey.h"
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include "include/GH1D.h"
#include "include/GH2D.h"
#include "include/GH2I.h"

class GH2TH {
public:
  GH2TH(){;}
  ~GH2TH(){;}
  void CopyDir(TDirectory *source, TDirectory *target) {
    TKey *key;
    TIter nextkey(source->GetListOfKeys());
    while ((key = (TKey*)nextkey())) {
      const char *classname = key->GetClassName();
      TClass *cl = gROOT->GetClass(classname);
      TObject *obj = key->ReadObj();
      if (!cl) continue;
      if(target->FindObject(obj->GetName())){
	continue;
      }
      if (cl->InheritsFrom(TDirectory::Class())) {
	TDirectory * dir = target->mkdir(obj->GetName());
	CopyDir((TDirectory *)obj,dir);
      } else if (cl->InheritsFrom(TTree::Class())) {
	continue;
	//target->cd();
	//TTree *T = (TTree*)source->Get(key->GetName());
	//T->CloneTree(-1,"fast");
      } else if (cl->InheritsFrom(GH2I::Class())){
	obj->Copy(gh2i);
	gh2i.Copy(th2i);
	target->cd();
	th2i.Write();
      } else if (cl->InheritsFrom(GH2D::Class())){
	obj->Copy(gh2d);
	gh2d.Copy(th2d);
	target->cd();
	th2d.Write();
      } else if (cl->InheritsFrom(GH1D::Class())){
	obj->Copy(gh1d);
	th1d = gh1d;
	target->cd();
	th1d.Write();
      }
      // delete obj;
    }
  }
private:
  GH1D gh1d;
  TH1D th1d;
  GH2D gh2d;
  GH2I gh2i;
  TH2D th2d;
  TH2I th2i;
};

void gh_to_th(const char *filein, const char *fileout){
  TDirectory *source = TFile::Open(filein,"READ");
  TDirectory *target = TFile::Open(fileout,"RECREATE");
  std::cout << "Input file:  " << filein << std::endl;
  std::cout << "Output file: " << fileout << std::endl;
  GH2TH * gh2th = new GH2TH();
  gh2th->CopyDir(source, target);
  //delete gh2th;
}

void gh_to_th(const char *filein){
  TString str = filein;
  if(str.EndsWith(".root")){
    str.Resize(str.Length()-5);
  }
  str += "_th.root";
  gh_to_th(filein, str.Data());
}
