float e_masse = 0.511; /* MeV/c2 */
float p_masse = 938.3; /* MeV/c2 */
float d_masse = 1875.6; /* MeV/c2 */
float t_masse = 2809.4; /* MeV/c2 */
float mu_masse = 105.7; /* MeV/c2 */
float he_masse = 3726.0; /* MeV/c2 */
float x0_si = 9.36; /* longueur radiation Si - cm */
float x_si = 0.020; /* epaisseur silicium - cm */
float x0_sci = 42.4; /* longueur radiation polystyrene - cm */
float x_sci = 2*0.10; /* epaisseur scintillateur 2*1mm - cm */
float x_sci2 = 2*0.1375; /* epaisseur scintillateur 2*1.375 mm - cm */
float c = 300000000.; /* vitesse de lumiere (vide) m/s */
float tof_reso = 100*(1e-12); /* resolution en temps ps */
float tof_reso2 = 150*(1e-12); /* resolution en temps ps */
float tof_reso3 = 300*(1e-12); /* resolution en temps ps */
Double_t alpha = 1/137.035; 
Double_t NA = 6.022141 * TMath::Power(10,23); /* nomber d'Avogaadro (/mol) */
Double_t re = 2.817940; /* rayonne d'electron fm */ 
Double_t K_A = 0.307075; /* K/A MeV*cm/g pour A = 1 g/mol */
Double_t Z_Si = 14;
Double_t A_Si = 28.855; /* g/mol */
// Double_t I_Si = 8.1517; /* ev */
Double_t I_Si = 173; /* eV */
Double_t rho_Si = 2.33; /* g/cm3 */
Double_t Z_Cu = 29;
Double_t A_Cu = 63.546; /* g/mol */
// Double_t I_Cu = 7.7264; /* ev */
Double_t I_Cu = 322; /* eV */
Double_t rho_Cu = 8.96; /* g/cm3 */
Double_t rho_Lexan = 1.20; /* g/cm3 */
Double_t Z_lexan[3] = { 1, 6, 8};
Double_t A_lexan[3] = { 1, 12, 16};
Double_t wt_lexan[3] = { 0.055491, 0.755751, 0.188758};
Double_t I_Lexan = 73.1; /* eV */
Double_t rho_Photoemul = 3.815; /* g/cm3 */
Double_t Z_photoemul[8] = { 1, 6, 7, 8, 16, 35, 47, 53 };  
Double_t A_photoemul[8] = { 1, 12, 14, 16, 32, 79.9, 107.87, 126.9 };
Double_t wt_photoemul[8] = { 0.014100, 0.072261, 0.019320, 0.066101, 0.001890, 0.349103, 0.474105, 0.003120 }; 
Double_t I_Photoemul = 331; /* eV */

void bb_photoemul() {

  /* impulsion cinetique en MeV/c */
  Double_t kex[5000];
  float pex[2000];
  float ppx[2000];
  float pmux[2000];
  float yp_si[2000];
  float ye_si[2000];
  float ymu_si[2000];
  float yp_sci[2000];
  float ye_sci[2000];
  float yp_sci_1mm[2000];
  float ye_sci_1mm[2000];
  float yp_sci_1375mm[2000];
  float ye_sci_1375mm[2000];
  Double_t beta[5000];
  Double_t gamma[5000];
  Double_t ededx[5000];
  Double_t pdedx[5000];
  Double_t ddedx[5000];
  Double_t hededx[5000];
  Double_t mudedx[5000];
  Double_t betagamma[5000];
  float pmu[2000], mubeta;
  Double_t exp, pmom, energie;
  float epaisseur_si[3] = { 0.0065, 0.0200, 0.0300 };

  Double_t Z_A_Lexan = 0;
  for (int i=0; i<3; i++) {
    Z_A_Lexan += (wt_lexan[i]*Z_lexan[i]/A_lexan[i]);
  }

  Double_t Z_A_Photoemul = 0;
  for (int i=0; i<8; i++) {
    Z_A_Photoemul += (wt_photoemul[i]*Z_photoemul[i]/A_photoemul[i]);
  }
 
  printf("Z/A lexan %6.3lf photo emulsion %6.3lf \n",Z_A_Lexan,Z_A_Photoemul);
  //  return;
 
  Double_t Tmax = 0; 
  //  Double_t delta = 28.816*TMath::Sqrt(rho_Si_*(Z_Si/A_Si));
  Double_t hwp2 = 28.816*TMath::Sqrt(rho_Photoemul*(Z_A_Photoemul));
  //  Double_t hwp2 = 28.816*TMath::Sqrt(rho_Cu*(Z_Cu/A_Cu));
  //  Double_t Ne = NA*(Z_Si/A_Si)*rho_Si*TMath::Power(10,-39); /* 1 fm = 10^{-13} cm */
  printf("NA %6.4e \n",NA);
  Double_t Ne = NA*(Z_A_Photoemul)*rho_Photoemul*TMath::Power(10,-39); /* 1 fm = 10^{-13} cm */
  //  Double_t Ne = NA*(Z_Cu/A_Cu)*rho_Cu*TMath::Power(10,-39); /* 1 fm = 10^{-13} cm */
  printf("Ne %6.4e \n",Ne);
  Double_t hwp = (e_masse/alpha)*TMath::Sqrt(4*TMath::Pi()*Ne*TMath::Power(re,3));
  hwp *= TMath::Power(10,6); /* MeV -> eV */ 
  printf("hwp %6.3lf hwp2 %6.3lf\n",hwp,hwp2);
  Double_t delta_2 = 0;
  Double_t z = 1;

  int ii = 0;
  //  for (int i=10; i<5000; i+=15) {
  for (int i=10; i<40000; i+=15) {
    //  for (int i=0; i<10000000; i+=5000) {
    exp = (Double_t)i/1. + 1.0;
    exp/=10.;
    exp = TMath::Log10(exp);
    //    printf(" ii %d\n",ii);
    kex[ii] = TMath::Power(10.,exp);
    pmom = TMath::Sqrt(kex[ii]*kex[ii] + 2.*kex[ii]*p_masse);
    beta[ii] = pmom/(kex[ii]+p_masse);
    gamma[ii] = (kex[ii]+p_masse)/p_masse;
    betagamma[ii] = beta[ii]*gamma[ii];
    Tmax = 2*e_masse*beta[ii]*beta[ii]*gamma[ii]*gamma[ii];
    //    printf("tmax %6.3f\n",Tmax);
    Tmax /= (1 + (2*gamma[ii]*e_masse/p_masse)+((e_masse/p_masse)*(e_masse/p_masse)));
    //    delta_2 = delta + TMath::Log(beta[ii]*gamma[ii]);
    //    printf("delta %6.4f delta_2 %6.3f\n",delta,delta_2);
    pdedx[ii] = K_A*Z_A_Photoemul*((z*z)/(beta[ii]*beta[ii]));
    Double_t val = 2*e_masse*beta[ii]*beta[ii]*gamma[ii]*gamma[ii]*Tmax;
    val *= TMath::Power(10,12);
    Double_t delta = 0.5*TMath::Log(beta[ii]*gamma[ii]);
    delta_2 = TMath::Log(hwp/I_Photoemul) + TMath::Log(beta[ii]*gamma[ii]) - 0.5;
    if (beta[ii]*gamma[ii] < 100)
      pdedx[ii] *= (0.5*TMath::Log((val/(I_Photoemul*I_Photoemul))) - beta[ii]*beta[ii] - delta);
    else
      pdedx[ii] = pdedx[ii]*(0.5*TMath::Log(val/(I_Photoemul*I_Photoemul)) - beta[ii]*beta[ii] - delta_2);
    //      pdedx[ii] = pdedx[ii]*(0.5*TMath::Log(val/(I_Cu*I_Cu)) - beta[ii]*beta[ii] - delta_2);
    //    ye_si[ii] = ((13.6*sqrt(xlong))/(ebeta*pe[ii]))*(1.+(0.038*log(xlong)));
    //      printf("ii %d ke %6.2f pmom %6.2f beta %6.4f gamma %6.3f Tmax %6.3f \n",ii,kex[ii],pmom,beta[ii],gamma[ii],Tmax);
    /*    if (kex[ii] > 36 && kex[ii] < 38)  printf("  p ke %6.2f MeV dedx %6.2f MeV-g/cm2\n",kex[ii],pdedx[ii]);
    if (kex[ii] > 49 && kex[ii] < 52)  printf("  p ke %6.2f MeV dedx %6.2f MeV-g/cm2\n",kex[ii],pdedx[ii]);
    if (kex[ii] > 69 && kex[ii] < 71)  printf("  p ke %6.2f MeV dedx %6.2f MeV-g/cm2\n",kex[ii],pdedx[ii]);
    if (kex[ii] > 99 && kex[ii] < 101)  printf("  p ke %6.2f MeV dedx %6.2f MeV-g/cm2\n",kex[ii],pdedx[ii]);
    if (kex[ii] > 124 && kex[ii] < 126)  printf("  p ke %6.2f MeV dedx %6.2f MeV-g/cm2\n",kex[ii],pdedx[ii]);
    if (kex[ii] > 153 && kex[ii] < 155)  printf("  p ke %6.2f MeV dedx %6.2f MeV-g/cm2\n",kex[ii],pdedx[ii]);
    if (kex[ii] > 173 && kex[ii] < 175)  printf("  p ke %6.2f MeV dedx %6.2f MeV-g/cm2\n",kex[ii],pdedx[ii]);
    if (kex[ii] > 201 && kex[ii] < 203)  printf("  p ke %6.2f MeV dedx %6.2f MeV-g/cm2\n",kex[ii],pdedx[ii]);
    if (kex[ii] > 227 && kex[ii] < 229)  printf("  p ke %6.2f MeV dedx %6.2f MeV-g/cm2\n",kex[ii],pdedx[ii]); */
    ii++;
  }

  TGraph *g_pdedx_pe = new TGraph(ii,kex,pdedx);
  g_pdedx_pe->SetMarkerStyle(20);
  g_pdedx_pe->SetMarkerColor(kRed);
  g_pdedx_pe->SetMarkerSize(1.0);
  g_pdedx_pe->SetLineColor(kRed);
  g_pdedx_pe->SetLineWidth(2);

  TGraph *g_pdedx_bg_pe = new TGraph(ii,betagamma,pdedx);
  g_pdedx_bg_pe->SetMarkerStyle(20);
  g_pdedx_bg_pe->SetMarkerColor(kRed);
  g_pdedx_bg_pe->SetMarkerSize(1.0);
  g_pdedx_bg_pe->SetLineColor(kRed);
  g_pdedx_bg_pe->SetLineWidth(2);

  TGraph *g_p_bg_ce = new TGraph(ii,kex,betagamma);
  g_p_bg_ce->SetMarkerStyle(20);
  g_p_bg_ce->SetMarkerColor(kRed);
  g_p_bg_ce->SetMarkerSize(1.0);
  g_p_bg_ce->SetLineColor(kRed);
  g_p_bg_ce->SetLineWidth(2);

  ii = 0;
  Double_t n = 2;
  //  for (int i=10; i<5000; i+=15) {
  for (int i=10; i<40000; i+=15) {
    //  for (int i=0; i<10000000; i+=5000) {
    exp = (Double_t)i/1. + 1.0;
    exp/=10.;
    exp = TMath::Log10(exp);
    //    printf(" ii %d\n",ii);
    kex[ii] = TMath::Power(10.,exp);
    pmom = TMath::Sqrt((kex[ii]*n*kex[ii]*n) + (2.*kex[ii]*n*d_masse));
    beta[ii] = pmom/(kex[ii]*n+d_masse);
    gamma[ii] = (kex[ii]*n+d_masse)/d_masse;
    betagamma[ii] = beta[ii]*gamma[ii];
    Tmax = 2*e_masse*beta[ii]*beta[ii]*gamma[ii]*gamma[ii];
    //    printf("tmax %6.3f\n",Tmax);
    Tmax /= (1 + (2*gamma[ii]*e_masse/d_masse)+((e_masse/d_masse)*(e_masse/d_masse)));
    //    delta_2 = delta + TMath::Log(beta[ii]*gamma[ii]);
    //    printf("delta %6.4f delta_2 %6.3f\n",delta,delta_2);
    ddedx[ii] = K_A*Z_A_Photoemul*((z*z)/(beta[ii]*beta[ii]));
    Double_t val = 2*e_masse*beta[ii]*beta[ii]*gamma[ii]*gamma[ii]*Tmax;
    val *= TMath::Power(10,12);
    Double_t delta = 0.5*TMath::Log(beta[ii]*gamma[ii]);
    delta_2 = TMath::Log(hwp/I_Photoemul) + TMath::Log(beta[ii]*gamma[ii]) - 0.5;
    if (beta[ii]*gamma[ii] < 100)
      ddedx[ii] *= (0.5*TMath::Log((val/(I_Photoemul*I_Photoemul))) - beta[ii]*beta[ii] - delta);
    else
      ddedx[ii] = pdedx[ii]*(0.5*TMath::Log(val/(I_Photoemul*I_Photoemul)) - beta[ii]*beta[ii] - delta_2);
    //      pdedx[ii] = pdedx[ii]*(0.5*TMath::Log(val/(I_Cu*I_Cu)) - beta[ii]*beta[ii] - delta_2);
    //    ye_si[ii] = ((13.6*sqrt(xlong))/(ebeta*pe[ii]))*(1.+(0.038*log(xlong)));
    //      printf("ii %d ke %6.2f pmom %6.2f beta %6.4f gamma %6.3f Tmax %6.3f \n",ii,kex[ii],pmom,beta[ii],gamma[ii],Tmax);
    /*    if (kex[ii] > 36 && kex[ii] < 38)  printf("  p ke %6.2f MeV dedx %6.2f MeV-g/cm2\n",kex[ii],pdedx[ii]);
    if (kex[ii] > 49 && kex[ii] < 52)  printf("  p ke %6.2f MeV dedx %6.2f MeV-g/cm2\n",kex[ii],pdedx[ii]);
    if (kex[ii] > 69 && kex[ii] < 71)  printf("  p ke %6.2f MeV dedx %6.2f MeV-g/cm2\n",kex[ii],pdedx[ii]);
    if (kex[ii] > 99 && kex[ii] < 101)  printf("  p ke %6.2f MeV dedx %6.2f MeV-g/cm2\n",kex[ii],pdedx[ii]);
    if (kex[ii] > 124 && kex[ii] < 126)  printf("  p ke %6.2f MeV dedx %6.2f MeV-g/cm2\n",kex[ii],pdedx[ii]);
    if (kex[ii] > 153 && kex[ii] < 155)  printf("  p ke %6.2f MeV dedx %6.2f MeV-g/cm2\n",kex[ii],pdedx[ii]);
    if (kex[ii] > 173 && kex[ii] < 175)  printf("  p ke %6.2f MeV dedx %6.2f MeV-g/cm2\n",kex[ii],pdedx[ii]);
    if (kex[ii] > 201 && kex[ii] < 203)  printf("  p ke %6.2f MeV dedx %6.2f MeV-g/cm2\n",kex[ii],pdedx[ii]);
    if (kex[ii] > 227 && kex[ii] < 229)  printf("  p ke %6.2f MeV dedx %6.2f MeV-g/cm2\n",kex[ii],pdedx[ii]); */
    ii++;
  }

  TGraph *g_ddedx_pe = new TGraph(ii,kex,ddedx);
  g_ddedx_pe->SetMarkerStyle(20);
  g_ddedx_pe->SetMarkerColor(kBlue);
  g_ddedx_pe->SetMarkerSize(1.0);
  g_ddedx_pe->SetLineColor(kBlue);
  g_ddedx_pe->SetLineWidth(2);

  ii = 0;
  n = 4;
  z = 2;
  //  for (int i=10; i<5000; i+=15) {
  for (int i=10; i<40000; i+=15) {
    //  for (int i=0; i<10000000; i+=5000) {
    exp = (Double_t)i/1. + 1.0;
    exp/=10.;
    exp = TMath::Log10(exp);
    //    printf(" ii %d\n",ii);
    kex[ii] = TMath::Power(10.,exp);
    pmom = TMath::Sqrt((kex[ii]*n*kex[ii]*n) + (2.*kex[ii]*n*he_masse));
    beta[ii] = pmom/(kex[ii]*n+he_masse);
    gamma[ii] = (kex[ii]*n+he_masse)/he_masse;
    betagamma[ii] = beta[ii]*gamma[ii];
    Tmax = 2*e_masse*beta[ii]*beta[ii]*gamma[ii]*gamma[ii];
    //    printf("tmax %6.3f\n",Tmax);
    Tmax /= (1 + (2*gamma[ii]*e_masse/d_masse)+((e_masse/d_masse)*(e_masse/d_masse)));
    //    delta_2 = delta + TMath::Log(beta[ii]*gamma[ii]);
    //    printf("delta %6.4f delta_2 %6.3f\n",delta,delta_2);
    hededx[ii] = K_A*Z_A_Photoemul*((z*z)/(beta[ii]*beta[ii]));
    Double_t val = 2*e_masse*beta[ii]*beta[ii]*gamma[ii]*gamma[ii]*Tmax;
    val *= TMath::Power(10,12);
    Double_t delta = 0.5*TMath::Log(beta[ii]*gamma[ii]);
    delta_2 = TMath::Log(hwp/I_Photoemul) + TMath::Log(beta[ii]*gamma[ii]) - 0.5;
    if (beta[ii]*gamma[ii] < 100)
      hededx[ii] *= (0.5*TMath::Log((val/(I_Photoemul*I_Photoemul))) - beta[ii]*beta[ii] - delta);
    else
      hededx[ii] = pdedx[ii]*(0.5*TMath::Log(val/(I_Photoemul*I_Photoemul)) - beta[ii]*beta[ii] - delta_2);
    //      pdedx[ii] = pdedx[ii]*(0.5*TMath::Log(val/(I_Cu*I_Cu)) - beta[ii]*beta[ii] - delta_2);
    //    ye_si[ii] = ((13.6*sqrt(xlong))/(ebeta*pe[ii]))*(1.+(0.038*log(xlong)));
    //      printf("ii %d ke %6.2f pmom %6.2f beta %6.4f gamma %6.3f Tmax %6.3f \n",ii,kex[ii],pmom,beta[ii],gamma[ii],Tmax);
    /*    if (kex[ii] > 36 && kex[ii] < 38)  printf("  p ke %6.2f MeV dedx %6.2f MeV-g/cm2\n",kex[ii],pdedx[ii]);
    if (kex[ii] > 49 && kex[ii] < 52)  printf("  p ke %6.2f MeV dedx %6.2f MeV-g/cm2\n",kex[ii],pdedx[ii]);
    if (kex[ii] > 69 && kex[ii] < 71)  printf("  p ke %6.2f MeV dedx %6.2f MeV-g/cm2\n",kex[ii],pdedx[ii]);
    if (kex[ii] > 99 && kex[ii] < 101)  printf("  p ke %6.2f MeV dedx %6.2f MeV-g/cm2\n",kex[ii],pdedx[ii]);
    if (kex[ii] > 124 && kex[ii] < 126)  printf("  p ke %6.2f MeV dedx %6.2f MeV-g/cm2\n",kex[ii],pdedx[ii]);
    if (kex[ii] > 153 && kex[ii] < 155)  printf("  p ke %6.2f MeV dedx %6.2f MeV-g/cm2\n",kex[ii],pdedx[ii]);
    if (kex[ii] > 173 && kex[ii] < 175)  printf("  p ke %6.2f MeV dedx %6.2f MeV-g/cm2\n",kex[ii],pdedx[ii]);
    if (kex[ii] > 201 && kex[ii] < 203)  printf("  p ke %6.2f MeV dedx %6.2f MeV-g/cm2\n",kex[ii],pdedx[ii]);
    if (kex[ii] > 227 && kex[ii] < 229)  printf("  p ke %6.2f MeV dedx %6.2f MeV-g/cm2\n",kex[ii],pdedx[ii]); */
    ii++;
  }

  TGraph *g_hededx_pe = new TGraph(ii,kex,hededx);
  g_hededx_pe->SetMarkerStyle(20);
  g_hededx_pe->SetMarkerColor(kGreen+3);
  g_hededx_pe->SetMarkerSize(1.0);
  g_hededx_pe->SetLineColor(kGreen+3);
  g_hededx_pe->SetLineWidth(2);

  gStyle->SetHistLineColor(1);
  gStyle->SetPalette(1,0);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  gStyle->SetFrameFillColor(kWhite);

  Double_t xpt[10], ypt[10];

  xpt[0] = 4000;
  ypt[0] = 2.00;
  TGraph *g_crmuon = new TGraph(1,xpt,ypt);
  g_crmuon->SetMarkerStyle(20);
  g_crmuon->SetMarkerSize(1.0);
  g_crmuon->SetMarkerColor(kGreen+3);

  TH1F *hdedx = new TH1F("hdedx"," ",10,0.5,10000.);
  hdedx->GetYaxis()->SetLabelOffset(0.01);
  hdedx->GetYaxis()->SetLabelSize(0.045);
  hdedx->GetYaxis()->SetTitle("-dE/dx (MeV g^{-1}cm^{2})");
  hdedx->GetYaxis()->SetTitleSize(0.055);
  hdedx->GetYaxis()->SetTitleOffset(1.2);
  hdedx->GetYaxis()->CenterTitle();
  hdedx->GetXaxis()->SetLabelOffset(0.01);
  hdedx->GetXaxis()->SetLabelSize(0.045);
  hdedx->GetXaxis()->SetTitle("Kinetic Energy (MeV/n)");
  hdedx->GetXaxis()->SetTitleSize(0.055);
  hdedx->GetXaxis()->SetTitleOffset(1.2);
  hdedx->GetXaxis()->CenterTitle();
    //    hped[p]->GetXaxis()->SetNdivisions(505,1.);
  hdedx->SetMaximum(2000.);
  hdedx->SetMinimum(0.5);

  TLatex *tpdt = new TLatex(0.625,0.75,"p,t,d");
  tpdt->SetNDC();
  tpdt->SetTextSize(0.045);
  tpdt->SetTextColor(kBlue);

  TLatex *the = new TLatex(0.6225,0.685,"He");
  the->SetNDC();
  the->SetTextSize(0.045);
  the->SetTextColor(kGreen+3);

  TLatex *tpem = new TLatex(0.500,0.825,"Photo Emulsion");
  tpem->SetNDC();
  tpem->SetTextSize(0.05);

  c0 = new TCanvas( "c0", "c0", 0, 0, 600, 500 );
  //  c0->Size(3.,3.);
  //  c0->SetFixedAspectRatio(1);
  c0->SetFillColor(10);
  c0->SetBottomMargin(0.15);
  c0->SetLeftMargin(0.150);
  c0->SetRightMargin(0.1250);
  c0->SetLogy(1);
  c0->SetLogx(1);
  c0->SetTickx();
  c0->SetTicky();
  c0->SetGrid();
  hdedx->Draw();
  g_pdedx_pe->Draw("C");
  g_ddedx_pe->Draw("C");
  g_hededx_pe->Draw("C");
  tpdt->Draw();
  the->Draw();
  tpem->Draw();

  return;

  TH1F *hdedx_bg = new TH1F("hdedxbg"," ",10,0.01,10000.);
  hdedx_bg->GetYaxis()->SetLabelOffset(0.01);
  hdedx_bg->GetYaxis()->SetLabelSize(0.045);
  hdedx_bg->GetYaxis()->SetTitle("-dE/dx (MeV g^{-1}cm^{2})");
  hdedx_bg->GetYaxis()->SetTitleSize(0.055);
  hdedx_bg->GetYaxis()->SetTitleOffset(1.2);
  hdedx_bg->GetYaxis()->CenterTitle();
  hdedx_bg->GetXaxis()->SetLabelOffset(0.01);
  hdedx_bg->GetXaxis()->SetLabelSize(0.045);
  hdedx_bg->GetXaxis()->SetTitle("#beta#gamma");
  hdedx_bg->GetXaxis()->SetTitleSize(0.055);
  hdedx_bg->GetXaxis()->SetTitleOffset(1.2);
  hdedx_bg->GetXaxis()->CenterTitle();
    //    hped[p]->GetXaxis()->SetNdivisions(505,1.);
  hdedx_bg->SetMaximum(500.);
  hdedx_bg->SetMinimum(0.5);

  c1 = new TCanvas( "c1", "c1", 0, 0, 600, 500 );
  //  c0->Size(3.,3.);
  //  c0->SetFixedAspectRatio(1);
  c1->SetFillColor(10);
  c1->SetBottomMargin(0.15);
  c1->SetLeftMargin(0.150);
  c1->SetRightMargin(0.1250);
  c1->SetLogy(1);
  c1->SetLogx(1);
  c1->SetTickx();
  c1->SetTicky();
  c1->SetGrid();
  hdedx_bg->Draw();
  g_ededx_bg_si->Draw("C");
  g_pdedx_bg_si->Draw("C");
  g_mudedx_bg_si->Draw("C");
  tproton_d->Draw();
  tmuon_d->Draw();
  telectron_d->Draw();
  tsi->Draw();

  TH1F *h_ce_bg = new TH1F("hcebg"," ",10,0.5,10000.);
  h_ce_bg->GetYaxis()->SetLabelOffset(0.01);
  h_ce_bg->GetYaxis()->SetLabelSize(0.045);
  h_ce_bg->GetYaxis()->SetTitle("#beta#gamma");
  h_ce_bg->GetYaxis()->SetTitleSize(0.055);
  h_ce_bg->GetYaxis()->SetTitleOffset(1.2);
  h_ce_bg->GetYaxis()->CenterTitle();
  h_ce_bg->GetXaxis()->SetLabelOffset(0.01);
  h_ce_bg->GetXaxis()->SetLabelSize(0.045);
  h_ce_bg->GetXaxis()->SetTitle("Kinetic Energy (MeV)");
  h_ce_bg->GetXaxis()->SetTitleSize(0.055);
  h_ce_bg->GetXaxis()->SetTitleOffset(1.2);
  h_ce_bg->GetXaxis()->CenterTitle();
    //    hped[p]->GetXaxis()->SetNdivisions(505,1.);
  h_ce_bg->SetMaximum(10000.);
  h_ce_bg->SetMinimum(0.01);

  TLatex *tproton_g = new TLatex(0.250,0.70,"proton");
  tproton_g->SetNDC();
  tproton_g->SetTextSize(0.045);
  tproton_g->SetTextColor(kRed);

  TLatex *tmuon_g = new TLatex(0.250,0.75,"muon");
  tmuon_g->SetNDC();
  tmuon_g->SetTextSize(0.045);
  tmuon_g->SetTextColor(kGreen+3);

  TLatex *telectron_g = new TLatex(0.250,0.80,"electron");
  telectron_g->SetNDC();
  telectron_g->SetTextSize(0.045);
  telectron_g->SetTextColor(kBlue);

  c2 = new TCanvas( "c2", "c2", 0, 0, 600, 500 );
  //  c0->Size(3.,3.);
  //  c0->SetFixedAspectRatio(1);
  c2->SetFillColor(10);
  c2->SetBottomMargin(0.15);
  c2->SetLeftMargin(0.150);
  c2->SetRightMargin(0.1250);
  c2->SetLogy(1);
  c2->SetLogx(1);
  c2->SetTickx();
  c2->SetTicky();
  c2->SetGrid();
  h_ce_bg->Draw();
  g_e_bg_ce->Draw("C");
  g_p_bg_ce->Draw("C");
  g_mu_bg_ce->Draw("C");
  tproton_g->Draw();
  tmuon_g->Draw();
  telectron_g->Draw();
  
  return();

}

void bb_si() {

  /* impulsion cinetique en MeV/c */
  Double_t kex[5000];
  float pex[2000];
  float ppx[2000];
  float pmux[2000];
  float yp_si[2000];
  float ye_si[2000];
  float ymu_si[2000];
  float yp_sci[2000];
  float ye_sci[2000];
  float yp_sci_1mm[2000];
  float ye_sci_1mm[2000];
  float yp_sci_1375mm[2000];
  float ye_sci_1375mm[2000];
  Double_t beta[5000];
  Double_t gamma[5000];
  Double_t ededx[5000];
  Double_t pdedx[5000];
  Double_t mudedx[5000];
  Double_t betagamma[5000];
  float pmu[2000], mubeta;
  Double_t exp, pmom, energie;
  float epaisseur_si[3] = { 0.0065, 0.0200, 0.0300 };
 
  Double_t Tmax = 0; 
  //  Double_t delta = 28.816*TMath::Sqrt(rho_Si_*(Z_Si/A_Si));
  Double_t hwp2 = 28.816*TMath::Sqrt(rho_Si*(Z_Si/A_Si));
  //  Double_t hwp2 = 28.816*TMath::Sqrt(rho_Cu*(Z_Cu/A_Cu));
  //  Double_t Ne = NA*(Z_Si/A_Si)*rho_Si*TMath::Power(10,-39); /* 1 fm = 10^{-13} cm */
  printf("NA %6.4e \n",NA);
  Double_t Ne = NA*(Z_Si/A_Si)*rho_Si*TMath::Power(10,-39); /* 1 fm = 10^{-13} cm */
  //  Double_t Ne = NA*(Z_Cu/A_Cu)*rho_Cu*TMath::Power(10,-39); /* 1 fm = 10^{-13} cm */
  printf("Ne %6.4e \n",Ne);
  Double_t hwp = (e_masse/alpha)*TMath::Sqrt(4*TMath::Pi()*Ne*TMath::Power(re,3));
  hwp *= TMath::Power(10,6); /* MeV -> eV */ 
  printf("hwp %6.3lf hwp2 %6.3lf\n",hwp,hwp2);
  Double_t delta_2 = 0;
  Double_t z = 1;
  //  return;
  
  int ii = 0;
  for (int i=10; i<40000; i+=15) {
    //  for (int i=0; i<10000000; i+=5000) {
    exp = (Double_t)i/1. + 1.0;
    exp/=10.;
    exp = TMath::Log10(exp);
    //    printf(" ii %d\n",ii);
    kex[ii] = TMath::Power(10.,exp);
    pmom = sqrt(kex[ii]*kex[ii] + 2.*kex[ii]*e_masse);
    beta[ii] = pmom/(kex[ii]+e_masse);
    gamma[ii] = (kex[ii]+e_masse)/e_masse;
    betagamma[ii] = beta[ii]*gamma[ii];
    Tmax = e_masse*(gamma[ii]-1)/2;
    //    ededx[ii] = 0.5*(K_A/A_Cu)*Z_Cu*((z*z)/(beta[ii]*beta[ii]));
    ededx[ii] = 0.5*(K_A/A_Si)*Z_Si*((z*z)/(beta[ii]*beta[ii]));
    Double_t val = e_masse*beta[ii]*beta[ii]*gamma[ii]*gamma[ii]*Tmax;
    val *= TMath::Power(10,12);
    Double_t delta = TMath::Log(beta[ii]*gamma[ii]);
    //    ededx[ii] = ededx[ii]*(TMath::Log(val/(I_Cu*I_Cu)) + (1-beta[ii]*beta[ii]) 
    ededx[ii] = ededx[ii]*(TMath::Log(val/(I_Si*I_Si)) + (1-beta[ii]*beta[ii]) 
                - TMath::Log(2.)*(2*gamma[ii]-1)/(gamma[ii]*gamma[ii])
			   + (1/8)*TMath::Power(((gamma[ii]-1)/gamma[ii]),2));
    //    printf("ii %d e pom %6.2f MeV dedx %6.2f MeV-g/cm2\n",ii,pmom,ededx[ii]);
    ii++;
  }

  TGraph *g_ededx_si = new TGraph(ii,kex,ededx);
  g_ededx_si->SetMarkerStyle(20);
  g_ededx_si->SetMarkerColor(kBlue);
  g_ededx_si->SetMarkerSize(1.0);
  g_ededx_si->SetLineColor(kBlue);
  g_ededx_si->SetLineWidth(2);

  TGraph *g_ededx_bg_si = new TGraph(ii,betagamma,ededx);
  g_ededx_bg_si->SetMarkerStyle(20);
  g_ededx_bg_si->SetMarkerColor(kBlue);
  g_ededx_bg_si->SetMarkerSize(1.0);
  g_ededx_bg_si->SetLineColor(kBlue);
  g_ededx_bg_si->SetLineWidth(2);

  TGraph *g_e_bg_ce = new TGraph(ii,kex,betagamma);
  g_e_bg_ce->SetMarkerStyle(20);
  g_e_bg_ce->SetMarkerColor(kBlue);
  g_e_bg_ce->SetMarkerSize(1.0);
  g_e_bg_ce->SetLineColor(kBlue);
  g_e_bg_ce->SetLineWidth(2);

  int ii = 0;
  //  for (int i=10; i<5000; i+=15) {
  for (int i=10; i<40000; i+=15) {
    //  for (int i=0; i<10000000; i+=5000) {
    exp = (Double_t)i/1. + 1.0;
    exp/=10.;
    exp = TMath::Log10(exp);
    //    printf(" ii %d\n",ii);
    kex[ii] = TMath::Power(10.,exp);
    pmom = TMath::Sqrt(kex[ii]*kex[ii] + 2.*kex[ii]*p_masse);
    beta[ii] = pmom/(kex[ii]+p_masse);
    gamma[ii] = (kex[ii]+p_masse)/p_masse;
    betagamma[ii] = beta[ii]*gamma[ii];
    Tmax = 2*e_masse*beta[ii]*beta[ii]*gamma[ii]*gamma[ii];
    //    printf("tmax %6.3f\n",Tmax);
    Tmax /= (1 + (2*gamma[ii]*e_masse/p_masse)+((e_masse/p_masse)*(e_masse/p_masse)));
    //    delta_2 = delta + TMath::Log(beta[ii]*gamma[ii]);
    //    printf("delta %6.4f delta_2 %6.3f\n",delta,delta_2);
    pdedx[ii] = (K_A/A_Si)*Z_Si*((z*z)/(beta[ii]*beta[ii]));
    //    pdedx[ii] = (K_A/A_Cu)*Z_Cu*((z*z)/(beta[ii]*beta[ii]));
    Double_t val = 2*e_masse*beta[ii]*beta[ii]*gamma[ii]*gamma[ii]*Tmax;
    val *= TMath::Power(10,12);
    Double_t delta = 0.5*TMath::Log(beta[ii]*gamma[ii]);
    delta_2 = TMath::Log(hwp/I_Cu) + TMath::Log(beta[ii]*gamma[ii]) - 0.5;
    if (beta[ii]*gamma[ii] < 100)
      pdedx[ii] *= (0.5*TMath::Log((val/(I_Si*I_Si))) - beta[ii]*beta[ii] - delta);
    //      pdedx[ii] = pdedx[ii]*(0.5*TMath::Log(val/(I_Cu*I_Cu)) - beta[ii]*beta[ii] - delta);
    else
      pdedx[ii] = pdedx[ii]*(0.5*TMath::Log(val/(I_Si*I_Si)) - beta[ii]*beta[ii] - delta_2);
    //      pdedx[ii] = pdedx[ii]*(0.5*TMath::Log(val/(I_Cu*I_Cu)) - beta[ii]*beta[ii] - delta_2);
    //    ye_si[ii] = ((13.6*sqrt(xlong))/(ebeta*pe[ii]))*(1.+(0.038*log(xlong)));
    //      printf("ii %d ke %6.2f pmom %6.2f beta %6.4f gamma %6.3f Tmax %6.3f \n",ii,kex[ii],pmom,beta[ii],gamma[ii],Tmax);
    if (kex[ii] > 36 && kex[ii] < 38)  printf("  p ke %6.2f MeV dedx %6.2f MeV-g/cm2\n",kex[ii],pdedx[ii]);
    if (kex[ii] > 49 && kex[ii] < 52)  printf("  p ke %6.2f MeV dedx %6.2f MeV-g/cm2\n",kex[ii],pdedx[ii]);
    if (kex[ii] > 69 && kex[ii] < 71)  printf("  p ke %6.2f MeV dedx %6.2f MeV-g/cm2\n",kex[ii],pdedx[ii]);
    if (kex[ii] > 99 && kex[ii] < 101)  printf("  p ke %6.2f MeV dedx %6.2f MeV-g/cm2\n",kex[ii],pdedx[ii]);
    if (kex[ii] > 124 && kex[ii] < 126)  printf("  p ke %6.2f MeV dedx %6.2f MeV-g/cm2\n",kex[ii],pdedx[ii]);
    if (kex[ii] > 153 && kex[ii] < 155)  printf("  p ke %6.2f MeV dedx %6.2f MeV-g/cm2\n",kex[ii],pdedx[ii]);
    if (kex[ii] > 173 && kex[ii] < 175)  printf("  p ke %6.2f MeV dedx %6.2f MeV-g/cm2\n",kex[ii],pdedx[ii]);
    if (kex[ii] > 201 && kex[ii] < 203)  printf("  p ke %6.2f MeV dedx %6.2f MeV-g/cm2\n",kex[ii],pdedx[ii]);
    if (kex[ii] > 227 && kex[ii] < 229)  printf("  p ke %6.2f MeV dedx %6.2f MeV-g/cm2\n",kex[ii],pdedx[ii]);
    ii++;
  }

  TGraph *g_pdedx_si = new TGraph(ii,kex,pdedx);
  g_pdedx_si->SetMarkerStyle(20);
  g_pdedx_si->SetMarkerColor(kRed);
  g_pdedx_si->SetMarkerSize(1.0);
  g_pdedx_si->SetLineColor(kRed);
  g_pdedx_si->SetLineWidth(2);

  TGraph *g_pdedx_bg_si = new TGraph(ii,betagamma,pdedx);
  g_pdedx_bg_si->SetMarkerStyle(20);
  g_pdedx_bg_si->SetMarkerColor(kRed);
  g_pdedx_bg_si->SetMarkerSize(1.0);
  g_pdedx_bg_si->SetLineColor(kRed);
  g_pdedx_bg_si->SetLineWidth(2);

  TGraph *g_p_bg_ce = new TGraph(ii,kex,betagamma);
  g_p_bg_ce->SetMarkerStyle(20);
  g_p_bg_ce->SetMarkerColor(kRed);
  g_p_bg_ce->SetMarkerSize(1.0);
  g_p_bg_ce->SetLineColor(kRed);
  g_p_bg_ce->SetLineWidth(2);

  int ii = 0;
  //  for (int i=0; i<1000; i+=5) {
  //  for (int i=10; i<5000; i+=15) {
  for (int i=10; i<40000; i+=15) {
    //  for (int i=10; i<20000; i+=15) {
    //  for (int i=0; i<10000000; i+=5000) {
    exp = (float)i/1. + 1.0;
    exp/=10.;
    exp = log10(exp);
    //    printf(" ii %d\n",ii);
    kex[ii] = pow(10.,exp);
    pmom = sqrt(kex[ii]*kex[ii] + 2.*kex[ii]*mu_masse);
    Double_t ke2 = (pmom*pmom) + (mu_masse*mu_masse);
    ke2 = TMath::Sqrt(ke2) - mu_masse;
    //    printf(" kex %6.3lf ke2 %6.3lf \n",kex[ii],ke2);
    beta[ii] = pmom/(kex[ii]+mu_masse);
    gamma[ii] = (kex[ii]+mu_masse)/mu_masse;
    betagamma[ii] = beta[ii]*gamma[ii];
    Tmax = 2*e_masse*beta[ii]*beta[ii]*gamma[ii]*gamma[ii];
    Tmax /= (1 + (2*gamma[ii]*e_masse/mu_masse)+((e_masse/mu_masse)*(e_masse/mu_masse)));
    mudedx[ii] = (K_A/A_Si)*Z_Si*((z*z)/(beta[ii]*beta[ii]));
    //    mudedx[ii] = (K_A/A_Cu)*Z_Cu*((z*z)/(beta[ii]*beta[ii]));
    Double_t delta = 0.5*TMath::Log(beta[ii]*gamma[ii]);
    delta_2 = TMath::Log(hwp/I_Cu) + delta - 0.5;
    //    printf("hwp %6.3lf Icu %6.3lf\n",hwp,I_Cu);
    //    printf("delta_2 %6.3lf delta %6.3lf \n",delta_2,delta);
    //    printf("mudedx %6.3lf\n",mudedx[ii]);
    Double_t val = 2*e_masse*beta[ii]*beta[ii]*gamma[ii]*gamma[ii]*Tmax;
    //    printf("val %6.3e \n",val);
    val *= TMath::Power(10,12);
    //    printf("val %6.3e \n",val);
    if (beta[ii]*gamma[ii] < 100)
      mudedx[ii] = mudedx[ii]*(0.5*TMath::Log(val/(I_Si*I_Si)) - beta[ii]*beta[ii] - delta);
    //      mudedx[ii] = mudedx[ii]*(0.5*TMath::Log(val/(I_Cu*I_Cu)) - beta[ii]*beta[ii] - delta);
    else
      mudedx[ii] = mudedx[ii]*(0.5*TMath::Log(val/(I_Si*I_Si)) - beta[ii]*beta[ii] - delta_2);
    //      mudedx[ii] = mudedx[ii]*(0.5*TMath::Log(val/(I_Cu*I_Cu)) - beta[ii]*beta[ii] - delta_2);
    //    printf("sum %6.3e\n",(0.5*TMath::Log(val/(I_Cu*I_Cu)) - beta[ii]*beta[ii] - delta_2/2)); 
    //    printf("val2 %6.3e log %6.3e b2 %6.3e \n",val2,0.5*TMath::Log(val/(I_Cu*I_Cu)),beta[ii]*beta[ii]);
    //    ye_si[ii] = ((13.6*sqrt(xlong))/(ebeta*pe[ii]))*(1.+(0.038*log(xlong)));
    //    printf("ii %d ke %6.2lf pmom %6.2lf beta %6.4lf gamma %6.3lf Tmax %6.3lf \n",ii,kex[ii],pmom,beta[ii],gamma[ii],Tmax);
    //    printf("  mu pmom %6.2lf MeV dedx %6.2lf MeV-g/cm2\n",pmom,mudedx[ii]);
    ii++;
  }

  //  return;

  TGraph *g_mudedx_si = new TGraph(ii,kex,mudedx);
  g_mudedx_si->SetMarkerStyle(20);
  g_mudedx_si->SetMarkerColor(kGreen+2);
  g_mudedx_si->SetMarkerSize(1.0);
  g_mudedx_si->SetLineColor(kGreen+2);
  g_mudedx_si->SetLineWidth(2);

  TGraph *g_mudedx_bg_si = new TGraph(ii,betagamma,mudedx);
  g_mudedx_bg_si->SetMarkerStyle(20);
  g_mudedx_bg_si->SetMarkerColor(kGreen+2);
  g_mudedx_bg_si->SetMarkerSize(1.0);
  g_mudedx_bg_si->SetLineColor(kGreen+2);
  g_mudedx_bg_si->SetLineWidth(2);
  g_mudedx_bg_si->SetLineStyle(2);

  TGraph *g_mu_bg_ce = new TGraph(ii,kex,betagamma);
  g_mu_bg_ce->SetMarkerStyle(20);
  g_mu_bg_ce->SetMarkerColor(kGreen+2);
  g_mu_bg_ce->SetMarkerSize(1.0);
  g_mu_bg_ce->SetLineColor(kGreen+2);
  g_mu_bg_ce->SetLineWidth(2);

  //  return;

  gStyle->SetHistLineColor(1);
  gStyle->SetPalette(1,0);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  gStyle->SetFrameFillColor(kWhite);

  Double_t xpt[10], ypt[10];

  xpt[0] = 4000;
  ypt[0] = 2.00;
  TGraph *g_crmuon = new TGraph(1,xpt,ypt);
  g_crmuon->SetMarkerStyle(20);
  g_crmuon->SetMarkerSize(1.0);
  g_crmuon->SetMarkerColor(kGreen+3);

  TH1F *hdedx = new TH1F("hdedx"," ",10,0.5,10000.);
  hdedx->GetYaxis()->SetLabelOffset(0.01);
  hdedx->GetYaxis()->SetLabelSize(0.045);
  hdedx->GetYaxis()->SetTitle("-dE/dx (MeV g^{-1}cm^{2})");
  hdedx->GetYaxis()->SetTitleSize(0.055);
  hdedx->GetYaxis()->SetTitleOffset(1.2);
  hdedx->GetYaxis()->CenterTitle();
  hdedx->GetXaxis()->SetLabelOffset(0.01);
  hdedx->GetXaxis()->SetLabelSize(0.045);
  hdedx->GetXaxis()->SetTitle("Kinetic Energy (MeV)");
  hdedx->GetXaxis()->SetTitleSize(0.055);
  hdedx->GetXaxis()->SetTitleOffset(1.2);
  hdedx->GetXaxis()->CenterTitle();
    //    hped[p]->GetXaxis()->SetNdivisions(505,1.);
  hdedx->SetMaximum(500.);
  hdedx->SetMinimum(0.5);

  TLatex *tproton_d = new TLatex(0.625,0.75,"proton");
  tproton_d->SetNDC();
  tproton_d->SetTextSize(0.045);
  tproton_d->SetTextColor(kRed);

  TLatex *tmuon_d = new TLatex(0.625,0.70,"muon");
  tmuon_d->SetNDC();
  tmuon_d->SetTextSize(0.045);
  tmuon_d->SetTextColor(kGreen+3);

  TLatex *telectron_d = new TLatex(0.625,0.65,"electron");
  telectron_d->SetNDC();
  telectron_d->SetTextSize(0.045);
  telectron_d->SetTextColor(kBlue);

  TLatex *tsi = new TLatex(0.800,0.825,"Si");
  tsi->SetNDC();
  tsi->SetTextSize(0.05);

  c0 = new TCanvas( "c0", "c0", 0, 0, 600, 500 );
  //  c0->Size(3.,3.);
  //  c0->SetFixedAspectRatio(1);
  c0->SetFillColor(10);
  c0->SetBottomMargin(0.15);
  c0->SetLeftMargin(0.150);
  c0->SetRightMargin(0.1250);
  c0->SetLogy(1);
  c0->SetLogx(1);
  c0->SetTickx();
  c0->SetTicky();
  c0->SetGrid();
  hdedx->Draw();
  g_ededx_si->Draw("C");
  g_pdedx_si->Draw("C");
  g_mudedx_si->Draw("C");
  g_crmuon->Draw("P");
  tproton_d->Draw();
  tmuon_d->Draw();
  telectron_d->Draw();
  tsi->Draw();

  TH1F *hdedx_bg = new TH1F("hdedxbg"," ",10,0.01,10000.);
  hdedx_bg->GetYaxis()->SetLabelOffset(0.01);
  hdedx_bg->GetYaxis()->SetLabelSize(0.045);
  hdedx_bg->GetYaxis()->SetTitle("-dE/dx (MeV g^{-1}cm^{2})");
  hdedx_bg->GetYaxis()->SetTitleSize(0.055);
  hdedx_bg->GetYaxis()->SetTitleOffset(1.2);
  hdedx_bg->GetYaxis()->CenterTitle();
  hdedx_bg->GetXaxis()->SetLabelOffset(0.01);
  hdedx_bg->GetXaxis()->SetLabelSize(0.045);
  hdedx_bg->GetXaxis()->SetTitle("#beta#gamma");
  hdedx_bg->GetXaxis()->SetTitleSize(0.055);
  hdedx_bg->GetXaxis()->SetTitleOffset(1.2);
  hdedx_bg->GetXaxis()->CenterTitle();
    //    hped[p]->GetXaxis()->SetNdivisions(505,1.);
  hdedx_bg->SetMaximum(500.);
  hdedx_bg->SetMinimum(0.5);

  c1 = new TCanvas( "c1", "c1", 0, 0, 600, 500 );
  //  c0->Size(3.,3.);
  //  c0->SetFixedAspectRatio(1);
  c1->SetFillColor(10);
  c1->SetBottomMargin(0.15);
  c1->SetLeftMargin(0.150);
  c1->SetRightMargin(0.1250);
  c1->SetLogy(1);
  c1->SetLogx(1);
  c1->SetTickx();
  c1->SetTicky();
  c1->SetGrid();
  hdedx_bg->Draw();
  g_ededx_bg_si->Draw("C");
  g_pdedx_bg_si->Draw("C");
  g_mudedx_bg_si->Draw("C");
  tproton_d->Draw();
  tmuon_d->Draw();
  telectron_d->Draw();
  tsi->Draw();

  TH1F *h_ce_bg = new TH1F("hcebg"," ",10,0.5,10000.);
  h_ce_bg->GetYaxis()->SetLabelOffset(0.01);
  h_ce_bg->GetYaxis()->SetLabelSize(0.045);
  h_ce_bg->GetYaxis()->SetTitle("#beta#gamma");
  h_ce_bg->GetYaxis()->SetTitleSize(0.055);
  h_ce_bg->GetYaxis()->SetTitleOffset(1.2);
  h_ce_bg->GetYaxis()->CenterTitle();
  h_ce_bg->GetXaxis()->SetLabelOffset(0.01);
  h_ce_bg->GetXaxis()->SetLabelSize(0.045);
  h_ce_bg->GetXaxis()->SetTitle("Kinetic Energy (MeV)");
  h_ce_bg->GetXaxis()->SetTitleSize(0.055);
  h_ce_bg->GetXaxis()->SetTitleOffset(1.2);
  h_ce_bg->GetXaxis()->CenterTitle();
    //    hped[p]->GetXaxis()->SetNdivisions(505,1.);
  h_ce_bg->SetMaximum(10000.);
  h_ce_bg->SetMinimum(0.01);

  TLatex *tproton_g = new TLatex(0.250,0.70,"proton");
  tproton_g->SetNDC();
  tproton_g->SetTextSize(0.045);
  tproton_g->SetTextColor(kRed);

  TLatex *tmuon_g = new TLatex(0.250,0.75,"muon");
  tmuon_g->SetNDC();
  tmuon_g->SetTextSize(0.045);
  tmuon_g->SetTextColor(kGreen+3);

  TLatex *telectron_g = new TLatex(0.250,0.80,"electron");
  telectron_g->SetNDC();
  telectron_g->SetTextSize(0.045);
  telectron_g->SetTextColor(kBlue);

  c2 = new TCanvas( "c2", "c2", 0, 0, 600, 500 );
  //  c0->Size(3.,3.);
  //  c0->SetFixedAspectRatio(1);
  c2->SetFillColor(10);
  c2->SetBottomMargin(0.15);
  c2->SetLeftMargin(0.150);
  c2->SetRightMargin(0.1250);
  c2->SetLogy(1);
  c2->SetLogx(1);
  c2->SetTickx();
  c2->SetTicky();
  c2->SetGrid();
  h_ce_bg->Draw();
  g_e_bg_ce->Draw("C");
  g_p_bg_ce->Draw("C");
  g_mu_bg_ce->Draw("C");
  tproton_g->Draw();
  tmuon_g->Draw();
  telectron_g->Draw();
  
  return();

}
