void ana_disconnect(){
  TSocket* sock = (TSocket *) gROOT->GetListOfSockets()->FindObject("localhost");
  sock->Send("Finished");          // tell server we are finished

  // Close the socket
  sock->Close();
  Info("ana_disconnect","Connection was terminated\n");
}
