void ana_read2(const char * shm_name = "TEST"){
  TSocket* sock = (TSocket *) gROOT->GetListOfSockets()->FindObject("localhost");
  TMessage mess(kMESS_OBJECT);
  mess.Reset(kMESS_ANY);              // re-use TMessage object
  mess.WriteTString(TString(shm_name));
  sock->Send(mess);          // send message
  TMessage *mess2;
  sock->Recv(mess2);
  Int_t shm_result;
  mess2->ReadInt(shm_result);
  if(shm_result){
    Info("ana_read2","Shared memory read succeeded %d", shm_result);
  }else{
    Info("ana_read2","Shared memory read failed! Exit! %d", shm_result);
    return;
  }
  
  sock->Recv(mess2);
  Long64_t length;
  TString filename;
  mess2->ReadTString(filename);
  mess2->ReadLong64(length); // '*mess >> length;' is broken in CINT for Long64_t.
  
  Info("ana_read2","Receive input from client for %s", filename.Data());
  TMemFile* oldfile = (TMemFile*) gROOT->GetListOfFiles()->FindObject(filename.Data());
  if (oldfile) {
    /* Does not work in ROOT v6.
       if (gROOT->ProcessLine("gBrowserEx; 2> /dev/null")) { */
    if (gROOT->ProcessLine("gBrowserEx;")) {
      gROOT->ProcessLine("gBrowserEx->ResetActBrowser();");
    }
    gROOT->GetListOfFiles()->Remove(oldfile);
    delete oldfile;
  }
  TMemFile *transient = new TMemFile(filename,mess2->Buffer() + mess2->Length(),length);
  
  //mess2->SetBufferOffset(mess2->Length()+length);
  delete mess2; 
  transient->ls();
}
