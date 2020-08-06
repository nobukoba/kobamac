TGraph *gr[2];

double pi = 3.141592653589793238;
double r2d = 180. / 3.141592653589793238;
double d2r = 3.141592653589793238 / 180.;
double theta_s_in_deg = 6.63;
double theta_s_in_rad = theta_s_in_deg * d2r;

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

Double_t dcs_fuc_2_cut(Double_t *x, Double_t *p){
  double A = x[0];
  double B = x[1];
  double atan_th = sqrt(atan(A)*atan(A)+atan(B)*atan(B));
  double th_in_rad = tan(atan_th);
  double th_in_deg = th_in_rad * r2d;
  double ret = gr[0]->Eval(th_in_deg,0,"S");

  if ((B > 0.060)||
      (B < -0.060)||
      (A < theta_s_in_rad-0.020)||
      (A > theta_s_in_rad+0.020)||
      (B > -9./4.*(A- theta_s_in_rad)+0.075)||    
      (B >  9./4.*(A- theta_s_in_rad)+0.075)||
      (B <  9./4.*(A- theta_s_in_rad)-0.075)||
      (B < -9./4.*(A- theta_s_in_rad)-0.075)
      ) {
    ret = 0.;
  }
  return ret;
  
  /*return gr[0]->Eval(x[0]);*/
}

void integ_dcs(){
  gr[0] = new TGraph("dcs_pZr90E80_num.dat");
  gr[1] = new TGraph("dcs_pZr94E80_num.dat");
  //gr[0]->Draw("a*");
  TF1 *f  = new TF1("f",dcs_fuc,0,180,0);
  TF2 *f2 = new TF2("f2",dcs_fuc_2,0., pi/16., -pi/16., pi/16.,0);
  TF2 *f2_cut = new TF2("f2_cut",dcs_fuc_2_cut,0., pi/16., -pi/16., pi/16.,0);
  f2->SetNpx(400);
  f2->SetNpy(400);
  f2_cut->SetNpx(400);
  f2_cut->SetNpy(400);
  f2->Draw("col");
  TLine *l = new TLine();
  l->SetLineColor(1);
  l->SetLineWidth(1);
  l->DrawLine(theta_s_in_rad-0.02, -0.03, theta_s_in_rad+0.02, -0.03);
  l->DrawLine(theta_s_in_rad+0.02, -0.03, theta_s_in_rad+0.02,  0.03);
  l->DrawLine(theta_s_in_rad+0.02,  0.03, theta_s_in_rad-0.02,  0.03);
  l->DrawLine(theta_s_in_rad-0.02,  0.03, theta_s_in_rad-0.02, -0.03);

  double x1 = theta_s_in_rad-0.02;
  double x2 = theta_s_in_rad+0.02;
  double y1 = -0.06;
  double y2 = 0.06;
  
  //l->DrawLine(x1,0., x1, 9./4.*(x1- theta_s_in_rad)+0.075);
  //l->DrawLine(x1, 0.03,-0.004+theta_s_in_rad,0.06);
  //l->DrawLine(-0.004+theta_s_in_rad,0.06,0.004+theta_s_in_rad,0.06);
  //l->DrawLine(0.004+theta_s_in_rad,0.06,x2,0.03);
  l->DrawLine(theta_s_in_rad-0.02, -0.06, theta_s_in_rad+0.02,  -0.06);
  l->DrawLine(theta_s_in_rad+0.02, -0.06, theta_s_in_rad+0.02,  0.06);
  l->DrawLine(theta_s_in_rad+0.02,  0.06, theta_s_in_rad-0.02,  0.06);
  l->DrawLine(theta_s_in_rad-0.02,  0.06, theta_s_in_rad-0.02, -0.06);
  
//
//4 * 0.06) << std::endl;
//
//
//4 * 0.10) << std::endl;
  
  return;
}
