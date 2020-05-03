void s2root_client(const char* jobname = "job1", Long64_t maxcnt = 0) {
   Int_t status, version, kind;
   TSocket *sock = new TSocket("localhost", 9090);
   //TSocket *sock = new TSocket("miho-1.rcnp.osaka-u.ac.jp", 9090);
   //TSocket *sock = new TSocket("koba-desktop.rcnp.osaka-u.ac.jp", 9090);
   if (!sock->IsValid()) {
      Error("treeClient","Could not establish a connection with the server %s:%d.","localhost",9090);
      return;
   }

   // Wait till we get the start message
   // server tells us who we are
   sock->Recv(status, kind);
   if (kind != 0 /* kStartConnection */)
   {
      Error("treeClient","Unexpected server message: kind=%d status=%d\n",kind,status);
      delete sock;
      return;
   }
   sock->Recv(version, kind);
   if (kind != 1 /* kStartConnection */)
   {
      Fatal("treeClient","Unexpected server message: kind=%d status=%d\n",kind,status);
   } else {
      Info("treeClient","Connected to fastMergeServer version %d\n",version);
   }
   TMemFile *transient = 0;
   sock->Send("test");
   TMessage *mess2;
   sock->Recv(mess2);
   
   Long64_t length;
   TString filename;
   Int_t clientId;
   mess2->ReadInt(clientId);
   mess2->ReadTString(filename);
   mess2->ReadLong64(length); // '*mess >> length;' is broken in CINT for Long64_t.
     
   Info("fastMergeServer","Receive input from client %d for %s",clientId,filename.Data());
   transient = new TMemFile(filename,mess2->Buffer() + mess2->Length(),length);
   mess2->SetBufferOffset(mess2->Length()+length);
   transient->ls();
   sock->Send("Finished");          // tell server we are finished
   // Close the socket
   sock->Close();
}
