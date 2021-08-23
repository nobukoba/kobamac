void GotoNode(TXMLEngine &xml, XMLNodePointer_t node, Int_t level,
	      TString baseurl, TString fulldir, TDirectoryFile* curdir) {
  TDirectoryFile* newdir = 0;
  TString attr_kind = xml.GetAttr(node,"_kind");
  TString attr_name = xml.GetAttr(node,"_name");
  if (attr_kind.BeginsWith("ROOT.TH")) {
    TString cmd = "";
    if (level == 1) {
      cmd = "curl -s " + baseurl + "/root.json";
    }else{
      cmd = "curl -s " + baseurl + "/" + fulldir + "/" + attr_name + "/root.json";
    }
    cmd.ReplaceAll(";","\\;");
    cmd.ReplaceAll("(","\\(");
    cmd.ReplaceAll(")","\\)");
    std::cout << cmd << std::endl;
    TString result = gSystem->GetFromPipe(cmd.Data());
    TObject *obj = 0;
    curdir->cd();
    TBufferJSON::FromJSON(obj, result);
    if (obj) obj->Clone();
  }else{
    if ( (level == 1)&& baseurl.EndsWith("/")) {
      newdir = curdir;
    }else{
      curdir->cd();
      newdir = new TDirectoryFile(attr_name.Data(),attr_name.Data(),"",curdir);
    }
    if (level > 1){
      fulldir += attr_name + "/";
    }
    XMLNodePointer_t child = xml.GetChild(node);
    while (child != 0) {
      GotoNode(xml, child, level + 1, baseurl, fulldir, newdir);
      child = xml.GetNext(child);
    }
  }
  return;
}

void thttpserver_loader(TString baseurl = "http://oxygen.rcnp.osaka-u.ac.jp:8083/") {
  /* First create engine */
  TXMLEngine xml;
  
  /* Now try to parse xml file
     Only file with restricted xml syntax are supported */
  TString cmd = "curl -s " + baseurl + "/h.xml";
  cmd.ReplaceAll(";","\\;");
  cmd.ReplaceAll("(","\\(");
  cmd.ReplaceAll(")","\\)");
  TString result = gSystem->GetFromPipe(cmd.Data());
  XMLDocPointer_t xmldoc = xml.ParseString(result);
  if (!xmldoc) return 0;
  
  /* take access to main node */
  XMLNodePointer_t mainnode = xml.DocGetRootElement(xmldoc);
  XMLNodePointer_t child = xml.GetChild(mainnode);
  
  /* Go to recursively all nodes and subnodes */
  TDirectoryFile* curdir = (TDirectoryFile*)gROOT;
  TString fulldir = "";
  GotoNode(xml, child, 1, baseurl, fulldir, curdir);
  
  /* Release memory before exit */
  xml.FreeDoc(xmldoc);
  return 0;
}
