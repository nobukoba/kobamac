#include <iostream>
#include <cstdio>

#include "TROOT.h"
#include "TSystem.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TROOT.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TGFileBrowser.h"
#include "TGListTree.h"
#include "TList.h"
#include "TKey.h"
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"

#include "GH1D.h"
#include "GH2Base.h"
#include "GH2D.h"
#include "GH2I.h"

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
    } else if (cl->InheritsFrom(TH1::Class())){
      target->cd();
      obj->Clone(obj->GetName());
    }
    delete obj;
  }
}

void MergeDir(TDirectory *source, TDirectory *target) {
  TKey *key;
  TIter nextkey(source->GetListOfKeys());
  while ((key = (TKey*)nextkey())) {
    if(key->GetCycle() > 1){continue;}
    TObject *tgt = target->FindObject(key->GetName());
    if (!tgt) {
      std::cout << "key: " << key->GetName()
		<< " was not found in the directory: "
		<< target->GetName()
		<< std::endl;
      continue;
    }
    const char *classname = key->GetClassName();
    TClass *cl = gROOT->GetClass(classname);
    const char *tgtclassname = tgt->ClassName();
    TClass *tgtcl = gROOT->GetClass(tgtclassname);
    if (cl != tgtcl) {
      std::cout << "source class: " << classname
		<< " is different from target class: "
		<< tgtclassname
		<< std::endl;
      continue;
    }
    TObject *obj = key->ReadObj();
    if (!cl) continue;
    if (cl->InheritsFrom(TDirectory::Class())) {
      MergeDir((TDirectory *)obj,(TDirectory *)tgt);
    } else if (cl->InheritsFrom(TTree::Class())) {
      continue;
      //TTree *T = (TTree*)source->Get(tgt->GetName());
      //T->Add((TTree*)obj);
    } else if (cl->InheritsFrom(TH2::Class())){
      ((TH2*)tgt)->Add((TH2*)obj);
    } else if (cl->InheritsFrom(TH1::Class())){
      ((TH1*)tgt)->Add((TH1*)obj);
    }
    delete obj;
  }
}

int main(int argc, char **argv){
  if (argc < 3) {
    printf("******Error in gadd\n");
    printf("===>  gadd target source1 [source_2] ...\n");
    return 1;
  }
  TString target = argv[1];
  TList list_of_active_dir;
  for (Int_t i = 2; i < argc; i++) {
    TDirectory* file = TFile::Open(argv[i],"read");
    if (!file) {
      std::cout << "Exit."  << std::endl;
      return 1;
    }
    list_of_active_dir.Add((TObject*)file);
  }
  
  TDirectory *mergedroot = TFile::Open(argv[1],"recreate");
  TDirectory *first_dir = (TDirectory *)(list_of_active_dir.At(0));
  std::cout << std::endl << "Merge start." << std::endl;
  std::cout << "Base dir: " << first_dir->GetName() << std::endl;
  CopyDir(first_dir, mergedroot);
  
  Int_t num_entries = list_of_active_dir.GetEntries();
  for (Int_t i = 1; i < num_entries; i++) {
    std::cout << i << "-th dir: " << list_of_active_dir.At(i)->GetName() << std::endl; 
    MergeDir((TDirectory *)(list_of_active_dir.At(i)), mergedroot);
  }
  mergedroot->Write();
  mergedroot->Close();
  std::cout << "Mearge finished." << std::endl;
  return 0;
}
