void ana_connect(){
  Int_t status, kind, version;
  TSocket *sock = new TSocket("localhost", 9090);
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
}
