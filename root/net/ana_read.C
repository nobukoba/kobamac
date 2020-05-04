void ana_read(){
  TSocket* sock = (TSocket *) gROOT->GetListOfSockets()->FindObject("localhost");
  TMessage mess(kMESS_OBJECT);
  mess.Reset(kMESS_ANY);              // re-use TMessage object
  sock->Send(mess);          // send message
  TMessage *mess2;
  sock->Recv(mess2);
  Long64_t length;
  TString filename;
  Int_t clientId;
  mess2->ReadInt(clientId);
  mess2->ReadTString(filename);
  mess2->ReadLong64(length); // '*mess >> length;' is broken in CINT for Long64_t.
  
  Info("fastMergeServer","Receive input from client %d for %s",clientId,filename.Data());
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
