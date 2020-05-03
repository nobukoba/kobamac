void print_active_on_landscape_root_pdf() {
  gROOT->ProcessLine(Form(".L %s/root/ForActiveItems/print_active_on_root_pdf.C", gEnv->GetValue("KOBAMAC_DIR",".")));
  gROOT->ProcessLine("print_active_on_root_pdf(\"Landscape\")");
  return;
}
