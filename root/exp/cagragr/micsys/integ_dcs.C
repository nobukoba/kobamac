TGraph *gr[2];

double pi = 3.141592653589793238;
double r2d = 180. / 3.141592653589793238;
double d2r = 3.141592653589793238 / 180.;

Double_t dcs_fuc(Double_t *x, Double_t *p){
  return gr[0]->Eval(x[0],0,"S");
  /*return gr[0]->Eval(x[0]);*/
}

Double_t dcs_fuc_2(Double_t *x, Double_t *p){
  double A = x[0];
  double B = x[1];
  double atan_th = sqrt(atan(A)*atan(A)+atan(B)*atan(B));
  double th_in_rad = tan(atan_th);
  double th_in_deg = th_in_rad * r2d;
  
  return gr[0]->Eval(th_in_deg,0,"S");
  
  /*return gr[0]->Eval(x[0]);*/
}

Double_t dcs_fuc_2_shift(Double_t *x, Double_t *p){
  double A = x[0] + 6.63 * d2r;
  double B = x[1];
  double atan_th = sqrt(atan(A)*atan(A)+atan(B)*atan(B));
  double th_in_rad = tan(atan_th);
  double th_in_deg = th_in_rad * r2d;
  
  return gr[0]->Eval(th_in_deg,0,"S");
  
  /*return gr[0]->Eval(x[0]);*/
}

void integ_dcs(){
  gr[0] = new TGraph("dcs_pZr90E80_num.dat");
  gr[1] = new TGraph("dcs_pZr94E80_num.dat");
  //gr[0]->Draw("a*");
  TF1 *f  = new TF1("f",dcs_fuc,0,180,0);
  TF2 *f2 = new TF2("f2",dcs_fuc_2,-pi/8., pi/8., -pi/8., pi/8.,0);
  TF2 *f2_shift = new TF2("f2_shift",dcs_fuc_2_shift,-pi/8., pi/8., -pi/8., pi/8.,0);
  //f2->SetNpx(400);
  //f2->SetNpy(400);
  //f2_shift->SetNpx(400);
  //f2_shift->SetNpy(400);
  //f2->Draw("col");
  //TLine *l = new TLine();
  //double theta_s_in_deg = 6.63;
  //double theta_s_in_rad = theta_s_in_deg * d2r;
  //l->SetLineColor(1);
  //l->SetLineWidth(1);
  //l->DrawLine(theta_s_in_rad-0.02, -0.03, theta_s_in_rad+0.02, -0.03);
  //l->DrawLine(theta_s_in_rad+0.02, -0.03, theta_s_in_rad+0.02,  0.03);
  //l->DrawLine(theta_s_in_rad+0.02,  0.03, theta_s_in_rad-0.02,  0.03);
  //l->DrawLine(theta_s_in_rad-0.02,  0.03, theta_s_in_rad-0.02, -0.03);
  //
  //l->DrawLine(theta_s_in_rad-0.02, -0.06, theta_s_in_rad+0.02, -0.06);
  //l->DrawLine(theta_s_in_rad+0.02, -0.06, theta_s_in_rad+0.02,  0.06);
  //l->DrawLine(theta_s_in_rad+0.02,  0.06, theta_s_in_rad-0.02,  0.06);
  //l->DrawLine(theta_s_in_rad-0.02,  0.06, theta_s_in_rad-0.02, -0.06);
  //
  //std::cout << "f2->Integral/2.4 mrad: " <<
  //  f2->Integral(theta_s_in_rad-0.02,theta_s_in_rad+0.02,-0.03, 0.03) / (0.04 * 0.06) << std::endl;
  //
  //std::cout << "f2->Integral/4.8 mrad: " <<
  //  f2->Integral(theta_s_in_rad-0.02,theta_s_in_rad+0.02,-0.05, 0.05) / (0.04 * 0.10) << std::endl;
  
  return;
}
