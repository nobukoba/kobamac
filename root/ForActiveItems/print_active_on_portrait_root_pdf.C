void print_active_on_portrait_root_pdf() {
  gROOT->ProcessLine(Form(".L %s/ForActiveItems/print_active_on_root_pdf.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  gROOT->ProcessLine("print_active_on_root_pdf(\"Portrait\")");
  return;
}
