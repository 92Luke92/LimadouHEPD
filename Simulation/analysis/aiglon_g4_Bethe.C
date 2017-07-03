// BetaGamma := x[0]
extern double const p2ch_ADCconv;   // layer1
extern double const p3ch_ADCconv;   // layer2
extern double const n2ch_ADCconv;   // layer1
extern double const n3ch_ADCconv;   // layer2

extern double const layerWidth;
extern double const mProton;

// Fundamental conversion formulae:
Double_t BetaGamma(double Ekin, double Mass) 
{
    /*
    It defines the relationship between the BetaGamma 
    and the particle kinetic energy (Ekin) and mass (Mass)
    */

    double frac_Ekin_Mass = Ekin/Mass;
    return TMath::Sqrt(frac_Ekin_Mass * (frac_Ekin_Mass + 2));
}

Double_t gamma_v(Double_t *x) 
{
    /*
    It is a function that is defined inside the Bethe-Bloch equation
    */

    return TMath::Sqrt(x[0]*x[0] + 1);
}

Double_t inv_Beta2(Double_t *x) 
{
    /*
    It is a function that is defined inside the Bethe-Bloch equation
    */

    return ((x[0]*x[0] + 1) / (x[0]*x[0]));
}

Double_t delta_BetaGamma(Double_t *x, double x_0, double x_1, double Cbar, double a, double k, double delta_0) 
{
    /*
    It is a function that is defined inside the Bethe-Bloch equation (see below)
    */

    double result;
    double Log_eta = TMath::Log10(x[0]);
    if (Log_eta >= x_1)
        result = 2 * TMath::Log(10) * Log_eta - Cbar;
    else if ((Log_eta >= x_0) && (Log_eta < x_1))
        result = 2 * TMath::Log(10) * Log_eta - Cbar + a * TMath::Power(x_1 - Log_eta, k);
    else if (Log_eta < x_0)
        result = 0; // for non-conductors
    //else if (Log_eta < x_0)
        //result = delta_0 * TMath::Power(10, 2 * (Log_eta - x_0));   // for conductors
    return result;
}

// Bethe-Bloch:
Double_t Bethe_p(Double_t *x, Double_t *par) 
{
    /*
    The definition of the Bethe-Block function can be found in: 
    http://pdg.lbl.gov/2016/reviews/rpp2016-rev-passage-particles-matter.pdf
    The variables are:
    x[0] := BetaGamma
    par[0] := multiplication factor that accounts for the MeV-ADC conversion
    */

    double z_p = 1;
    double r_e = 2.8179403267 * TMath::Power(10,-12); // [mm]
    double m_e = 0.5109989461;    // [MeV]
    double N_A = 6.022140857 * TMath::Power(10,23); // [1/mol]
    double Z_Si = 14;
    double A_Si = 28.085;  // [g/mol]
    double rho_Si = 2.33 * TMath::Power(10,-3); // [g/mm^3]
    double I_Si = 173 * TMath::Power(10,-6);    // [MeV]
    double K = 4 * TMath::Pi() * N_A * r_e*r_e * m_e;   // [MeV mm^2 / mol]
    double frac_me_mp = m_e/mProton;
    double num_Tmax = 2 * m_e * x[0]*x[0];
    double den_Tmax = 1 + 2 * gamma_v(x) * frac_me_mp + frac_me_mp*frac_me_mp;
    double Tmax = num_Tmax/den_Tmax;
    double fact1 = 0.5 * TMath::Log(2 * m_e * x[0]*x[0] * Tmax / (I_Si*I_Si));
    double fact2 = 1 / inv_Beta2(x);
    double fact3 = 0.5 * delta_BetaGamma(x, 0.2014, 2.8715, 4.4351, 0.14921, 3.2546, 0.058);    // http://www.sciencedirect.com/science/article/pii/0092640X84900020
    double result = par[0] * (K / A_Si) * Z_Si * z_p*z_p * inv_Beta2(x) * (fact1 - fact2 - fact3) * rho_Si;
    return result;
}

Double_t Bethe_p_ADC(Double_t *x, Double_t *par) 
{
    /*
    Same function of Bethe_p() but multiplied by the silicon layer width
    */

    return Bethe_p(x, par) * layerWidth;
}


void figs_Bethe()
{
    /*
    This function displays the LangauMean and its uncertainty LangauSigma as a function of BetaGamma 
    (both the "nominal" one and the one obtained from simulation for layer 1 and 2). These results are compared
    with the real data points analyzed by Francesco
    */

    const int n_points = 7;
    // BeamEnergy at the entrance of the HEPD detector (air contribution is included...values calculated by Francesco)
    double BeamEnergy_Layer1[n_points] = {32.68,47.49,98.55,123.78,152.94,173.03,227.19};   // [MeV] (68,202 not included)
    double BeamEnergy_Layer2[n_points] = {31.70,46.77,97.83,123.37,152.64,172.75,226.95};    // [MeV]

    // BetaGamma nominal,i.e. obtained by relating the BeamEnergy_Layer1 and 2 (= particle kinetic energy) with BetaGamma thorugh function BetaGamma() defined above
    double BetaGammaLayer1_nominal[n_points] = {0.266220,0.322165,0.470212,0.530331,0.593779,0.634693,0.736820};
    double BetaGammaLayer2_nominal[n_points] = {0.262131,0.319654,0.468405,0.529397,0.593152,0.634136,0.736388};
    // BetaGamma obtained from simulation
    double BetaGammaLayer1[n_points] = {0.265260,0.321506,0.469970,0.530039,0.593394,0.634565,0.736721};
    double BetaGammaLayer2[n_points] = {0.259831,0.318061,0.468505,0.528845,0.592022,0.633753,0.735320};
    
    // Energy loss in tracker layer 1 and 2 in MeV (no MeV-ADC conversion factor is adopted!)
    double ELossLayer1[n_points] = {0.971396,0.722455,0.412288,0.350284,0.302673,0.280052,0.233690}; // [MeV]
    double ELossLayer2[n_points] = {1.002555,0.731523,0.415013,0.350809,0.304043,0.278118,0.234642}; // [MeV]

    // Energy loss and in tracker layer 1 and 2 (p- and n-channel) in ADC counts obtained from simulations and its related uncertainty
    double ADCLayer1_pch[n_points] = {333.852868,251.227529,149.297257,127.364462,109.247783,101.266535,82.778968};   // [ADC]
    double Err_ADCLayer1_pch[n_points] = {6.612847,1.449224,2.329383,0.688203,0.656491,0.604015,0.505640};    // [ADC]
    double ADCLayer2_pch[n_points] = {348.094513,257.014252,152.789793,128.772963,110.728523,101.084349,84.236144};    // [ADC]
    double Err_ADCLayer2_pch[n_points] = {4.863760,1.451447,0.814635,0.759604,0.653209,0.663729,0.534607};    // [ADC]
    double ADCLayer1_nch[n_points] = {414.562255,311.571158,184.045856,156.442652,134.270761,124.151375,101.966588};    // [ADC]
    double Err_ADCLayer1_nch[n_points] = {11.754336,1.702559,0.964592,0.809923,15.199819,0.722203,0.614229};    // [ADC]
    double ADCLayer2_nch[n_points] = {427.562887,315.175996,185.802704,156.919605,134.827845,122.727978,102.472618};    // [ADC]
    double Err_ADCLayer2_nch[n_points] = {9.749943,1.708007,0.947937,0.868926,16.208909,0.701841,0.625596};    // [ADC]

    // Energy loss (+ electronic noise) and in tracker layer 1 and 2 (p- and n-channel) in ADC counts obtained from simulations and its related uncertainty 
    double ADCLayer1_pch_noise[n_points] = {333.711811,251.529320,148.920222,127.571624,109.322165,100.930860,82.498769};    // [ADC]
    double Err_ADCLayer1_pch_noise[n_points] = {3.497474,1.541509,1.618184,0.779839,1.315523,0.692094,0.621958};    // [ADC]
    double ADCLayer2_pch_noise[n_points] = {348.030693,257.247293,151.995832,128.269222,111.132543,101.265546,84.417827};    // [ADC]
    double Err_ADCLayer2_pch_noise[n_points] = {7.252152,1.441332,0.881203,0.983115,1.271799,0.669165,0.818436};    // [ADC]
    double ADCLayer1_nch_noise[n_points] = {413.836897,311.895773,184.152913,157.116284,134.224499,124.599881,101.916306};    // [ADC]
    double Err_ADCLayer1_nch_noise[n_points] = {20.756458,1.703955,1.013571,0.875745,0.834097,0.757376,0.709405};    // [ADC]
    double ADCLayer2_nch_noise[n_points] = {427.407170,315.145215,186.120837,157.168281,134.647506,123.317332,102.821859};    // [ADC]
    double Err_ADCLayer2_nch_noise[n_points] = {9.503808,1.700428,0.988604,0.928088,0.807378,0.751821,0.704887};    // [ADC]

    // Real data points from Francesco:
    //double BetaGamma_data[n_points] = {0.279659,0.330785,0.473833,0.533101,0.595989,0.636621,0.738275};//33.8659 (muons)   -> nominal!
    double ADCdata_ladder2_pch[n_points] = {332.535,250.268,139.662,120.297,102.053,95.3886,78.5463};//51.9271
    double Err_ADCdata_ladder2_pch[n_points] = {1.37819,1.21656,1.21079,1.00333,0.757534,2.86342,0.746469};//0.259274
    double ADCdata_ladder3_pch[n_points] = {344.799,260.583,143.054,119.66,102.201,94.1895,78.3564};//52.5984
    double Err_ADCdata_ladder3_pch[n_points] = {0.909381,1.40095,0.810588,0.692243,0.532334,0.513737,0.402994};//0.194752
    double ADCdata_ladder2_nch[n_points] = {378.215,297.49,177.93,153.388,132.507,122.55,99.0033};//53.8292
    double Err_ADCdata_ladder2_nch[n_points] = {5.28342,1.4331,1.0497,0.952546,0.788914,0.868192,0.762296};//0.198917
    double ADCdata_ladder3_nch[n_points] = {300.341,300.012,300.029,299.925,304.254,287.777,281.433};//0
    double Err_ADCdata_ladder3_nch[n_points] = {0,0,0,0,0,0,0};//0

    // The above informations are also contained in the "Bethe_EnergyLoss.txt" file
    printf("FILE: Bethe_EnergyLoss.txt\n");

    //// Input: BeamEnergy -> Output: BetaGamma
    //double BetaGamma_tmp[n_points];
    //printf("BeamEnergy_Layer2 = ");
    //for (int i = 0; i < n_points; i++) {
    //    BetaGamma_tmp[i] = BetaGamma(BeamEnergy_Layer2[i], mProton);
    //    printf("%f ", BetaGamma_tmp[i]);
    //}
    //printf("\n");


    double dEdXLayer1[n_points];
    double dEdXLayer2[n_points];
    for (int i = 0; i < n_points; i++) {
        dEdXLayer1[i] = ELossLayer1[i] / layerWidth;
        dEdXLayer2[i] = ELossLayer2[i] / layerWidth;
    }

    // Relative discrepancy between simulation outcomes and real data. 
    // Propagation error allows to estimate an uncertainty on the results
    double diff_ADC_Layer1_pch[n_points];
    double Err_diff_ADC_Layer1_pch[n_points];
    double diff_ADC_Layer2_pch[n_points];
    double Err_diff_ADC_Layer2_pch[n_points];
    double diff_ADC_Layer1_nch[n_points];
    double Err_diff_ADC_Layer1_nch[n_points];
    double diff_ADC_Layer2_nch[n_points];
    double Err_diff_ADC_Layer2_nch[n_points];

    // Same as above but with the estimation of the electronic noise
    double diff_ADC_Layer1_pch_noise[n_points];
    double Err_diff_ADC_Layer1_pch_noise[n_points];
    double diff_ADC_Layer2_pch_noise[n_points];
    double Err_diff_ADC_Layer2_pch_noise[n_points];
    double diff_ADC_Layer1_nch_noise[n_points];
    double Err_diff_ADC_Layer1_nch_noise[n_points];
    double diff_ADC_Layer2_nch_noise[n_points];
    double Err_diff_ADC_Layer2_nch_noise[n_points];

    for (int i = 0; i < n_points; i++) {
        diff_ADC_Layer1_pch[i] = (ADCLayer1_pch[i] - ADCdata_ladder2_pch[i]) / ADCdata_ladder2_pch[i];
        Err_diff_ADC_Layer1_pch[i] = (TMath::Sqrt((ADCdata_ladder2_pch[i] * ADCdata_ladder2_pch[i]) * (Err_ADCLayer1_pch[i] * Err_ADCLayer1_pch[i]) + (ADCLayer1_pch[i] * ADCLayer1_pch[i]) * (Err_ADCdata_ladder2_pch[i] * Err_ADCdata_ladder2_pch[i]))) / (ADCdata_ladder2_pch[i] * ADCdata_ladder2_pch[i]);
        diff_ADC_Layer2_pch[i] = (ADCLayer2_pch[i] - ADCdata_ladder3_pch[i]) / ADCdata_ladder3_pch[i];
        Err_diff_ADC_Layer2_pch[i] = (TMath::Sqrt((ADCdata_ladder3_pch[i] * ADCdata_ladder3_pch[i]) * (Err_ADCLayer2_pch[i] * Err_ADCLayer2_pch[i]) + (ADCLayer2_pch[i] * ADCLayer2_pch[i]) * (Err_ADCdata_ladder3_pch[i] * Err_ADCdata_ladder3_pch[i]))) / (ADCdata_ladder3_pch[i] * ADCdata_ladder3_pch[i]);
        diff_ADC_Layer1_nch[i] = (ADCLayer1_nch[i] - ADCdata_ladder2_nch[i]) / ADCdata_ladder2_nch[i];
        Err_diff_ADC_Layer1_nch[i] = (TMath::Sqrt((ADCdata_ladder2_nch[i] * ADCdata_ladder2_nch[i]) * (Err_ADCLayer1_nch[i] * Err_ADCLayer1_nch[i]) + (ADCLayer1_nch[i] * ADCLayer1_nch[i]) * (Err_ADCdata_ladder2_nch[i] * Err_ADCdata_ladder2_nch[i]))) / (ADCdata_ladder2_nch[i] * ADCdata_ladder2_nch[i]);
        diff_ADC_Layer2_nch[i] = (ADCLayer2_nch[i] - ADCdata_ladder3_nch[i]) / ADCdata_ladder3_nch[i];
        Err_diff_ADC_Layer2_nch[i] = (TMath::Sqrt((ADCdata_ladder3_nch[i] * ADCdata_ladder3_nch[i]) * (Err_ADCLayer2_nch[i] * Err_ADCLayer2_nch[i]) + (ADCLayer2_nch[i] * ADCLayer2_nch[i]) * (Err_ADCdata_ladder3_nch[i] * Err_ADCdata_ladder3_nch[i]))) / (ADCdata_ladder3_nch[i] * ADCdata_ladder3_nch[i]);

        diff_ADC_Layer1_pch_noise[i] = (ADCLayer1_pch_noise[i] - ADCdata_ladder2_pch[i]) / ADCdata_ladder2_pch[i];
        Err_diff_ADC_Layer1_pch_noise[i] = (TMath::Sqrt((ADCdata_ladder2_pch[i] * ADCdata_ladder2_pch[i]) * (Err_ADCLayer1_pch_noise[i] * Err_ADCLayer1_pch_noise[i]) + (ADCLayer1_pch_noise[i] * ADCLayer1_pch_noise[i]) * (Err_ADCdata_ladder2_pch[i] * Err_ADCdata_ladder2_pch[i]))) / (ADCdata_ladder2_pch[i] * ADCdata_ladder2_pch[i]);
        diff_ADC_Layer2_pch_noise[i] = (ADCLayer2_pch_noise[i] - ADCdata_ladder3_pch[i]) / ADCdata_ladder3_pch[i];
        Err_diff_ADC_Layer2_pch_noise[i] = (TMath::Sqrt((ADCdata_ladder3_pch[i] * ADCdata_ladder3_pch[i]) * (Err_ADCLayer2_pch_noise[i] * Err_ADCLayer2_pch_noise[i]) + (ADCLayer2_pch_noise[i] * ADCLayer2_pch_noise[i]) * (Err_ADCdata_ladder3_pch[i] * Err_ADCdata_ladder3_pch[i]))) / (ADCdata_ladder3_pch[i] * ADCdata_ladder3_pch[i]);
        diff_ADC_Layer1_nch_noise[i] = (ADCLayer1_nch_noise[i] - ADCdata_ladder2_nch[i]) / ADCdata_ladder2_nch[i];
        Err_diff_ADC_Layer1_nch_noise[i] = (TMath::Sqrt((ADCdata_ladder2_nch[i] * ADCdata_ladder2_nch[i]) * (Err_ADCLayer1_nch_noise[i] * Err_ADCLayer1_nch_noise[i]) + (ADCLayer1_nch_noise[i] * ADCLayer1_nch_noise[i]) * (Err_ADCdata_ladder2_nch[i] * Err_ADCdata_ladder2_nch[i]))) / (ADCdata_ladder2_nch[i] * ADCdata_ladder2_nch[i]);
        diff_ADC_Layer2_nch_noise[i] = (ADCLayer2_nch_noise[i] - ADCdata_ladder3_nch[i]) / ADCdata_ladder3_nch[i];
        Err_diff_ADC_Layer2_nch_noise[i] = (TMath::Sqrt((ADCdata_ladder3_nch[i] * ADCdata_ladder3_nch[i]) * (Err_ADCLayer2_nch_noise[i] * Err_ADCLayer2_nch_noise[i]) + (ADCLayer2_nch_noise[i] * ADCLayer2_nch_noise[i]) * (Err_ADCdata_ladder3_nch[i] * Err_ADCdata_ladder3_nch[i]))) / (ADCdata_ladder3_nch[i] * ADCdata_ladder3_nch[i]);
    }

    //double diff_ADC_ladder2_pch[n_points];
    //double diff_ADC_ladder3_pch[n_points];
    //double diff_ADC_ladder2_nch[n_points];
    //double diff_ADC_ladder3_nch[n_points];
    //for (int i = 0; i < n_points; i++) {
    //    diff_ADC_ladder2_pch[i] = (ADCdata_ladder2_pch[i] - Bethe_p_ADC(&BetaGammaLayer1_nominal[i], &p2ch_ADCconv)) / Bethe_p_ADC(&BetaGammaLayer1_nominal[i], &p2ch_ADCconv);
    //    diff_ADC_ladder3_pch[i] = (ADCdata_ladder3_pch[i] - Bethe_p_ADC(&BetaGammaLayer2_nominal[i], &p3ch_ADCconv)) / Bethe_p_ADC(&BetaGammaLayer2_nominal[i], &p3ch_ADCconv);
    //    diff_ADC_ladder2_nch[i] = (ADCdata_ladder2_nch[i] - Bethe_p_ADC(&BetaGammaLayer1_nominal[i], &n2ch_ADCconv)) / Bethe_p_ADC(&BetaGammaLayer1_nominal[i], &n2ch_ADCconv);
    //    diff_ADC_ladder3_nch[i] = (ADCdata_ladder3_nch[i] - Bethe_p_ADC(&BetaGammaLayer2_nominal[i], &n3ch_ADCconv)) / Bethe_p_ADC(&BetaGammaLayer2_nominal[i], &n3ch_ADCconv);
    //}


    TF1 *BetheFit = new TF1("BetheFit",Bethe_p,0.0001,1,1);
    BetheFit->SetLineStyle(1);
    BetheFit->SetLineColor(kBlack);
    BetheFit->SetParNames("Norm");

    TF1 *BetheFitADC = new TF1("BetheFitADC",Bethe_p_ADC,0.0001,1,1);
    BetheFitADC->SetLineStyle(1);
    BetheFitADC->SetLineColor(kBlack);
    BetheFitADC->SetParNames("NormADC");


    // Bethe nominal Layer1 (1)
    TGraphErrors *graphBetheLayer1_nominal = new TGraphErrors(n_points,BetaGammaLayer1_nominal,dEdXLayer1,0,0);
    graphBetheLayer1_nominal->SetTitle("Bethe nominal Layer1");
    graphBetheLayer1_nominal->GetYaxis()->SetLabelOffset(0.005);
    graphBetheLayer1_nominal->GetYaxis()->SetLabelSize(0.045);
    graphBetheLayer1_nominal->GetYaxis()->SetTitle("#LT#frac{dE}{dX}#GT [MeV / mm]");
    graphBetheLayer1_nominal->GetYaxis()->SetTitleSize(0.050);
    graphBetheLayer1_nominal->GetYaxis()->SetTitleOffset(1.25);
    graphBetheLayer1_nominal->GetYaxis()->CenterTitle();
    graphBetheLayer1_nominal->GetXaxis()->SetLabelOffset(0.010);
    graphBetheLayer1_nominal->GetXaxis()->SetLabelSize(0.045);
    graphBetheLayer1_nominal->GetXaxis()->SetTitle("#beta#gamma_{nom}");
    graphBetheLayer1_nominal->GetXaxis()->SetTitleSize(0.050);
    graphBetheLayer1_nominal->GetXaxis()->SetTitleOffset(1.1);
    graphBetheLayer1_nominal->GetXaxis()->CenterTitle();
    graphBetheLayer1_nominal->SetMarkerStyle(kFullCircle);
    graphBetheLayer1_nominal->SetMarkerSize(1.0);
    graphBetheLayer1_nominal->SetMarkerColor(kRed);
    graphBetheLayer1_nominal->SetLineColor(kRed);
    graphBetheLayer1_nominal->SetFillColor(0);

    // Bethe nominal Layer2 (1)
    TGraphErrors *graphBetheLayer2_nominal = new TGraphErrors(n_points,BetaGammaLayer2_nominal,dEdXLayer2,0,0);
    graphBetheLayer2_nominal->SetTitle("Bethe nominal Layer2");
    graphBetheLayer2_nominal->GetYaxis()->SetLabelOffset(0.005);
    graphBetheLayer2_nominal->GetYaxis()->SetLabelSize(0.045);
    graphBetheLayer2_nominal->GetYaxis()->SetTitle("#LT#frac{dE}{dX}#GT [MeV / mm]");
    graphBetheLayer2_nominal->GetYaxis()->SetTitleSize(0.050);
    graphBetheLayer2_nominal->GetYaxis()->SetTitleOffset(1.25);
    graphBetheLayer2_nominal->GetYaxis()->CenterTitle();
    graphBetheLayer2_nominal->GetXaxis()->SetLabelOffset(0.010);
    graphBetheLayer2_nominal->GetXaxis()->SetLabelSize(0.045);
    graphBetheLayer2_nominal->GetXaxis()->SetTitle("#beta#gamma_{nom}");
    graphBetheLayer2_nominal->GetXaxis()->SetTitleSize(0.050);
    graphBetheLayer2_nominal->GetXaxis()->SetTitleOffset(1.1);
    graphBetheLayer2_nominal->GetXaxis()->CenterTitle();
    graphBetheLayer2_nominal->SetMarkerStyle(kFullCircle);
    graphBetheLayer2_nominal->SetMarkerSize(1.0);
    graphBetheLayer2_nominal->SetMarkerColor(kBlue);
    graphBetheLayer2_nominal->SetLineColor(kBlue);
    graphBetheLayer2_nominal->SetFillColor(0);

    // Bethe simulation Layer1 (1)
    TGraphErrors *graphBetheLayer1_simulation = new TGraphErrors(n_points,BetaGammaLayer1,dEdXLayer1,0,0);
    graphBetheLayer1_simulation->SetTitle("Bethe simulation Layer1");
    graphBetheLayer1_simulation->GetYaxis()->SetLabelOffset(0.005);
    graphBetheLayer1_simulation->GetYaxis()->SetLabelSize(0.045);
    graphBetheLayer1_simulation->GetYaxis()->SetTitle("#LT#frac{dE}{dX}#GT [MeV / mm]");
    graphBetheLayer1_simulation->GetYaxis()->SetTitleSize(0.050);
    graphBetheLayer1_simulation->GetYaxis()->SetTitleOffset(1.25);
    graphBetheLayer1_simulation->GetYaxis()->CenterTitle();
    graphBetheLayer1_simulation->GetXaxis()->SetLabelOffset(0.010);
    graphBetheLayer1_simulation->GetXaxis()->SetLabelSize(0.045);
    graphBetheLayer1_simulation->GetXaxis()->SetTitle("#beta#gamma_{sim}");
    graphBetheLayer1_simulation->GetXaxis()->SetTitleSize(0.050);
    graphBetheLayer1_simulation->GetXaxis()->SetTitleOffset(1.1);
    graphBetheLayer1_simulation->GetXaxis()->CenterTitle();
    graphBetheLayer1_simulation->SetMarkerStyle(kFullCircle);
    graphBetheLayer1_simulation->SetMarkerSize(1.0);
    graphBetheLayer1_simulation->SetMarkerColor(kRed);
    graphBetheLayer1_simulation->SetLineColor(kRed);
    graphBetheLayer1_simulation->SetFillColor(0);

    // Bethe simulation Layer2 (1)
    TGraphErrors *graphBetheLayer2_simulation = new TGraphErrors(n_points,BetaGammaLayer2,dEdXLayer2,0,0);
    graphBetheLayer2_simulation->SetTitle("Bethe simulation Layer2");
    graphBetheLayer2_simulation->GetYaxis()->SetLabelOffset(0.005);
    graphBetheLayer2_simulation->GetYaxis()->SetLabelSize(0.045);
    graphBetheLayer2_simulation->GetYaxis()->SetTitle("#LT#frac{dE}{dX}#GT [MeV / mm]");
    graphBetheLayer2_simulation->GetYaxis()->SetTitleSize(0.050);
    graphBetheLayer2_simulation->GetYaxis()->SetTitleOffset(1.25);
    graphBetheLayer2_simulation->GetYaxis()->CenterTitle();
    graphBetheLayer2_simulation->GetXaxis()->SetLabelOffset(0.010);
    graphBetheLayer2_simulation->GetXaxis()->SetLabelSize(0.045);
    graphBetheLayer2_simulation->GetXaxis()->SetTitle("#beta#gamma_{sim}");
    graphBetheLayer2_simulation->GetXaxis()->SetTitleSize(0.050);
    graphBetheLayer2_simulation->GetXaxis()->SetTitleOffset(1.1);
    graphBetheLayer2_simulation->GetXaxis()->CenterTitle();
    graphBetheLayer2_simulation->SetMarkerStyle(kFullCircle);
    graphBetheLayer2_simulation->SetMarkerSize(1.0);
    graphBetheLayer2_simulation->SetMarkerColor(kBlue);
    graphBetheLayer2_simulation->SetLineColor(kBlue);
    graphBetheLayer2_simulation->SetFillColor(0);


    // Bethe ADC_Layer1_pch nominal (2)
    TGraphErrors *graphBetheADC_Layer1_pch_nominal = new TGraphErrors(n_points,BetaGammaLayer1_nominal,ADCLayer1_pch,0,Err_ADCLayer1_pch);
    graphBetheADC_Layer1_pch_nominal->SetTitle("Bethe ADC_Layer1_pch nominal");
    graphBetheADC_Layer1_pch_nominal->GetYaxis()->SetLabelOffset(0.005);
    graphBetheADC_Layer1_pch_nominal->GetYaxis()->SetLabelSize(0.045);
    graphBetheADC_Layer1_pch_nominal->GetYaxis()->SetTitle("#LT#Delta E#GT [ADC]");
    graphBetheADC_Layer1_pch_nominal->GetYaxis()->SetTitleSize(0.050);
    graphBetheADC_Layer1_pch_nominal->GetYaxis()->SetTitleOffset(1.25);
    graphBetheADC_Layer1_pch_nominal->GetYaxis()->CenterTitle();
    graphBetheADC_Layer1_pch_nominal->GetXaxis()->SetLabelOffset(0.010);
    graphBetheADC_Layer1_pch_nominal->GetXaxis()->SetLabelSize(0.045);
    graphBetheADC_Layer1_pch_nominal->GetXaxis()->SetTitle("#beta#gamma_{nom}");
    graphBetheADC_Layer1_pch_nominal->GetXaxis()->SetTitleSize(0.050);
    graphBetheADC_Layer1_pch_nominal->GetXaxis()->SetTitleOffset(1.1);
    graphBetheADC_Layer1_pch_nominal->GetXaxis()->CenterTitle();
    graphBetheADC_Layer1_pch_nominal->SetMarkerStyle(kFullCircle);
    graphBetheADC_Layer1_pch_nominal->SetMarkerSize(1.0);
    graphBetheADC_Layer1_pch_nominal->SetMarkerColor(kRed);
    graphBetheADC_Layer1_pch_nominal->SetLineColor(kRed);
    graphBetheADC_Layer1_pch_nominal->SetFillColor(0);

    // Bethe ADC_Layer2_pch nominal (2)
    TGraphErrors *graphBetheADC_Layer2_pch_nominal = new TGraphErrors(n_points,BetaGammaLayer2_nominal,ADCLayer2_pch,0,Err_ADCLayer2_pch);
    graphBetheADC_Layer2_pch_nominal->SetTitle("Bethe ADC_Layer2_pch nominal");
    graphBetheADC_Layer2_pch_nominal->GetYaxis()->SetLabelOffset(0.005);
    graphBetheADC_Layer2_pch_nominal->GetYaxis()->SetLabelSize(0.045);
    graphBetheADC_Layer2_pch_nominal->GetYaxis()->SetTitle("#LT#Delta E#GT [ADC]");
    graphBetheADC_Layer2_pch_nominal->GetYaxis()->SetTitleSize(0.050);
    graphBetheADC_Layer2_pch_nominal->GetYaxis()->SetTitleOffset(1.25);
    graphBetheADC_Layer2_pch_nominal->GetYaxis()->CenterTitle();
    graphBetheADC_Layer2_pch_nominal->GetXaxis()->SetLabelOffset(0.010);
    graphBetheADC_Layer2_pch_nominal->GetXaxis()->SetLabelSize(0.045);
    graphBetheADC_Layer2_pch_nominal->GetXaxis()->SetTitle("#beta#gamma_{nom}");
    graphBetheADC_Layer2_pch_nominal->GetXaxis()->SetTitleSize(0.050);
    graphBetheADC_Layer2_pch_nominal->GetXaxis()->SetTitleOffset(1.1);
    graphBetheADC_Layer2_pch_nominal->GetXaxis()->CenterTitle();
    graphBetheADC_Layer2_pch_nominal->SetMarkerStyle(kFullCircle);
    graphBetheADC_Layer2_pch_nominal->SetMarkerSize(1.0);
    graphBetheADC_Layer2_pch_nominal->SetMarkerColor(kBlue);
    graphBetheADC_Layer2_pch_nominal->SetLineColor(kBlue);
    graphBetheADC_Layer2_pch_nominal->SetFillColor(0);

    // diff ADC_Layer1_pch nominal (2)
    TGraphErrors *graphdiff_ADC_Layer1_pch_nominal = new TGraphErrors(n_points,BetaGammaLayer1_nominal,diff_ADC_Layer1_pch,0,Err_diff_ADC_Layer1_pch);
    graphdiff_ADC_Layer1_pch_nominal->SetTitle("diff ADC_Layer1_pch nominal");
    graphdiff_ADC_Layer1_pch_nominal->GetYaxis()->SetLabelOffset(0.005);
    graphdiff_ADC_Layer1_pch_nominal->GetYaxis()->SetLabelSize(0.045);
    graphdiff_ADC_Layer1_pch_nominal->GetYaxis()->SetTitle("#frac{#LT#Delta E#GT - Data}{Data}");
    graphdiff_ADC_Layer1_pch_nominal->GetYaxis()->SetTitleSize(0.050);
    graphdiff_ADC_Layer1_pch_nominal->GetYaxis()->SetTitleOffset(1.25);
    graphdiff_ADC_Layer1_pch_nominal->GetYaxis()->CenterTitle();
    graphdiff_ADC_Layer1_pch_nominal->GetYaxis()->SetRangeUser(-0.3,0.2);
    graphdiff_ADC_Layer1_pch_nominal->GetXaxis()->SetLabelOffset(0.010);
    graphdiff_ADC_Layer1_pch_nominal->GetXaxis()->SetLabelSize(0.045);
    graphdiff_ADC_Layer1_pch_nominal->GetXaxis()->SetTitle("#beta#gamma_{nom}");
    graphdiff_ADC_Layer1_pch_nominal->GetXaxis()->SetTitleSize(0.050);
    graphdiff_ADC_Layer1_pch_nominal->GetXaxis()->SetTitleOffset(1.1);
    graphdiff_ADC_Layer1_pch_nominal->GetXaxis()->CenterTitle();
    graphdiff_ADC_Layer1_pch_nominal->SetMarkerStyle(kFullCircle);
    graphdiff_ADC_Layer1_pch_nominal->SetMarkerSize(1.0);
    graphdiff_ADC_Layer1_pch_nominal->SetMarkerColor(kRed);
    graphdiff_ADC_Layer1_pch_nominal->SetLineColor(kRed);
    graphdiff_ADC_Layer1_pch_nominal->SetFillColor(0);

    // diff ADC_Layer2_pch nominal (2)
    TGraphErrors *graphdiff_ADC_Layer2_pch_nominal = new TGraphErrors(n_points,BetaGammaLayer2_nominal,diff_ADC_Layer2_pch,0,Err_diff_ADC_Layer2_pch);
    graphdiff_ADC_Layer2_pch_nominal->SetTitle("diff ADC_Layer2_pch nominal");
    graphdiff_ADC_Layer2_pch_nominal->GetYaxis()->SetLabelOffset(0.005);
    graphdiff_ADC_Layer2_pch_nominal->GetYaxis()->SetLabelSize(0.045);
    graphdiff_ADC_Layer2_pch_nominal->GetYaxis()->SetTitle("#frac{#LT#Delta E#GT - Data}{Data}");
    graphdiff_ADC_Layer2_pch_nominal->GetYaxis()->SetTitleSize(0.050);
    graphdiff_ADC_Layer2_pch_nominal->GetYaxis()->SetTitleOffset(1.25);
    graphdiff_ADC_Layer2_pch_nominal->GetYaxis()->CenterTitle();
    graphdiff_ADC_Layer2_pch_nominal->GetYaxis()->SetRangeUser(-0.3,0.2);
    graphdiff_ADC_Layer2_pch_nominal->GetXaxis()->SetLabelOffset(0.010);
    graphdiff_ADC_Layer2_pch_nominal->GetXaxis()->SetLabelSize(0.045);
    graphdiff_ADC_Layer2_pch_nominal->GetXaxis()->SetTitle("#beta#gamma_{nom}");
    graphdiff_ADC_Layer2_pch_nominal->GetXaxis()->SetTitleSize(0.050);
    graphdiff_ADC_Layer2_pch_nominal->GetXaxis()->SetTitleOffset(1.1);
    graphdiff_ADC_Layer2_pch_nominal->GetXaxis()->CenterTitle();
    graphdiff_ADC_Layer2_pch_nominal->SetMarkerStyle(kFullCircle);
    graphdiff_ADC_Layer2_pch_nominal->SetMarkerSize(1.0);
    graphdiff_ADC_Layer2_pch_nominal->SetMarkerColor(kBlue);
    graphdiff_ADC_Layer2_pch_nominal->SetLineColor(kBlue);
    graphdiff_ADC_Layer2_pch_nominal->SetFillColor(0);


    // Bethe ADC_ladder2_pch (2, 21, 4, 41)
    TGraphErrors *graphBetheADCdata_ladder2_pch = new TGraphErrors(n_points,BetaGammaLayer1_nominal,ADCdata_ladder2_pch,0,Err_ADCdata_ladder2_pch);
    graphBetheADCdata_ladder2_pch->SetTitle("Bethe ADC_ladder2_pch");
    graphBetheADCdata_ladder2_pch->GetYaxis()->SetLabelOffset(0.005);
    graphBetheADCdata_ladder2_pch->GetYaxis()->SetLabelSize(0.045);
    graphBetheADCdata_ladder2_pch->GetYaxis()->SetTitle("#LT#Delta E#GT [ADC]");
    graphBetheADCdata_ladder2_pch->GetYaxis()->SetTitleSize(0.050);
    graphBetheADCdata_ladder2_pch->GetYaxis()->SetTitleOffset(1.25);
    graphBetheADCdata_ladder2_pch->GetYaxis()->CenterTitle();
    graphBetheADCdata_ladder2_pch->GetXaxis()->SetLabelOffset(0.010);
    graphBetheADCdata_ladder2_pch->GetXaxis()->SetLabelSize(0.045);
    graphBetheADCdata_ladder2_pch->GetXaxis()->SetTitle("#beta#gamma_{nom}");
    graphBetheADCdata_ladder2_pch->GetXaxis()->SetTitleSize(0.050);
    graphBetheADCdata_ladder2_pch->GetXaxis()->SetTitleOffset(1.1);
    graphBetheADCdata_ladder2_pch->GetXaxis()->CenterTitle();
    graphBetheADCdata_ladder2_pch->SetMarkerStyle(kFullSquare);
    graphBetheADCdata_ladder2_pch->SetMarkerSize(1.0);
    graphBetheADCdata_ladder2_pch->SetMarkerColor(kGreen+2);
    graphBetheADCdata_ladder2_pch->SetLineColor(kGreen+2);
    graphBetheADCdata_ladder2_pch->SetFillColor(0);

    // Bethe ADC_ladder3_pch (2, 21, 4, 41)
    TGraphErrors *graphBetheADCdata_ladder3_pch = new TGraphErrors(n_points,BetaGammaLayer2_nominal,ADCdata_ladder3_pch,0,Err_ADCdata_ladder3_pch);
    graphBetheADCdata_ladder3_pch->SetTitle("Bethe ADC_ladder3_pch");
    graphBetheADCdata_ladder3_pch->GetYaxis()->SetLabelOffset(0.005);
    graphBetheADCdata_ladder3_pch->GetYaxis()->SetLabelSize(0.045);
    graphBetheADCdata_ladder3_pch->GetYaxis()->SetTitle("#LT#Delta E#GT [ADC]");
    graphBetheADCdata_ladder3_pch->GetYaxis()->SetTitleSize(0.050);
    graphBetheADCdata_ladder3_pch->GetYaxis()->SetTitleOffset(1.25);
    graphBetheADCdata_ladder3_pch->GetYaxis()->CenterTitle();
    graphBetheADCdata_ladder3_pch->GetXaxis()->SetLabelOffset(0.010);
    graphBetheADCdata_ladder3_pch->GetXaxis()->SetLabelSize(0.045);
    graphBetheADCdata_ladder3_pch->GetXaxis()->SetTitle("#beta#gamma_{nom}");
    graphBetheADCdata_ladder3_pch->GetXaxis()->SetTitleSize(0.050);
    graphBetheADCdata_ladder3_pch->GetXaxis()->SetTitleOffset(1.1);
    graphBetheADCdata_ladder3_pch->GetXaxis()->CenterTitle();
    graphBetheADCdata_ladder3_pch->SetMarkerStyle(kFullSquare);
    graphBetheADCdata_ladder3_pch->SetMarkerSize(1.0);
    graphBetheADCdata_ladder3_pch->SetMarkerColor(kOrange-3);
    graphBetheADCdata_ladder3_pch->SetLineColor(kOrange-3);
    graphBetheADCdata_ladder3_pch->SetFillColor(0);


    // Bethe ADC_Layer1_pch simulation (21)
    TGraphErrors *graphBetheADC_Layer1_pch_simulation = new TGraphErrors(n_points,BetaGammaLayer1,ADCLayer1_pch,0,Err_ADCLayer1_pch);
    graphBetheADC_Layer1_pch_simulation->SetTitle("Bethe ADC_Layer1_pch simulation");
    graphBetheADC_Layer1_pch_simulation->GetYaxis()->SetLabelOffset(0.005);
    graphBetheADC_Layer1_pch_simulation->GetYaxis()->SetLabelSize(0.045);
    graphBetheADC_Layer1_pch_simulation->GetYaxis()->SetTitle("#LT#Delta E#GT [ADC]");
    graphBetheADC_Layer1_pch_simulation->GetYaxis()->SetTitleSize(0.050);
    graphBetheADC_Layer1_pch_simulation->GetYaxis()->SetTitleOffset(1.25);
    graphBetheADC_Layer1_pch_simulation->GetYaxis()->CenterTitle();
    graphBetheADC_Layer1_pch_simulation->GetXaxis()->SetLabelOffset(0.010);
    graphBetheADC_Layer1_pch_simulation->GetXaxis()->SetLabelSize(0.045);
    graphBetheADC_Layer1_pch_simulation->GetXaxis()->SetTitle("#beta#gamma_{sim}");
    graphBetheADC_Layer1_pch_simulation->GetXaxis()->SetTitleSize(0.050);
    graphBetheADC_Layer1_pch_simulation->GetXaxis()->SetTitleOffset(1.1);
    graphBetheADC_Layer1_pch_simulation->GetXaxis()->CenterTitle();
    graphBetheADC_Layer1_pch_simulation->SetMarkerStyle(kFullCircle);
    graphBetheADC_Layer1_pch_simulation->SetMarkerSize(1.0);
    graphBetheADC_Layer1_pch_simulation->SetMarkerColor(kRed);
    graphBetheADC_Layer1_pch_simulation->SetLineColor(kRed);
    graphBetheADC_Layer1_pch_simulation->SetFillColor(0);

    // Bethe ADC_Layer2_pch simulation (21)
    TGraphErrors *graphBetheADC_Layer2_pch_simulation = new TGraphErrors(n_points,BetaGammaLayer2,ADCLayer2_pch,0,Err_ADCLayer2_pch);
    graphBetheADC_Layer2_pch_simulation->SetTitle("Bethe ADC_Layer2_pch simulation");
    graphBetheADC_Layer2_pch_simulation->GetYaxis()->SetLabelOffset(0.005);
    graphBetheADC_Layer2_pch_simulation->GetYaxis()->SetLabelSize(0.045);
    graphBetheADC_Layer2_pch_simulation->GetYaxis()->SetTitle("#LT#Delta E#GT [ADC]");
    graphBetheADC_Layer2_pch_simulation->GetYaxis()->SetTitleSize(0.050);
    graphBetheADC_Layer2_pch_simulation->GetYaxis()->SetTitleOffset(1.25);
    graphBetheADC_Layer2_pch_simulation->GetYaxis()->CenterTitle();
    graphBetheADC_Layer2_pch_simulation->GetXaxis()->SetLabelOffset(0.010);
    graphBetheADC_Layer2_pch_simulation->GetXaxis()->SetLabelSize(0.045);
    graphBetheADC_Layer2_pch_simulation->GetXaxis()->SetTitle("#beta#gamma_{sim}");
    graphBetheADC_Layer2_pch_simulation->GetXaxis()->SetTitleSize(0.050);
    graphBetheADC_Layer2_pch_simulation->GetXaxis()->SetTitleOffset(1.1);
    graphBetheADC_Layer2_pch_simulation->GetXaxis()->CenterTitle();
    graphBetheADC_Layer2_pch_simulation->SetMarkerStyle(kFullCircle);
    graphBetheADC_Layer2_pch_simulation->SetMarkerSize(1.0);
    graphBetheADC_Layer2_pch_simulation->SetMarkerColor(kBlue);
    graphBetheADC_Layer2_pch_simulation->SetLineColor(kBlue);
    graphBetheADC_Layer2_pch_simulation->SetFillColor(0);

    // diff ADC_Layer1_pch simulation (21)
    TGraphErrors *graphdiff_ADC_Layer1_pch_simulation = new TGraphErrors(n_points,BetaGammaLayer1,diff_ADC_Layer1_pch,0,Err_diff_ADC_Layer1_pch);
    graphdiff_ADC_Layer1_pch_simulation->SetTitle("diff ADC_Layer1_pch simulation");
    graphdiff_ADC_Layer1_pch_simulation->GetYaxis()->SetLabelOffset(0.005);
    graphdiff_ADC_Layer1_pch_simulation->GetYaxis()->SetLabelSize(0.045);
    graphdiff_ADC_Layer1_pch_simulation->GetYaxis()->SetTitle("#frac{#LT#Delta E#GT - Data}{Data}");
    graphdiff_ADC_Layer1_pch_simulation->GetYaxis()->SetTitleSize(0.050);
    graphdiff_ADC_Layer1_pch_simulation->GetYaxis()->SetTitleOffset(1.25);
    graphdiff_ADC_Layer1_pch_simulation->GetYaxis()->CenterTitle();
    graphdiff_ADC_Layer1_pch_simulation->GetYaxis()->SetRangeUser(-0.3,0.2);
    graphdiff_ADC_Layer1_pch_simulation->GetXaxis()->SetLabelOffset(0.010);
    graphdiff_ADC_Layer1_pch_simulation->GetXaxis()->SetLabelSize(0.045);
    graphdiff_ADC_Layer1_pch_simulation->GetXaxis()->SetTitle("#beta#gamma_{sim}");
    graphdiff_ADC_Layer1_pch_simulation->GetXaxis()->SetTitleSize(0.050);
    graphdiff_ADC_Layer1_pch_simulation->GetXaxis()->SetTitleOffset(1.1);
    graphdiff_ADC_Layer1_pch_simulation->GetXaxis()->CenterTitle();
    graphdiff_ADC_Layer1_pch_simulation->SetMarkerStyle(kFullCircle);
    graphdiff_ADC_Layer1_pch_simulation->SetMarkerSize(1.0);
    graphdiff_ADC_Layer1_pch_simulation->SetMarkerColor(kRed);
    graphdiff_ADC_Layer1_pch_simulation->SetLineColor(kRed);
    graphdiff_ADC_Layer1_pch_simulation->SetFillColor(0);

    // diff ADC_Layer1_pch simulation (21)
    TGraphErrors *graphdiff_ADC_Layer2_pch_simulation = new TGraphErrors(n_points,BetaGammaLayer2,diff_ADC_Layer2_pch,0,Err_diff_ADC_Layer2_pch);
    graphdiff_ADC_Layer2_pch_simulation->SetTitle("diff ADC_Layer2_pch simulation");
    graphdiff_ADC_Layer2_pch_simulation->GetYaxis()->SetLabelOffset(0.005);
    graphdiff_ADC_Layer2_pch_simulation->GetYaxis()->SetLabelSize(0.045);
    graphdiff_ADC_Layer2_pch_simulation->GetYaxis()->SetTitle("#frac{#LT#Delta E#GT - Data}{Data}");
    graphdiff_ADC_Layer2_pch_simulation->GetYaxis()->SetTitleSize(0.050);
    graphdiff_ADC_Layer2_pch_simulation->GetYaxis()->SetTitleOffset(1.25);
    graphdiff_ADC_Layer2_pch_simulation->GetYaxis()->CenterTitle();
    graphdiff_ADC_Layer2_pch_simulation->GetYaxis()->SetRangeUser(-0.3,0.2);
    graphdiff_ADC_Layer2_pch_simulation->GetXaxis()->SetLabelOffset(0.010);
    graphdiff_ADC_Layer2_pch_simulation->GetXaxis()->SetLabelSize(0.045);
    graphdiff_ADC_Layer2_pch_simulation->GetXaxis()->SetTitle("#beta#gamma_{sim}");
    graphdiff_ADC_Layer2_pch_simulation->GetXaxis()->SetTitleSize(0.050);
    graphdiff_ADC_Layer2_pch_simulation->GetXaxis()->SetTitleOffset(1.1);
    graphdiff_ADC_Layer2_pch_simulation->GetXaxis()->CenterTitle();
    graphdiff_ADC_Layer2_pch_simulation->SetMarkerStyle(kFullCircle);
    graphdiff_ADC_Layer2_pch_simulation->SetMarkerSize(1.0);
    graphdiff_ADC_Layer2_pch_simulation->SetMarkerColor(kBlue);
    graphdiff_ADC_Layer2_pch_simulation->SetLineColor(kBlue);
    graphdiff_ADC_Layer2_pch_simulation->SetFillColor(0);


    // Bethe ADC_Layer1_nch nominal (3)
    TGraphErrors *graphBetheADC_Layer1_nch_nominal = new TGraphErrors(n_points,BetaGammaLayer1_nominal,ADCLayer1_nch,0,Err_ADCLayer1_nch);
    graphBetheADC_Layer1_nch_nominal->SetTitle("Bethe ADC_Layer1_nch nominal");
    graphBetheADC_Layer1_nch_nominal->GetYaxis()->SetLabelOffset(0.005);
    graphBetheADC_Layer1_nch_nominal->GetYaxis()->SetLabelSize(0.045);
    graphBetheADC_Layer1_nch_nominal->GetYaxis()->SetTitle("#LT#Delta E#GT [ADC]");
    graphBetheADC_Layer1_nch_nominal->GetYaxis()->SetTitleSize(0.050);
    graphBetheADC_Layer1_nch_nominal->GetYaxis()->SetTitleOffset(1.25);
    graphBetheADC_Layer1_nch_nominal->GetYaxis()->CenterTitle();
    graphBetheADC_Layer1_nch_nominal->GetXaxis()->SetLabelOffset(0.010);
    graphBetheADC_Layer1_nch_nominal->GetXaxis()->SetLabelSize(0.045);
    graphBetheADC_Layer1_nch_nominal->GetXaxis()->SetTitle("#beta#gamma_{nom}");
    graphBetheADC_Layer1_nch_nominal->GetXaxis()->SetTitleSize(0.050);
    graphBetheADC_Layer1_nch_nominal->GetXaxis()->SetTitleOffset(1.1);
    graphBetheADC_Layer1_nch_nominal->GetXaxis()->CenterTitle();
    graphBetheADC_Layer1_nch_nominal->SetMarkerStyle(kFullCircle);
    graphBetheADC_Layer1_nch_nominal->SetMarkerSize(1.0);
    graphBetheADC_Layer1_nch_nominal->SetMarkerColor(kRed);
    graphBetheADC_Layer1_nch_nominal->SetLineColor(kRed);
    graphBetheADC_Layer1_nch_nominal->SetFillColor(0);

    // Bethe ADC_Layer2_nch nominal (3)
    TGraphErrors *graphBetheADC_Layer2_nch_nominal = new TGraphErrors(n_points,BetaGammaLayer2_nominal,ADCLayer2_nch,0,Err_ADCLayer2_nch);
    graphBetheADC_Layer2_nch_nominal->SetTitle("Bethe ADC_Layer2_nch nominal");
    graphBetheADC_Layer2_nch_nominal->GetYaxis()->SetLabelOffset(0.005);
    graphBetheADC_Layer2_nch_nominal->GetYaxis()->SetLabelSize(0.045);
    graphBetheADC_Layer2_nch_nominal->GetYaxis()->SetTitle("#LT#Delta E#GT [ADC]");
    graphBetheADC_Layer2_nch_nominal->GetYaxis()->SetTitleSize(0.050);
    graphBetheADC_Layer2_nch_nominal->GetYaxis()->SetTitleOffset(1.25);
    graphBetheADC_Layer2_nch_nominal->GetYaxis()->CenterTitle();
    graphBetheADC_Layer2_nch_nominal->GetXaxis()->SetLabelOffset(0.010);
    graphBetheADC_Layer2_nch_nominal->GetXaxis()->SetLabelSize(0.045);
    graphBetheADC_Layer2_nch_nominal->GetXaxis()->SetTitle("#beta#gamma_{nom}");
    graphBetheADC_Layer2_nch_nominal->GetXaxis()->SetTitleSize(0.050);
    graphBetheADC_Layer2_nch_nominal->GetXaxis()->SetTitleOffset(1.1);
    graphBetheADC_Layer2_nch_nominal->GetXaxis()->CenterTitle();
    graphBetheADC_Layer2_nch_nominal->SetMarkerStyle(kFullCircle);
    graphBetheADC_Layer2_nch_nominal->SetMarkerSize(1.0);
    graphBetheADC_Layer2_nch_nominal->SetMarkerColor(kBlue);
    graphBetheADC_Layer2_nch_nominal->SetLineColor(kBlue);
    graphBetheADC_Layer2_nch_nominal->SetFillColor(0);

    // diff ADC_Layer1_nch nominal (3)
    TGraphErrors *graphdiff_ADC_Layer1_nch_nominal = new TGraphErrors(n_points,BetaGammaLayer1_nominal,diff_ADC_Layer1_nch,0,Err_diff_ADC_Layer1_nch);
    graphdiff_ADC_Layer1_nch_nominal->SetTitle("diff ADC_Layer1_nch nominal");
    graphdiff_ADC_Layer1_nch_nominal->GetYaxis()->SetLabelOffset(0.005);
    graphdiff_ADC_Layer1_nch_nominal->GetYaxis()->SetLabelSize(0.045);
    graphdiff_ADC_Layer1_nch_nominal->GetYaxis()->SetTitle("#frac{#LT#Delta E#GT - Data}{Data}");
    graphdiff_ADC_Layer1_nch_nominal->GetYaxis()->SetTitleSize(0.050);
    graphdiff_ADC_Layer1_nch_nominal->GetYaxis()->SetTitleOffset(1.25);
    graphdiff_ADC_Layer1_nch_nominal->GetYaxis()->CenterTitle();
    graphdiff_ADC_Layer1_nch_nominal->GetYaxis()->SetRangeUser(-0.3,0.2);
    graphdiff_ADC_Layer1_nch_nominal->GetXaxis()->SetLabelOffset(0.010);
    graphdiff_ADC_Layer1_nch_nominal->GetXaxis()->SetLabelSize(0.045);
    graphdiff_ADC_Layer1_nch_nominal->GetXaxis()->SetTitle("#beta#gamma_{nom}");
    graphdiff_ADC_Layer1_nch_nominal->GetXaxis()->SetTitleSize(0.050);
    graphdiff_ADC_Layer1_nch_nominal->GetXaxis()->SetTitleOffset(1.1);
    graphdiff_ADC_Layer1_nch_nominal->GetXaxis()->CenterTitle();
    graphdiff_ADC_Layer1_nch_nominal->SetMarkerStyle(kFullCircle);
    graphdiff_ADC_Layer1_nch_nominal->SetMarkerSize(1.0);
    graphdiff_ADC_Layer1_nch_nominal->SetMarkerColor(kRed);
    graphdiff_ADC_Layer1_nch_nominal->SetLineColor(kRed);
    graphdiff_ADC_Layer1_nch_nominal->SetFillColor(0);

    // diff ADC_Layer2_nch nominal (3)
    TGraphErrors *graphdiff_ADC_Layer2_nch_nominal = new TGraphErrors(n_points,BetaGammaLayer2_nominal,diff_ADC_Layer2_nch,0,Err_diff_ADC_Layer2_nch);
    graphdiff_ADC_Layer2_nch_nominal->SetTitle("diff ADC_Layer2_nch nominal");
    graphdiff_ADC_Layer2_nch_nominal->GetYaxis()->SetLabelOffset(0.005);
    graphdiff_ADC_Layer2_nch_nominal->GetYaxis()->SetLabelSize(0.045);
    graphdiff_ADC_Layer2_nch_nominal->GetYaxis()->SetTitle("#frac{#LT#Delta E#GT - Data}{Data}");
    graphdiff_ADC_Layer2_nch_nominal->GetYaxis()->SetTitleSize(0.050);
    graphdiff_ADC_Layer2_nch_nominal->GetYaxis()->SetTitleOffset(1.25);
    graphdiff_ADC_Layer2_nch_nominal->GetYaxis()->CenterTitle();
    graphdiff_ADC_Layer2_nch_nominal->GetYaxis()->SetRangeUser(-0.3,0.2);
    graphdiff_ADC_Layer2_nch_nominal->GetXaxis()->SetLabelOffset(0.010);
    graphdiff_ADC_Layer2_nch_nominal->GetXaxis()->SetLabelSize(0.045);
    graphdiff_ADC_Layer2_nch_nominal->GetXaxis()->SetTitle("#beta#gamma_{nom}");
    graphdiff_ADC_Layer2_nch_nominal->GetXaxis()->SetTitleSize(0.050);
    graphdiff_ADC_Layer2_nch_nominal->GetXaxis()->SetTitleOffset(1.1);
    graphdiff_ADC_Layer2_nch_nominal->GetXaxis()->CenterTitle();
    graphdiff_ADC_Layer2_nch_nominal->SetMarkerStyle(kFullCircle);
    graphdiff_ADC_Layer2_nch_nominal->SetMarkerSize(1.0);
    graphdiff_ADC_Layer2_nch_nominal->SetMarkerColor(kBlue);
    graphdiff_ADC_Layer2_nch_nominal->SetLineColor(kBlue);
    graphdiff_ADC_Layer2_nch_nominal->SetFillColor(0);


    // Bethe ADC_ladder2_nch (3, 31, 5, 51)
    TGraphErrors *graphBetheADCdata_ladder2_nch = new TGraphErrors(n_points,BetaGammaLayer1_nominal,ADCdata_ladder2_nch,0,Err_ADCdata_ladder2_nch);
    graphBetheADCdata_ladder2_nch->SetTitle("Bethe ADC_ladder2_nch");
    graphBetheADCdata_ladder2_nch->GetYaxis()->SetLabelOffset(0.005);
    graphBetheADCdata_ladder2_nch->GetYaxis()->SetLabelSize(0.045);
    graphBetheADCdata_ladder2_nch->GetYaxis()->SetTitle("#LT#Delta E#GT [ADC]");
    graphBetheADCdata_ladder2_nch->GetYaxis()->SetTitleSize(0.050);
    graphBetheADCdata_ladder2_nch->GetYaxis()->SetTitleOffset(1.25);
    graphBetheADCdata_ladder2_nch->GetYaxis()->CenterTitle();
    graphBetheADCdata_ladder2_nch->GetXaxis()->SetLabelOffset(0.010);
    graphBetheADCdata_ladder2_nch->GetXaxis()->SetLabelSize(0.045);
    graphBetheADCdata_ladder2_nch->GetXaxis()->SetTitle("#beta#gamma_{nom}");
    graphBetheADCdata_ladder2_nch->GetXaxis()->SetTitleSize(0.050);
    graphBetheADCdata_ladder2_nch->GetXaxis()->SetTitleOffset(1.1);
    graphBetheADCdata_ladder2_nch->GetXaxis()->CenterTitle();
    graphBetheADCdata_ladder2_nch->SetMarkerStyle(kFullSquare);
    graphBetheADCdata_ladder2_nch->SetMarkerSize(1.0);
    graphBetheADCdata_ladder2_nch->SetMarkerColor(kGreen+2);
    graphBetheADCdata_ladder2_nch->SetLineColor(kGreen+2);
    graphBetheADCdata_ladder2_nch->SetFillColor(0);

    // Bethe ADC_ladder3_nch (3, 31, 5, 51)
    TGraphErrors *graphBetheADCdata_ladder3_nch = new TGraphErrors(n_points,BetaGammaLayer2_nominal,ADCdata_ladder3_nch,0,Err_ADCdata_ladder3_nch);
    graphBetheADCdata_ladder3_nch->SetTitle("Bethe ADC_ladder3_nch");
    graphBetheADCdata_ladder3_nch->GetYaxis()->SetLabelOffset(0.005);
    graphBetheADCdata_ladder3_nch->GetYaxis()->SetLabelSize(0.045);
    graphBetheADCdata_ladder3_nch->GetYaxis()->SetTitle("#LT#Delta E#GT [ADC]");
    graphBetheADCdata_ladder3_nch->GetYaxis()->SetTitleSize(0.050);
    graphBetheADCdata_ladder3_nch->GetYaxis()->SetTitleOffset(1.25);
    graphBetheADCdata_ladder3_nch->GetYaxis()->CenterTitle();
    graphBetheADCdata_ladder3_nch->GetXaxis()->SetLabelOffset(0.010);
    graphBetheADCdata_ladder3_nch->GetXaxis()->SetLabelSize(0.045);
    graphBetheADCdata_ladder3_nch->GetXaxis()->SetTitle("#beta#gamma_{nom}");
    graphBetheADCdata_ladder3_nch->GetXaxis()->SetTitleSize(0.050);
    graphBetheADCdata_ladder3_nch->GetXaxis()->SetTitleOffset(1.1);
    graphBetheADCdata_ladder3_nch->GetXaxis()->CenterTitle();
    graphBetheADCdata_ladder3_nch->SetMarkerStyle(kFullSquare);
    graphBetheADCdata_ladder3_nch->SetMarkerSize(1.0);
    graphBetheADCdata_ladder3_nch->SetMarkerColor(kOrange-3);
    graphBetheADCdata_ladder3_nch->SetLineColor(kOrange-3);
    graphBetheADCdata_ladder3_nch->SetFillColor(0);


    // Bethe ADC_Layer1_nch simulation (31)
    TGraphErrors *graphBetheADC_Layer1_nch_simulation = new TGraphErrors(n_points,BetaGammaLayer1,ADCLayer1_nch,0,Err_ADCLayer1_nch);
    graphBetheADC_Layer1_nch_simulation->SetTitle("Bethe ADC_Layer1_nch simulation");
    graphBetheADC_Layer1_nch_simulation->GetYaxis()->SetLabelOffset(0.005);
    graphBetheADC_Layer1_nch_simulation->GetYaxis()->SetLabelSize(0.045);
    graphBetheADC_Layer1_nch_simulation->GetYaxis()->SetTitle("#LT#Delta E#GT [ADC]");
    graphBetheADC_Layer1_nch_simulation->GetYaxis()->SetTitleSize(0.050);
    graphBetheADC_Layer1_nch_simulation->GetYaxis()->SetTitleOffset(1.25);
    graphBetheADC_Layer1_nch_simulation->GetYaxis()->CenterTitle();
    graphBetheADC_Layer1_nch_simulation->GetXaxis()->SetLabelOffset(0.010);
    graphBetheADC_Layer1_nch_simulation->GetXaxis()->SetLabelSize(0.045);
    graphBetheADC_Layer1_nch_simulation->GetXaxis()->SetTitle("#beta#gamma_{sim}");
    graphBetheADC_Layer1_nch_simulation->GetXaxis()->SetTitleSize(0.050);
    graphBetheADC_Layer1_nch_simulation->GetXaxis()->SetTitleOffset(1.1);
    graphBetheADC_Layer1_nch_simulation->GetXaxis()->CenterTitle();
    graphBetheADC_Layer1_nch_simulation->SetMarkerStyle(kFullCircle);
    graphBetheADC_Layer1_nch_simulation->SetMarkerSize(1.0);
    graphBetheADC_Layer1_nch_simulation->SetMarkerColor(kRed);
    graphBetheADC_Layer1_nch_simulation->SetLineColor(kRed);
    graphBetheADC_Layer1_nch_simulation->SetFillColor(0);

    // Bethe ADC_Layer2_nch simulation (31)
    TGraphErrors *graphBetheADC_Layer2_nch_simulation = new TGraphErrors(n_points,BetaGammaLayer2,ADCLayer2_nch,0,Err_ADCLayer2_nch);
    graphBetheADC_Layer2_nch_simulation->SetTitle("Bethe ADC_Layer2_nch simulation");
    graphBetheADC_Layer2_nch_simulation->GetYaxis()->SetLabelOffset(0.005);
    graphBetheADC_Layer2_nch_simulation->GetYaxis()->SetLabelSize(0.045);
    graphBetheADC_Layer2_nch_simulation->GetYaxis()->SetTitle("#LT#Delta E#GT [ADC]");
    graphBetheADC_Layer2_nch_simulation->GetYaxis()->SetTitleSize(0.050);
    graphBetheADC_Layer2_nch_simulation->GetYaxis()->SetTitleOffset(1.25);
    graphBetheADC_Layer2_nch_simulation->GetYaxis()->CenterTitle();
    graphBetheADC_Layer2_nch_simulation->GetXaxis()->SetLabelOffset(0.010);
    graphBetheADC_Layer2_nch_simulation->GetXaxis()->SetLabelSize(0.045);
    graphBetheADC_Layer2_nch_simulation->GetXaxis()->SetTitle("#beta#gamma_{sim}");
    graphBetheADC_Layer2_nch_simulation->GetXaxis()->SetTitleSize(0.050);
    graphBetheADC_Layer2_nch_simulation->GetXaxis()->SetTitleOffset(1.1);
    graphBetheADC_Layer2_nch_simulation->GetXaxis()->CenterTitle();
    graphBetheADC_Layer2_nch_simulation->SetMarkerStyle(kFullCircle);
    graphBetheADC_Layer2_nch_simulation->SetMarkerSize(1.0);
    graphBetheADC_Layer2_nch_simulation->SetMarkerColor(kBlue);
    graphBetheADC_Layer2_nch_simulation->SetLineColor(kBlue);
    graphBetheADC_Layer2_nch_simulation->SetFillColor(0);

    // diff ADC_Layer1_nch simulation (31)
    TGraphErrors *graphdiff_ADC_Layer1_nch_simulation = new TGraphErrors(n_points,BetaGammaLayer1,diff_ADC_Layer1_nch,0,Err_diff_ADC_Layer1_nch);
    graphdiff_ADC_Layer1_nch_simulation->SetTitle("diff ADC_Layer1_nch simulation");
    graphdiff_ADC_Layer1_nch_simulation->GetYaxis()->SetLabelOffset(0.005);
    graphdiff_ADC_Layer1_nch_simulation->GetYaxis()->SetLabelSize(0.045);
    graphdiff_ADC_Layer1_nch_simulation->GetYaxis()->SetTitle("#frac{#LT#Delta E#GT - Data}{Data}");
    graphdiff_ADC_Layer1_nch_simulation->GetYaxis()->SetTitleSize(0.050);
    graphdiff_ADC_Layer1_nch_simulation->GetYaxis()->SetTitleOffset(1.25);
    graphdiff_ADC_Layer1_nch_simulation->GetYaxis()->CenterTitle();
    graphdiff_ADC_Layer1_nch_simulation->GetYaxis()->SetRangeUser(-0.3,0.2);
    graphdiff_ADC_Layer1_nch_simulation->GetXaxis()->SetLabelOffset(0.010);
    graphdiff_ADC_Layer1_nch_simulation->GetXaxis()->SetLabelSize(0.045);
    graphdiff_ADC_Layer1_nch_simulation->GetXaxis()->SetTitle("#beta#gamma_{sim}");
    graphdiff_ADC_Layer1_nch_simulation->GetXaxis()->SetTitleSize(0.050);
    graphdiff_ADC_Layer1_nch_simulation->GetXaxis()->SetTitleOffset(1.1);
    graphdiff_ADC_Layer1_nch_simulation->GetXaxis()->CenterTitle();
    graphdiff_ADC_Layer1_nch_simulation->SetMarkerStyle(kFullCircle);
    graphdiff_ADC_Layer1_nch_simulation->SetMarkerSize(1.0);
    graphdiff_ADC_Layer1_nch_simulation->SetMarkerColor(kRed);
    graphdiff_ADC_Layer1_nch_simulation->SetLineColor(kRed);
    graphdiff_ADC_Layer1_nch_simulation->SetFillColor(0);

    // diff ADC_Layer1_nch simulation (31)
    TGraphErrors *graphdiff_ADC_Layer2_nch_simulation = new TGraphErrors(n_points,BetaGammaLayer2,diff_ADC_Layer2_nch,0,Err_diff_ADC_Layer2_nch);
    graphdiff_ADC_Layer2_nch_simulation->SetTitle("diff ADC_Layer2_nch simulation");
    graphdiff_ADC_Layer2_nch_simulation->GetYaxis()->SetLabelOffset(0.005);
    graphdiff_ADC_Layer2_nch_simulation->GetYaxis()->SetLabelSize(0.045);
    graphdiff_ADC_Layer2_nch_simulation->GetYaxis()->SetTitle("#frac{#LT#Delta E#GT - Data}{Data}");
    graphdiff_ADC_Layer2_nch_simulation->GetYaxis()->SetTitleSize(0.050);
    graphdiff_ADC_Layer2_nch_simulation->GetYaxis()->SetTitleOffset(1.25);
    graphdiff_ADC_Layer2_nch_simulation->GetYaxis()->CenterTitle();
    graphdiff_ADC_Layer2_nch_simulation->GetYaxis()->SetRangeUser(-0.3,0.2);
    graphdiff_ADC_Layer2_nch_simulation->GetXaxis()->SetLabelOffset(0.010);
    graphdiff_ADC_Layer2_nch_simulation->GetXaxis()->SetLabelSize(0.045);
    graphdiff_ADC_Layer2_nch_simulation->GetXaxis()->SetTitle("#beta#gamma_{sim}");
    graphdiff_ADC_Layer2_nch_simulation->GetXaxis()->SetTitleSize(0.050);
    graphdiff_ADC_Layer2_nch_simulation->GetXaxis()->SetTitleOffset(1.1);
    graphdiff_ADC_Layer2_nch_simulation->GetXaxis()->CenterTitle();
    graphdiff_ADC_Layer2_nch_simulation->SetMarkerStyle(kFullCircle);
    graphdiff_ADC_Layer2_nch_simulation->SetMarkerSize(1.0);
    graphdiff_ADC_Layer2_nch_simulation->SetMarkerColor(kBlue);
    graphdiff_ADC_Layer2_nch_simulation->SetLineColor(kBlue);
    graphdiff_ADC_Layer2_nch_simulation->SetFillColor(0);



    // NOISE
    // Bethe ADC_Layer1_pch_noise nominal (4)
    TGraphErrors *graphBetheADC_Layer1_pch_noise_nominal = new TGraphErrors(n_points,BetaGammaLayer1_nominal,ADCLayer1_pch_noise,0,Err_ADCLayer1_pch_noise);
    graphBetheADC_Layer1_pch_noise_nominal->SetTitle("Bethe ADC_Layer1_pch_noise nominal");
    graphBetheADC_Layer1_pch_noise_nominal->GetYaxis()->SetLabelOffset(0.005);
    graphBetheADC_Layer1_pch_noise_nominal->GetYaxis()->SetLabelSize(0.045);
    graphBetheADC_Layer1_pch_noise_nominal->GetYaxis()->SetTitle("#LT#Delta E#GT [ADC]");
    graphBetheADC_Layer1_pch_noise_nominal->GetYaxis()->SetTitleSize(0.050);
    graphBetheADC_Layer1_pch_noise_nominal->GetYaxis()->SetTitleOffset(1.25);
    graphBetheADC_Layer1_pch_noise_nominal->GetYaxis()->CenterTitle();
    graphBetheADC_Layer1_pch_noise_nominal->GetXaxis()->SetLabelOffset(0.010);
    graphBetheADC_Layer1_pch_noise_nominal->GetXaxis()->SetLabelSize(0.045);
    graphBetheADC_Layer1_pch_noise_nominal->GetXaxis()->SetTitle("#beta#gamma_{nom}");
    graphBetheADC_Layer1_pch_noise_nominal->GetXaxis()->SetTitleSize(0.050);
    graphBetheADC_Layer1_pch_noise_nominal->GetXaxis()->SetTitleOffset(1.1);
    graphBetheADC_Layer1_pch_noise_nominal->GetXaxis()->CenterTitle();
    graphBetheADC_Layer1_pch_noise_nominal->SetMarkerStyle(kFullCircle);
    graphBetheADC_Layer1_pch_noise_nominal->SetMarkerSize(1.0);
    graphBetheADC_Layer1_pch_noise_nominal->SetMarkerColor(kRed);
    graphBetheADC_Layer1_pch_noise_nominal->SetLineColor(kRed);
    graphBetheADC_Layer1_pch_noise_nominal->SetFillColor(0);

    // Bethe ADC_Layer2_pch_noise nominal (4)
    TGraphErrors *graphBetheADC_Layer2_pch_noise_nominal = new TGraphErrors(n_points,BetaGammaLayer2_nominal,ADCLayer2_pch_noise,0,Err_ADCLayer2_pch_noise);
    graphBetheADC_Layer2_pch_noise_nominal->SetTitle("Bethe ADC_Layer2_pch_noise nominal");
    graphBetheADC_Layer2_pch_noise_nominal->GetYaxis()->SetLabelOffset(0.005);
    graphBetheADC_Layer2_pch_noise_nominal->GetYaxis()->SetLabelSize(0.045);
    graphBetheADC_Layer2_pch_noise_nominal->GetYaxis()->SetTitle("#LT#Delta E#GT [ADC]");
    graphBetheADC_Layer2_pch_noise_nominal->GetYaxis()->SetTitleSize(0.050);
    graphBetheADC_Layer2_pch_noise_nominal->GetYaxis()->SetTitleOffset(1.25);
    graphBetheADC_Layer2_pch_noise_nominal->GetYaxis()->CenterTitle();
    graphBetheADC_Layer2_pch_noise_nominal->GetXaxis()->SetLabelOffset(0.010);
    graphBetheADC_Layer2_pch_noise_nominal->GetXaxis()->SetLabelSize(0.045);
    graphBetheADC_Layer2_pch_noise_nominal->GetXaxis()->SetTitle("#beta#gamma_{nom}");
    graphBetheADC_Layer2_pch_noise_nominal->GetXaxis()->SetTitleSize(0.050);
    graphBetheADC_Layer2_pch_noise_nominal->GetXaxis()->SetTitleOffset(1.1);
    graphBetheADC_Layer2_pch_noise_nominal->GetXaxis()->CenterTitle();
    graphBetheADC_Layer2_pch_noise_nominal->SetMarkerStyle(kFullCircle);
    graphBetheADC_Layer2_pch_noise_nominal->SetMarkerSize(1.0);
    graphBetheADC_Layer2_pch_noise_nominal->SetMarkerColor(kBlue);
    graphBetheADC_Layer2_pch_noise_nominal->SetLineColor(kBlue);
    graphBetheADC_Layer2_pch_noise_nominal->SetFillColor(0);

    // diff ADC_Layer1_pch_noise nominal (4)
    TGraphErrors *graphdiff_ADC_Layer1_pch_noise_nominal = new TGraphErrors(n_points,BetaGammaLayer1_nominal,diff_ADC_Layer1_pch_noise,0,Err_diff_ADC_Layer1_pch_noise);
    graphdiff_ADC_Layer1_pch_noise_nominal->SetTitle("diff ADC_Layer1_pch_noise nominal");
    graphdiff_ADC_Layer1_pch_noise_nominal->GetYaxis()->SetLabelOffset(0.005);
    graphdiff_ADC_Layer1_pch_noise_nominal->GetYaxis()->SetLabelSize(0.045);
    graphdiff_ADC_Layer1_pch_noise_nominal->GetYaxis()->SetTitle("#frac{#LT#Delta E#GT - Data}{Data}");
    graphdiff_ADC_Layer1_pch_noise_nominal->GetYaxis()->SetTitleSize(0.050);
    graphdiff_ADC_Layer1_pch_noise_nominal->GetYaxis()->SetTitleOffset(1.25);
    graphdiff_ADC_Layer1_pch_noise_nominal->GetYaxis()->CenterTitle();
    graphdiff_ADC_Layer1_pch_noise_nominal->GetYaxis()->SetRangeUser(-0.3,0.2);
    graphdiff_ADC_Layer1_pch_noise_nominal->GetXaxis()->SetLabelOffset(0.010);
    graphdiff_ADC_Layer1_pch_noise_nominal->GetXaxis()->SetLabelSize(0.045);
    graphdiff_ADC_Layer1_pch_noise_nominal->GetXaxis()->SetTitle("#beta#gamma_{nom}");
    graphdiff_ADC_Layer1_pch_noise_nominal->GetXaxis()->SetTitleSize(0.050);
    graphdiff_ADC_Layer1_pch_noise_nominal->GetXaxis()->SetTitleOffset(1.1);
    graphdiff_ADC_Layer1_pch_noise_nominal->GetXaxis()->CenterTitle();
    graphdiff_ADC_Layer1_pch_noise_nominal->SetMarkerStyle(kFullCircle);
    graphdiff_ADC_Layer1_pch_noise_nominal->SetMarkerSize(1.0);
    graphdiff_ADC_Layer1_pch_noise_nominal->SetMarkerColor(kRed);
    graphdiff_ADC_Layer1_pch_noise_nominal->SetLineColor(kRed);
    graphdiff_ADC_Layer1_pch_noise_nominal->SetFillColor(0);

    // diff ADC_Layer2_pch_noise nominal (4)
    TGraphErrors *graphdiff_ADC_Layer2_pch_noise_nominal = new TGraphErrors(n_points,BetaGammaLayer2_nominal,diff_ADC_Layer2_pch_noise,0,Err_diff_ADC_Layer2_pch_noise);
    graphdiff_ADC_Layer2_pch_noise_nominal->SetTitle("diff ADC_Layer2_pch_noise nominal");
    graphdiff_ADC_Layer2_pch_noise_nominal->GetYaxis()->SetLabelOffset(0.005);
    graphdiff_ADC_Layer2_pch_noise_nominal->GetYaxis()->SetLabelSize(0.045);
    graphdiff_ADC_Layer2_pch_noise_nominal->GetYaxis()->SetTitle("#frac{#LT#Delta E#GT - Data}{Data}");
    graphdiff_ADC_Layer2_pch_noise_nominal->GetYaxis()->SetTitleSize(0.050);
    graphdiff_ADC_Layer2_pch_noise_nominal->GetYaxis()->SetTitleOffset(1.25);
    graphdiff_ADC_Layer2_pch_noise_nominal->GetYaxis()->CenterTitle();
    graphdiff_ADC_Layer2_pch_noise_nominal->GetYaxis()->SetRangeUser(-0.3,0.2);
    graphdiff_ADC_Layer2_pch_noise_nominal->GetXaxis()->SetLabelOffset(0.010);
    graphdiff_ADC_Layer2_pch_noise_nominal->GetXaxis()->SetLabelSize(0.045);
    graphdiff_ADC_Layer2_pch_noise_nominal->GetXaxis()->SetTitle("#beta#gamma_{nom}");
    graphdiff_ADC_Layer2_pch_noise_nominal->GetXaxis()->SetTitleSize(0.050);
    graphdiff_ADC_Layer2_pch_noise_nominal->GetXaxis()->SetTitleOffset(1.1);
    graphdiff_ADC_Layer2_pch_noise_nominal->GetXaxis()->CenterTitle();
    graphdiff_ADC_Layer2_pch_noise_nominal->SetMarkerStyle(kFullCircle);
    graphdiff_ADC_Layer2_pch_noise_nominal->SetMarkerSize(1.0);
    graphdiff_ADC_Layer2_pch_noise_nominal->SetMarkerColor(kBlue);
    graphdiff_ADC_Layer2_pch_noise_nominal->SetLineColor(kBlue);
    graphdiff_ADC_Layer2_pch_noise_nominal->SetFillColor(0);


    // Bethe ADC_Layer1_pch_noise simulation (41)
    TGraphErrors *graphBetheADC_Layer1_pch_noise_simulation = new TGraphErrors(n_points,BetaGammaLayer1,ADCLayer1_pch_noise,0,Err_ADCLayer1_pch_noise);
    graphBetheADC_Layer1_pch_noise_simulation->SetTitle("Bethe ADC_Layer1_pch_noise simulation");
    graphBetheADC_Layer1_pch_noise_simulation->GetYaxis()->SetLabelOffset(0.005);
    graphBetheADC_Layer1_pch_noise_simulation->GetYaxis()->SetLabelSize(0.045);
    graphBetheADC_Layer1_pch_noise_simulation->GetYaxis()->SetTitle("#LT#Delta E#GT [ADC]");
    graphBetheADC_Layer1_pch_noise_simulation->GetYaxis()->SetTitleSize(0.050);
    graphBetheADC_Layer1_pch_noise_simulation->GetYaxis()->SetTitleOffset(1.25);
    graphBetheADC_Layer1_pch_noise_simulation->GetYaxis()->CenterTitle();
    graphBetheADC_Layer1_pch_noise_simulation->GetXaxis()->SetLabelOffset(0.010);
    graphBetheADC_Layer1_pch_noise_simulation->GetXaxis()->SetLabelSize(0.045);
    graphBetheADC_Layer1_pch_noise_simulation->GetXaxis()->SetTitle("#beta#gamma_{sim}");
    graphBetheADC_Layer1_pch_noise_simulation->GetXaxis()->SetTitleSize(0.050);
    graphBetheADC_Layer1_pch_noise_simulation->GetXaxis()->SetTitleOffset(1.1);
    graphBetheADC_Layer1_pch_noise_simulation->GetXaxis()->CenterTitle();
    graphBetheADC_Layer1_pch_noise_simulation->SetMarkerStyle(kFullCircle);
    graphBetheADC_Layer1_pch_noise_simulation->SetMarkerSize(1.0);
    graphBetheADC_Layer1_pch_noise_simulation->SetMarkerColor(kRed);
    graphBetheADC_Layer1_pch_noise_simulation->SetLineColor(kRed);
    graphBetheADC_Layer1_pch_noise_simulation->SetFillColor(0);

    // Bethe ADC_Layer2_pch_noise simulation (41)
    TGraphErrors *graphBetheADC_Layer2_pch_noise_simulation = new TGraphErrors(n_points,BetaGammaLayer2,ADCLayer2_pch_noise,0,Err_ADCLayer2_pch_noise);
    graphBetheADC_Layer2_pch_noise_simulation->SetTitle("Bethe ADC_Layer2_pch_noise simulation");
    graphBetheADC_Layer2_pch_noise_simulation->GetYaxis()->SetLabelOffset(0.005);
    graphBetheADC_Layer2_pch_noise_simulation->GetYaxis()->SetLabelSize(0.045);
    graphBetheADC_Layer2_pch_noise_simulation->GetYaxis()->SetTitle("#LT#Delta E#GT [ADC]");
    graphBetheADC_Layer2_pch_noise_simulation->GetYaxis()->SetTitleSize(0.050);
    graphBetheADC_Layer2_pch_noise_simulation->GetYaxis()->SetTitleOffset(1.25);
    graphBetheADC_Layer2_pch_noise_simulation->GetYaxis()->CenterTitle();
    graphBetheADC_Layer2_pch_noise_simulation->GetXaxis()->SetLabelOffset(0.010);
    graphBetheADC_Layer2_pch_noise_simulation->GetXaxis()->SetLabelSize(0.045);
    graphBetheADC_Layer2_pch_noise_simulation->GetXaxis()->SetTitle("#beta#gamma_{sim}");
    graphBetheADC_Layer2_pch_noise_simulation->GetXaxis()->SetTitleSize(0.050);
    graphBetheADC_Layer2_pch_noise_simulation->GetXaxis()->SetTitleOffset(1.1);
    graphBetheADC_Layer2_pch_noise_simulation->GetXaxis()->CenterTitle();
    graphBetheADC_Layer2_pch_noise_simulation->SetMarkerStyle(kFullCircle);
    graphBetheADC_Layer2_pch_noise_simulation->SetMarkerSize(1.0);
    graphBetheADC_Layer2_pch_noise_simulation->SetMarkerColor(kBlue);
    graphBetheADC_Layer2_pch_noise_simulation->SetLineColor(kBlue);
    graphBetheADC_Layer2_pch_noise_simulation->SetFillColor(0);

    // diff ADC_Layer1_pch_noise simulation (41)
    TGraphErrors *graphdiff_ADC_Layer1_pch_noise_simulation = new TGraphErrors(n_points,BetaGammaLayer1,diff_ADC_Layer1_pch_noise,0,Err_diff_ADC_Layer1_pch_noise); // add err_diff!
    graphdiff_ADC_Layer1_pch_noise_simulation->SetTitle("diff ADC_Layer1_pch_noise simulation");
    graphdiff_ADC_Layer1_pch_noise_simulation->GetYaxis()->SetLabelOffset(0.005);
    graphdiff_ADC_Layer1_pch_noise_simulation->GetYaxis()->SetLabelSize(0.045);
    graphdiff_ADC_Layer1_pch_noise_simulation->GetYaxis()->SetTitle("#frac{#LT#Delta E#GT - Data}{Data}");
    graphdiff_ADC_Layer1_pch_noise_simulation->GetYaxis()->SetTitleSize(0.050);
    graphdiff_ADC_Layer1_pch_noise_simulation->GetYaxis()->SetTitleOffset(1.25);
    graphdiff_ADC_Layer1_pch_noise_simulation->GetYaxis()->CenterTitle();
    graphdiff_ADC_Layer1_pch_noise_simulation->GetYaxis()->SetRangeUser(-0.3,0.2);
    graphdiff_ADC_Layer1_pch_noise_simulation->GetXaxis()->SetLabelOffset(0.010);
    graphdiff_ADC_Layer1_pch_noise_simulation->GetXaxis()->SetLabelSize(0.045);
    graphdiff_ADC_Layer1_pch_noise_simulation->GetXaxis()->SetTitle("#beta#gamma_{sim}");
    graphdiff_ADC_Layer1_pch_noise_simulation->GetXaxis()->SetTitleSize(0.050);
    graphdiff_ADC_Layer1_pch_noise_simulation->GetXaxis()->SetTitleOffset(1.1);
    graphdiff_ADC_Layer1_pch_noise_simulation->GetXaxis()->CenterTitle();
    graphdiff_ADC_Layer1_pch_noise_simulation->SetMarkerStyle(kFullCircle);
    graphdiff_ADC_Layer1_pch_noise_simulation->SetMarkerSize(1.0);
    graphdiff_ADC_Layer1_pch_noise_simulation->SetMarkerColor(kRed);
    graphdiff_ADC_Layer1_pch_noise_simulation->SetLineColor(kRed);
    graphdiff_ADC_Layer1_pch_noise_simulation->SetFillColor(0);

    // diff ADC_Layer1_pch_noise simulation (41)
    TGraphErrors *graphdiff_ADC_Layer2_pch_noise_simulation = new TGraphErrors(n_points,BetaGammaLayer2,diff_ADC_Layer2_pch_noise,0,Err_diff_ADC_Layer2_pch_noise);
    graphdiff_ADC_Layer2_pch_noise_simulation->SetTitle("diff ADC_Layer2_pch_noise simulation");
    graphdiff_ADC_Layer2_pch_noise_simulation->GetYaxis()->SetLabelOffset(0.005);
    graphdiff_ADC_Layer2_pch_noise_simulation->GetYaxis()->SetLabelSize(0.045);
    graphdiff_ADC_Layer2_pch_noise_simulation->GetYaxis()->SetTitle("#frac{#LT#Delta E#GT - Data}{Data}");
    graphdiff_ADC_Layer2_pch_noise_simulation->GetYaxis()->SetTitleSize(0.050);
    graphdiff_ADC_Layer2_pch_noise_simulation->GetYaxis()->SetTitleOffset(1.25);
    graphdiff_ADC_Layer2_pch_noise_simulation->GetYaxis()->CenterTitle();
    graphdiff_ADC_Layer2_pch_noise_simulation->GetYaxis()->SetRangeUser(-0.3,0.2);
    graphdiff_ADC_Layer2_pch_noise_simulation->GetXaxis()->SetLabelOffset(0.010);
    graphdiff_ADC_Layer2_pch_noise_simulation->GetXaxis()->SetLabelSize(0.045);
    graphdiff_ADC_Layer2_pch_noise_simulation->GetXaxis()->SetTitle("#beta#gamma_{sim}");
    graphdiff_ADC_Layer2_pch_noise_simulation->GetXaxis()->SetTitleSize(0.050);
    graphdiff_ADC_Layer2_pch_noise_simulation->GetXaxis()->SetTitleOffset(1.1);
    graphdiff_ADC_Layer2_pch_noise_simulation->GetXaxis()->CenterTitle();
    graphdiff_ADC_Layer2_pch_noise_simulation->SetMarkerStyle(kFullCircle);
    graphdiff_ADC_Layer2_pch_noise_simulation->SetMarkerSize(1.0);
    graphdiff_ADC_Layer2_pch_noise_simulation->SetMarkerColor(kBlue);
    graphdiff_ADC_Layer2_pch_noise_simulation->SetLineColor(kBlue);
    graphdiff_ADC_Layer2_pch_noise_simulation->SetFillColor(0);


    // Bethe ADC_Layer1_nch_noise nominal (5)
    TGraphErrors *graphBetheADC_Layer1_nch_noise_nominal = new TGraphErrors(n_points,BetaGammaLayer1_nominal,ADCLayer1_nch_noise,0,Err_ADCLayer1_nch_noise);
    graphBetheADC_Layer1_nch_noise_nominal->SetTitle("Bethe ADC_Layer1_nch_noise nominal");
    graphBetheADC_Layer1_nch_noise_nominal->GetYaxis()->SetLabelOffset(0.005);
    graphBetheADC_Layer1_nch_noise_nominal->GetYaxis()->SetLabelSize(0.045);
    graphBetheADC_Layer1_nch_noise_nominal->GetYaxis()->SetTitle("#LT#Delta E#GT [ADC]");
    graphBetheADC_Layer1_nch_noise_nominal->GetYaxis()->SetTitleSize(0.050);
    graphBetheADC_Layer1_nch_noise_nominal->GetYaxis()->SetTitleOffset(1.25);
    graphBetheADC_Layer1_nch_noise_nominal->GetYaxis()->CenterTitle();
    graphBetheADC_Layer1_nch_noise_nominal->GetXaxis()->SetLabelOffset(0.010);
    graphBetheADC_Layer1_nch_noise_nominal->GetXaxis()->SetLabelSize(0.045);
    graphBetheADC_Layer1_nch_noise_nominal->GetXaxis()->SetTitle("#beta#gamma_{nom}");
    graphBetheADC_Layer1_nch_noise_nominal->GetXaxis()->SetTitleSize(0.050);
    graphBetheADC_Layer1_nch_noise_nominal->GetXaxis()->SetTitleOffset(1.1);
    graphBetheADC_Layer1_nch_noise_nominal->GetXaxis()->CenterTitle();
    graphBetheADC_Layer1_nch_noise_nominal->SetMarkerStyle(kFullCircle);
    graphBetheADC_Layer1_nch_noise_nominal->SetMarkerSize(1.0);
    graphBetheADC_Layer1_nch_noise_nominal->SetMarkerColor(kRed);
    graphBetheADC_Layer1_nch_noise_nominal->SetLineColor(kRed);
    graphBetheADC_Layer1_nch_noise_nominal->SetFillColor(0);

    // Bethe ADC_Layer2_nch_noise nominal (5)
    TGraphErrors *graphBetheADC_Layer2_nch_noise_nominal = new TGraphErrors(n_points,BetaGammaLayer2_nominal,ADCLayer2_nch_noise,0,Err_ADCLayer2_nch_noise);
    graphBetheADC_Layer2_nch_noise_nominal->SetTitle("Bethe ADC_Layer2_nch_noise nominal");
    graphBetheADC_Layer2_nch_noise_nominal->GetYaxis()->SetLabelOffset(0.005);
    graphBetheADC_Layer2_nch_noise_nominal->GetYaxis()->SetLabelSize(0.045);
    graphBetheADC_Layer2_nch_noise_nominal->GetYaxis()->SetTitle("#LT#Delta E#GT [ADC]");
    graphBetheADC_Layer2_nch_noise_nominal->GetYaxis()->SetTitleSize(0.050);
    graphBetheADC_Layer2_nch_noise_nominal->GetYaxis()->SetTitleOffset(1.25);
    graphBetheADC_Layer2_nch_noise_nominal->GetYaxis()->CenterTitle();
    graphBetheADC_Layer2_nch_noise_nominal->GetXaxis()->SetLabelOffset(0.010);
    graphBetheADC_Layer2_nch_noise_nominal->GetXaxis()->SetLabelSize(0.045);
    graphBetheADC_Layer2_nch_noise_nominal->GetXaxis()->SetTitle("#beta#gamma_{nom}");
    graphBetheADC_Layer2_nch_noise_nominal->GetXaxis()->SetTitleSize(0.050);
    graphBetheADC_Layer2_nch_noise_nominal->GetXaxis()->SetTitleOffset(1.1);
    graphBetheADC_Layer2_nch_noise_nominal->GetXaxis()->CenterTitle();
    graphBetheADC_Layer2_nch_noise_nominal->SetMarkerStyle(kFullCircle);
    graphBetheADC_Layer2_nch_noise_nominal->SetMarkerSize(1.0);
    graphBetheADC_Layer2_nch_noise_nominal->SetMarkerColor(kBlue);
    graphBetheADC_Layer2_nch_noise_nominal->SetLineColor(kBlue);
    graphBetheADC_Layer2_nch_noise_nominal->SetFillColor(0);

    // diff ADC_Layer1_nch_noise nominal (5)
    TGraphErrors *graphdiff_ADC_Layer1_nch_noise_nominal = new TGraphErrors(n_points,BetaGammaLayer1_nominal,diff_ADC_Layer1_nch_noise,0,Err_diff_ADC_Layer1_nch_noise);
    graphdiff_ADC_Layer1_nch_noise_nominal->SetTitle("diff ADC_Layer1_nch_noise nominal");
    graphdiff_ADC_Layer1_nch_noise_nominal->GetYaxis()->SetLabelOffset(0.005);
    graphdiff_ADC_Layer1_nch_noise_nominal->GetYaxis()->SetLabelSize(0.045);
    graphdiff_ADC_Layer1_nch_noise_nominal->GetYaxis()->SetTitle("#frac{#LT#Delta E#GT - Data}{Data}");
    graphdiff_ADC_Layer1_nch_noise_nominal->GetYaxis()->SetTitleSize(0.050);
    graphdiff_ADC_Layer1_nch_noise_nominal->GetYaxis()->SetTitleOffset(1.25);
    graphdiff_ADC_Layer1_nch_noise_nominal->GetYaxis()->CenterTitle();
    graphdiff_ADC_Layer1_nch_noise_nominal->GetYaxis()->SetRangeUser(-0.3,0.2);
    graphdiff_ADC_Layer1_nch_noise_nominal->GetXaxis()->SetLabelOffset(0.010);
    graphdiff_ADC_Layer1_nch_noise_nominal->GetXaxis()->SetLabelSize(0.045);
    graphdiff_ADC_Layer1_nch_noise_nominal->GetXaxis()->SetTitle("#beta#gamma_{nom}");
    graphdiff_ADC_Layer1_nch_noise_nominal->GetXaxis()->SetTitleSize(0.050);
    graphdiff_ADC_Layer1_nch_noise_nominal->GetXaxis()->SetTitleOffset(1.1);
    graphdiff_ADC_Layer1_nch_noise_nominal->GetXaxis()->CenterTitle();
    graphdiff_ADC_Layer1_nch_noise_nominal->SetMarkerStyle(kFullCircle);
    graphdiff_ADC_Layer1_nch_noise_nominal->SetMarkerSize(1.0);
    graphdiff_ADC_Layer1_nch_noise_nominal->SetMarkerColor(kRed);
    graphdiff_ADC_Layer1_nch_noise_nominal->SetLineColor(kRed);
    graphdiff_ADC_Layer1_nch_noise_nominal->SetFillColor(0);

    // diff ADC_Layer2_nch_noise nominal (5)
    TGraphErrors *graphdiff_ADC_Layer2_nch_noise_nominal = new TGraphErrors(n_points,BetaGammaLayer2_nominal,diff_ADC_Layer2_nch_noise,0,Err_diff_ADC_Layer2_nch_noise);
    graphdiff_ADC_Layer2_nch_noise_nominal->SetTitle("diff ADC_Layer2_nch_noise nominal");
    graphdiff_ADC_Layer2_nch_noise_nominal->GetYaxis()->SetLabelOffset(0.005);
    graphdiff_ADC_Layer2_nch_noise_nominal->GetYaxis()->SetLabelSize(0.045);
    graphdiff_ADC_Layer2_nch_noise_nominal->GetYaxis()->SetTitle("#frac{#LT#Delta E#GT - Data}{Data}");
    graphdiff_ADC_Layer2_nch_noise_nominal->GetYaxis()->SetTitleSize(0.050);
    graphdiff_ADC_Layer2_nch_noise_nominal->GetYaxis()->SetTitleOffset(1.25);
    graphdiff_ADC_Layer2_nch_noise_nominal->GetYaxis()->CenterTitle();
    graphdiff_ADC_Layer2_nch_noise_nominal->GetYaxis()->SetRangeUser(-0.3,0.2);
    graphdiff_ADC_Layer2_nch_noise_nominal->GetXaxis()->SetLabelOffset(0.010);
    graphdiff_ADC_Layer2_nch_noise_nominal->GetXaxis()->SetLabelSize(0.045);
    graphdiff_ADC_Layer2_nch_noise_nominal->GetXaxis()->SetTitle("#beta#gamma_{nom}");
    graphdiff_ADC_Layer2_nch_noise_nominal->GetXaxis()->SetTitleSize(0.050);
    graphdiff_ADC_Layer2_nch_noise_nominal->GetXaxis()->SetTitleOffset(1.1);
    graphdiff_ADC_Layer2_nch_noise_nominal->GetXaxis()->CenterTitle();
    graphdiff_ADC_Layer2_nch_noise_nominal->SetMarkerStyle(kFullCircle);
    graphdiff_ADC_Layer2_nch_noise_nominal->SetMarkerSize(1.0);
    graphdiff_ADC_Layer2_nch_noise_nominal->SetMarkerColor(kBlue);
    graphdiff_ADC_Layer2_nch_noise_nominal->SetLineColor(kBlue);
    graphdiff_ADC_Layer2_nch_noise_nominal->SetFillColor(0);


    // Bethe ADC_Layer1_nch_noise simulation (51)
    TGraphErrors *graphBetheADC_Layer1_nch_noise_simulation = new TGraphErrors(n_points,BetaGammaLayer1,ADCLayer1_nch_noise,0,Err_ADCLayer1_nch_noise);
    graphBetheADC_Layer1_nch_noise_simulation->SetTitle("Bethe ADC_Layer1_nch_noise simulation");
    graphBetheADC_Layer1_nch_noise_simulation->GetYaxis()->SetLabelOffset(0.005);
    graphBetheADC_Layer1_nch_noise_simulation->GetYaxis()->SetLabelSize(0.045);
    graphBetheADC_Layer1_nch_noise_simulation->GetYaxis()->SetTitle("#LT#Delta E#GT [ADC]");
    graphBetheADC_Layer1_nch_noise_simulation->GetYaxis()->SetTitleSize(0.050);
    graphBetheADC_Layer1_nch_noise_simulation->GetYaxis()->SetTitleOffset(1.25);
    graphBetheADC_Layer1_nch_noise_simulation->GetYaxis()->CenterTitle();
    graphBetheADC_Layer1_nch_noise_simulation->GetXaxis()->SetLabelOffset(0.010);
    graphBetheADC_Layer1_nch_noise_simulation->GetXaxis()->SetLabelSize(0.045);
    graphBetheADC_Layer1_nch_noise_simulation->GetXaxis()->SetTitle("#beta#gamma_{sim}");
    graphBetheADC_Layer1_nch_noise_simulation->GetXaxis()->SetTitleSize(0.050);
    graphBetheADC_Layer1_nch_noise_simulation->GetXaxis()->SetTitleOffset(1.1);
    graphBetheADC_Layer1_nch_noise_simulation->GetXaxis()->CenterTitle();
    graphBetheADC_Layer1_nch_noise_simulation->SetMarkerStyle(kFullCircle);
    graphBetheADC_Layer1_nch_noise_simulation->SetMarkerSize(1.0);
    graphBetheADC_Layer1_nch_noise_simulation->SetMarkerColor(kRed);
    graphBetheADC_Layer1_nch_noise_simulation->SetLineColor(kRed);
    graphBetheADC_Layer1_nch_noise_simulation->SetFillColor(0);

    // Bethe ADC_Layer2_nch_noise simulation (51)
    TGraphErrors *graphBetheADC_Layer2_nch_noise_simulation = new TGraphErrors(n_points,BetaGammaLayer2,ADCLayer2_nch_noise,0,Err_ADCLayer2_nch_noise);
    graphBetheADC_Layer2_nch_noise_simulation->SetTitle("Bethe ADC_Layer2_nch_noise simulation");
    graphBetheADC_Layer2_nch_noise_simulation->GetYaxis()->SetLabelOffset(0.005);
    graphBetheADC_Layer2_nch_noise_simulation->GetYaxis()->SetLabelSize(0.045);
    graphBetheADC_Layer2_nch_noise_simulation->GetYaxis()->SetTitle("#LT#Delta E#GT [ADC]");
    graphBetheADC_Layer2_nch_noise_simulation->GetYaxis()->SetTitleSize(0.050);
    graphBetheADC_Layer2_nch_noise_simulation->GetYaxis()->SetTitleOffset(1.25);
    graphBetheADC_Layer2_nch_noise_simulation->GetYaxis()->CenterTitle();
    graphBetheADC_Layer2_nch_noise_simulation->GetXaxis()->SetLabelOffset(0.010);
    graphBetheADC_Layer2_nch_noise_simulation->GetXaxis()->SetLabelSize(0.045);
    graphBetheADC_Layer2_nch_noise_simulation->GetXaxis()->SetTitle("#beta#gamma_{sim}");
    graphBetheADC_Layer2_nch_noise_simulation->GetXaxis()->SetTitleSize(0.050);
    graphBetheADC_Layer2_nch_noise_simulation->GetXaxis()->SetTitleOffset(1.1);
    graphBetheADC_Layer2_nch_noise_simulation->GetXaxis()->CenterTitle();
    graphBetheADC_Layer2_nch_noise_simulation->SetMarkerStyle(kFullCircle);
    graphBetheADC_Layer2_nch_noise_simulation->SetMarkerSize(1.0);
    graphBetheADC_Layer2_nch_noise_simulation->SetMarkerColor(kBlue);
    graphBetheADC_Layer2_nch_noise_simulation->SetLineColor(kBlue);
    graphBetheADC_Layer2_nch_noise_simulation->SetFillColor(0);

    // diff ADC_Layer1_nch_noise simulation (51)
    TGraphErrors *graphdiff_ADC_Layer1_nch_noise_simulation = new TGraphErrors(n_points,BetaGammaLayer1,diff_ADC_Layer1_nch_noise,0,Err_diff_ADC_Layer1_nch_noise);
    graphdiff_ADC_Layer1_nch_noise_simulation->SetTitle("diff ADC_Layer1_nch_noise simulation");
    graphdiff_ADC_Layer1_nch_noise_simulation->GetYaxis()->SetLabelOffset(0.005);
    graphdiff_ADC_Layer1_nch_noise_simulation->GetYaxis()->SetLabelSize(0.045);
    graphdiff_ADC_Layer1_nch_noise_simulation->GetYaxis()->SetTitle("#frac{#LT#Delta E#GT - Data}{Data}");
    graphdiff_ADC_Layer1_nch_noise_simulation->GetYaxis()->SetTitleSize(0.050);
    graphdiff_ADC_Layer1_nch_noise_simulation->GetYaxis()->SetTitleOffset(1.25);
    graphdiff_ADC_Layer1_nch_noise_simulation->GetYaxis()->CenterTitle();
    graphdiff_ADC_Layer1_nch_noise_simulation->GetYaxis()->SetRangeUser(-0.3,0.2);
    graphdiff_ADC_Layer1_nch_noise_simulation->GetXaxis()->SetLabelOffset(0.010);
    graphdiff_ADC_Layer1_nch_noise_simulation->GetXaxis()->SetLabelSize(0.045);
    graphdiff_ADC_Layer1_nch_noise_simulation->GetXaxis()->SetTitle("#beta#gamma_{sim}");
    graphdiff_ADC_Layer1_nch_noise_simulation->GetXaxis()->SetTitleSize(0.050);
    graphdiff_ADC_Layer1_nch_noise_simulation->GetXaxis()->SetTitleOffset(1.1);
    graphdiff_ADC_Layer1_nch_noise_simulation->GetXaxis()->CenterTitle();
    graphdiff_ADC_Layer1_nch_noise_simulation->SetMarkerStyle(kFullCircle);
    graphdiff_ADC_Layer1_nch_noise_simulation->SetMarkerSize(1.0);
    graphdiff_ADC_Layer1_nch_noise_simulation->SetMarkerColor(kRed);
    graphdiff_ADC_Layer1_nch_noise_simulation->SetLineColor(kRed);
    graphdiff_ADC_Layer1_nch_noise_simulation->SetFillColor(0);

    // diff ADC_Layer1_nch_noise simulation (51)
    TGraphErrors *graphdiff_ADC_Layer2_nch_noise_simulation = new TGraphErrors(n_points,BetaGammaLayer2,diff_ADC_Layer2_nch_noise,0,Err_diff_ADC_Layer2_nch_noise);
    graphdiff_ADC_Layer2_nch_noise_simulation->SetTitle("diff ADC_Layer2_nch_noise simulation");
    graphdiff_ADC_Layer2_nch_noise_simulation->GetYaxis()->SetLabelOffset(0.005);
    graphdiff_ADC_Layer2_nch_noise_simulation->GetYaxis()->SetLabelSize(0.045);
    graphdiff_ADC_Layer2_nch_noise_simulation->GetYaxis()->SetTitle("#frac{#LT#Delta E#GT - Data}{Data}");
    graphdiff_ADC_Layer2_nch_noise_simulation->GetYaxis()->SetTitleSize(0.050);
    graphdiff_ADC_Layer2_nch_noise_simulation->GetYaxis()->SetTitleOffset(1.25);
    graphdiff_ADC_Layer2_nch_noise_simulation->GetYaxis()->CenterTitle();
    graphdiff_ADC_Layer2_nch_noise_simulation->GetYaxis()->SetRangeUser(-0.3,0.2);
    graphdiff_ADC_Layer2_nch_noise_simulation->GetXaxis()->SetLabelOffset(0.010);
    graphdiff_ADC_Layer2_nch_noise_simulation->GetXaxis()->SetLabelSize(0.045);
    graphdiff_ADC_Layer2_nch_noise_simulation->GetXaxis()->SetTitle("#beta#gamma_{sim}");
    graphdiff_ADC_Layer2_nch_noise_simulation->GetXaxis()->SetTitleSize(0.050);
    graphdiff_ADC_Layer2_nch_noise_simulation->GetXaxis()->SetTitleOffset(1.1);
    graphdiff_ADC_Layer2_nch_noise_simulation->GetXaxis()->CenterTitle();
    graphdiff_ADC_Layer2_nch_noise_simulation->SetMarkerStyle(kFullCircle);
    graphdiff_ADC_Layer2_nch_noise_simulation->SetMarkerSize(1.0);
    graphdiff_ADC_Layer2_nch_noise_simulation->SetMarkerColor(kBlue);
    graphdiff_ADC_Layer2_nch_noise_simulation->SetLineColor(kBlue);
    graphdiff_ADC_Layer2_nch_noise_simulation->SetFillColor(0);


    gStyle->SetOptTitle(0);
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(101); // display (0->no, 1->yes): first->Prob, second->Chi^2, third->Err, fourth->Par_value
    gStyle->SetCanvasColor(10);
    gStyle->SetPadColor(10);
    gStyle->SetFrameFillColor(10);

    TPad* ptpad;
    char cpad[50];


    // Show TCanvas
    TCanvas *c1 = new TCanvas( "c1","Bethe MeV nominal/simulation Layer1 - Layer2", 0, 0, 1200, 800 );

    c1->Divide(2,2,0.001,0.001);
    for (int i=1; i<5; i++) {
        sprintf(cpad,"c1_%d",i);
        ptpad = (TPad*) c1->FindObject(cpad);
        ptpad->SetLeftMargin(0.15);
        ptpad->SetBottomMargin(0.15);
        ptpad->SetRightMargin(0.15);
        ptpad->SetTopMargin(0.125);
        ptpad->SetFillColor(10);
        ptpad->SetLogx(0);
        ptpad->SetLogy(0);
        ptpad->SetGrid();
        c1->cd(i);
        switch(i) {
        case 1: {
                    printf("BetheLayer1_nominal\n");
                    graphBetheLayer1_nominal->Draw("APE");
                    BetheFit->SetParameter(0,1);    // the normalization of the Bethe-function is set to 1 because it has to fit MeV data
                    BetheFit->FixParameter(0,1);    // the normalization of the Bethe-function is set to 1 because it has to fit MeV data
                    graphBetheLayer1_nominal->Fit("BetheFit");
                    //BetheFit->Draw("Same");
                    printf("\n");
                    TLegend *legend_c1_1 = new TLegend(0.6,0.65,0.825,0.8);
                    legend_c1_1->AddEntry(BetheFitADC,"Bethe fit","l");
                    legend_c1_1->AddEntry(graphBetheLayer1_nominal,"Tracker Layer 1","p");
                    legend_c1_1->Draw();
                    break;
                }
        case 2: {
                    printf("BetheLayer2_nominal\n");
                    graphBetheLayer2_nominal->Draw("APE");
                    BetheFit->SetParameter(0,1);
                    BetheFit->FixParameter(0,1);
                    graphBetheLayer2_nominal->Fit("BetheFit");
                    //BetheFit->Draw("Same");
                    printf("\n");
                    TLegend *legend_c1_2 = new TLegend(0.6,0.65,0.825,0.8);
                    legend_c1_2->AddEntry(BetheFitADC,"Bethe fit","l");
                    legend_c1_2->AddEntry(graphBetheLayer2_nominal,"Tracker Layer 2","p");
                    legend_c1_2->Draw();
                    break;
                }
        case 3: {
                    printf("BetheLayer1_simulation\n");
                    graphBetheLayer1_simulation->Draw("APE");
                    BetheFit->SetParameter(0,1);
                    BetheFit->FixParameter(0,1);
                    graphBetheLayer1_simulation->Fit("BetheFit");
                    //BetheFit->Draw("Same");
                    printf("\n");
                    TLegend *legend_c1_3 = new TLegend(0.6,0.65,0.825,0.8);
                    legend_c1_3->AddEntry(BetheFitADC,"Bethe fit","l");
                    legend_c1_3->AddEntry(graphBetheLayer1_simulation,"Tracker Layer 1","p");
                    legend_c1_3->Draw();
                    break;
                }
        case 4: {
                    printf("BetheLayer2_simulation\n");
                    graphBetheLayer2_simulation->Draw("APE");
                    BetheFit->SetParameter(0,1);
                    BetheFit->FixParameter(0,1);
                    graphBetheLayer2_simulation->Fit("BetheFit");
                    //BetheFit->Draw("Same");
                    printf("\n");
                    TLegend *legend_c1_4 = new TLegend(0.6,0.65,0.825,0.8);
                    legend_c1_4->AddEntry(BetheFitADC,"Bethe fit","l");
                    legend_c1_4->AddEntry(graphBetheLayer2_simulation,"Tracker Layer 2","p");
                    legend_c1_4->Draw();
                    break;
                }
        default: break;
        } 
    }

    TCanvas *c2 = new TCanvas( "c2","Bethe ADC_pch nominal Layer1 - Layer2", 0, 0, 1200, 800 );

    c2->Divide(2,2,0.001,0.001);
    for (int i=1; i<5; i++) {
        sprintf(cpad,"c2_%d",i);
        ptpad = (TPad*) c2->FindObject(cpad);
        ptpad->SetLeftMargin(0.15);
        ptpad->SetBottomMargin(0.15);
        ptpad->SetRightMargin(0.15);
        ptpad->SetTopMargin(0.125);
        ptpad->SetFillColor(10);
        ptpad->SetLogx(0);
        ptpad->SetLogy(0);
        ptpad->SetGrid();
        c2->cd(i);
        switch(i) {
        case 1: {
                    printf("BetheADC_Layer1_pch_nominal\n");
                    graphBetheADC_Layer1_pch_nominal->Draw("APE");
                    BetheFitADC->SetParameter(0,p2ch_ADCconv);  // the normalization of the Bethe-function is set to the appropriate MeV-ADC conversion value because it has to fit ADC data
                    BetheFitADC->FixParameter(0,p2ch_ADCconv);  // the normalization of the Bethe-function is set to the appropriate MeV-ADC conversion value because it has to fit ADC data
                    graphBetheADC_Layer1_pch_nominal->Fit("BetheFitADC");
                    //BetheFit->Draw("Same");
                    printf("\n");
                    graphBetheADCdata_ladder2_pch->Draw("PE");
                    //graphBetheADCdata_ladder3_pch->Draw("PE");
                    TLegend *legend_c2_1 = new TLegend(0.6,0.60,0.825,0.8);
                    legend_c2_1->AddEntry(BetheFitADC,"Bethe fit","l");
                    legend_c2_1->AddEntry(graphBetheADC_Layer1_pch_nominal,"Tracker Layer 1","p");
                    legend_c2_1->AddEntry(graphBetheADCdata_ladder2_pch,"Data ladder 2 p","p");
                    //legend_c2_1->AddEntry(graphBetheADCdata_ladder3_pch,"Data ladder 3 p","p");
                    legend_c2_1->Draw();
                    break;
                }
        case 2: {
                    printf("BetheADC_Layer2_pch_nominal\n");
                    graphBetheADC_Layer2_pch_nominal->Draw("APE");
                    BetheFitADC->SetParameter(0,p3ch_ADCconv);
                    BetheFitADC->FixParameter(0,p3ch_ADCconv);
                    graphBetheADC_Layer2_pch_nominal->Fit("BetheFitADC");
                    //BetheFit->Draw("Same");
                    printf("\n");
                    //graphBetheADCdata_ladder2_pch->Draw("PE");
                    graphBetheADCdata_ladder3_pch->Draw("PE");
                    TLegend *legend_c2_2 = new TLegend(0.6,0.60,0.825,0.8);
                    legend_c2_2->AddEntry(BetheFitADC,"Bethe fit","l");
                    legend_c2_2->AddEntry(graphBetheADC_Layer2_pch_nominal,"Tracker Layer 2","p");
                    //legend_c2_2->AddEntry(graphBetheADCdata_ladder2_pch,"Data ladder 2 p","p");
                    legend_c2_2->AddEntry(graphBetheADCdata_ladder3_pch,"Data ladder 3 p","p");
                    legend_c2_2->Draw();
                    break;
                }
        case 3: {
                    graphdiff_ADC_Layer1_pch_nominal->Draw("APE");
                    break;
                }
        case 4: {
                    graphdiff_ADC_Layer2_pch_nominal->Draw("APE");
                    break;
                }
        default: break;
        } 
    }

    TCanvas *c21 = new TCanvas( "c21","Bethe ADC_pch simulation Layer1 - Layer2", 0, 0, 1200, 800 );

    c21->Divide(2,2,0.001,0.001);
    for (int i=1; i<5; i++) {
        sprintf(cpad,"c21_%d",i);
        ptpad = (TPad*) c21->FindObject(cpad);
        ptpad->SetLeftMargin(0.15);
        ptpad->SetBottomMargin(0.15);
        ptpad->SetRightMargin(0.15);
        ptpad->SetTopMargin(0.125);
        ptpad->SetFillColor(10);
        ptpad->SetLogx(0);
        ptpad->SetLogy(0);
        ptpad->SetGrid();
        c21->cd(i);
        switch(i) {
        case 1: {
                    printf("BetheADC_Layer1_pch_simulation\n");
                    graphBetheADC_Layer1_pch_simulation->Draw("APE");
                    BetheFitADC->SetParameter(0,p2ch_ADCconv);
                    BetheFitADC->FixParameter(0,p2ch_ADCconv);
                    graphBetheADC_Layer1_pch_simulation->Fit("BetheFitADC");
                    //BetheFit->Draw("Same");
                    printf("\n");
                    graphBetheADCdata_ladder2_pch->Draw("PE");
                    //graphBetheADCdata_ladder3_pch->Draw("PE");
                    TLegend *legend_c21_1 = new TLegend(0.6,0.60,0.825,0.8);
                    legend_c21_1->AddEntry(BetheFitADC,"Bethe fit","l");
                    legend_c21_1->AddEntry(graphBetheADC_Layer1_pch_simulation,"Tracker Layer 1","p");
                    legend_c21_1->AddEntry(graphBetheADCdata_ladder2_pch,"Data ladder 2 p","p");
                    //legend_c21_1->AddEntry(graphBetheADCdata_ladder3_pch,"Data ladder 3 p","p");
                    legend_c21_1->Draw();
                    break;
                }
        case 2: {
                    printf("BetheADC_Layer2_pch_simulation\n");
                    graphBetheADC_Layer2_pch_simulation->Draw("APE");
                    BetheFitADC->SetParameter(0,p3ch_ADCconv);
                    BetheFitADC->FixParameter(0,p3ch_ADCconv);
                    graphBetheADC_Layer2_pch_simulation->Fit("BetheFitADC");
                    //BetheFit->Draw("Same");
                    printf("\n");
                    //graphBetheADCdata_ladder2_pch->Draw("PE");
                    graphBetheADCdata_ladder3_pch->Draw("PE");
                    TLegend *legend_c21_2 = new TLegend(0.6,0.60,0.825,0.8);
                    legend_c21_2->AddEntry(BetheFitADC,"Bethe fit","l");
                    legend_c21_2->AddEntry(graphBetheADC_Layer2_pch_simulation,"Tracker Layer 2","p");
                    //legend_c21_2->AddEntry(graphBetheADCdata_ladder2_pch,"Data ladder 2 p","p");
                    legend_c21_2->AddEntry(graphBetheADCdata_ladder3_pch,"Data ladder 3 p","p");
                    legend_c21_2->Draw();
                    break;
                }
        case 3: {
                    graphdiff_ADC_Layer1_pch_simulation->Draw("APE");
                    break;
                }
        case 4: {
                    graphdiff_ADC_Layer2_pch_simulation->Draw("APE");
                    break;
                }
        default: break;
        } 
    }

    TCanvas *c3 = new TCanvas( "c3","Bethe ADC_nch nominal Layer1 - Layer2", 0, 0, 1200, 800 );

    c3->Divide(2,2,0.001,0.001);
    for (int i=1; i<5; i++) {
        sprintf(cpad,"c3_%d",i);
        ptpad = (TPad*) c3->FindObject(cpad);
        ptpad->SetLeftMargin(0.15);
        ptpad->SetBottomMargin(0.15);
        ptpad->SetRightMargin(0.15);
        ptpad->SetTopMargin(0.125);
        ptpad->SetFillColor(10);
        ptpad->SetLogx(0);
        ptpad->SetLogy(0);
        ptpad->SetGrid();
        c3->cd(i);
        switch(i) {
        case 1: {
                    printf("BetheADC_Layer1_nch_nominal\n");
                    graphBetheADC_Layer1_nch_nominal->Draw("APE");
                    BetheFitADC->SetParameter(0,n2ch_ADCconv);
                    BetheFitADC->FixParameter(0,n2ch_ADCconv);
                    graphBetheADC_Layer1_nch_nominal->Fit("BetheFitADC");
                    //BetheFit->Draw("Same");
                    printf("\n");
                    graphBetheADCdata_ladder2_nch->Draw("PE");
                    //graphBetheADCdata_ladder3_nch->Draw("PE");
                    TLegend *legend_c3_1 = new TLegend(0.6,0.60,0.825,0.8);
                    legend_c3_1->AddEntry(BetheFitADC,"Bethe fit","l");
                    legend_c3_1->AddEntry(graphBetheADC_Layer1_nch_nominal,"Tracker Layer 1","p");
                    legend_c3_1->AddEntry(graphBetheADCdata_ladder2_nch,"Data ladder 2 n","p");
                    //legend_c3_1->AddEntry(graphBetheADCdata_ladder3_nch,"Data ladder 3 n","p");
                    legend_c3_1->Draw();
                    break;
                }
        case 2: {
                    printf("BetheADC_Layer2_nch_nominal\n");
                    graphBetheADC_Layer2_nch_nominal->Draw("APE");
                    BetheFitADC->SetParameter(0,n3ch_ADCconv);
                    BetheFitADC->FixParameter(0,n3ch_ADCconv);
                    graphBetheADC_Layer2_nch_nominal->Fit("BetheFitADC");
                    //BetheFit->Draw("Same");
                    printf("\n");
                    //graphBetheADCdata_ladder2_nch->Draw("PE");
                    graphBetheADCdata_ladder3_nch->Draw("PE");
                    TLegend *legend_c3_2 = new TLegend(0.6,0.60,0.825,0.8);
                    legend_c3_2->AddEntry(BetheFitADC,"Bethe fit","l");
                    legend_c3_2->AddEntry(graphBetheADC_Layer2_nch_nominal,"Tracker Layer 2","p");
                    //legend_c3_2->AddEntry(graphBetheADCdata_ladder2_nch,"Data ladder 2 n","p");
                    legend_c3_2->AddEntry(graphBetheADCdata_ladder3_nch,"Data ladder 3 n","p");
                    legend_c3_2->Draw();
                    break;
                }
        case 3: {
                    graphdiff_ADC_Layer1_nch_nominal->Draw("APE");
                    break;
                }
        case 4: {
                    //graphdiff_ADC_Layer2_nch_nominal->Draw("APE");    // ladder 3 n-channel is not working properly!
                    break;
                }
        default: break;
        } 
    }

    TCanvas *c31 = new TCanvas( "c31","Bethe ADC_nch simulation Layer1 - Layer2", 0, 0, 1200, 800 );

    c31->Divide(2,2,0.001,0.001);
    for (int i=1; i<5; i++) {
        sprintf(cpad,"c31_%d",i);
        ptpad = (TPad*) c31->FindObject(cpad);
        ptpad->SetLeftMargin(0.15);
        ptpad->SetBottomMargin(0.15);
        ptpad->SetRightMargin(0.15);
        ptpad->SetTopMargin(0.125);
        ptpad->SetFillColor(10);
        ptpad->SetLogx(0);
        ptpad->SetLogy(0);
        ptpad->SetGrid();
        c31->cd(i);
        switch(i) {
        case 1: {
                    printf("BetheADC_Layer1_nch_simulation\n");
                    graphBetheADC_Layer1_nch_simulation->Draw("APE");
                    BetheFitADC->SetParameter(0,n2ch_ADCconv);
                    BetheFitADC->FixParameter(0,n2ch_ADCconv);
                    graphBetheADC_Layer1_nch_simulation->Fit("BetheFitADC");
                    //BetheFit->Draw("Same");
                    printf("\n");
                    graphBetheADCdata_ladder2_nch->Draw("PE");
                    //graphBetheADCdata_ladder3_nch->Draw("PE");
                    TLegend *legend_c31_1 = new TLegend(0.6,0.60,0.825,0.8);
                    legend_c31_1->AddEntry(BetheFitADC,"Bethe fit","l");
                    legend_c31_1->AddEntry(graphBetheADC_Layer1_nch_simulation,"Tracker Layer 1","p");
                    legend_c31_1->AddEntry(graphBetheADCdata_ladder2_nch,"Data ladder 2 n","p");
                    //legend_c31_1->AddEntry(graphBetheADCdata_ladder3_nch,"Data ladder 3 n","p");
                    legend_c31_1->Draw();
                    break;
                }
        case 2: {
                    printf("BetheADC_Layer2_nch_simulation\n");
                    graphBetheADC_Layer2_nch_simulation->Draw("APE");
                    BetheFitADC->SetParameter(0,n3ch_ADCconv);
                    BetheFitADC->FixParameter(0,n3ch_ADCconv);
                    graphBetheADC_Layer2_nch_simulation->Fit("BetheFitADC");
                    //BetheFit->Draw("Same");
                    printf("\n");
                    //graphBetheADCdata_ladder2_nch->Draw("PE");
                    graphBetheADCdata_ladder3_nch->Draw("PE");
                    TLegend *legend_c31_2 = new TLegend(0.6,0.60,0.825,0.8);
                    legend_c31_2->AddEntry(BetheFitADC,"Bethe fit","l");
                    legend_c31_2->AddEntry(graphBetheADC_Layer2_nch_simulation,"Tracker Layer 2","p");
                    //legend_c31_2->AddEntry(graphBetheADCdata_ladder2_nch,"Data ladder 2 n","p");
                    legend_c31_2->AddEntry(graphBetheADCdata_ladder3_nch,"Data ladder 3 n","p");
                    legend_c31_2->Draw();
                    break;
                }
        case 3: {
                    graphdiff_ADC_Layer1_nch_simulation->Draw("APE");
                    break;
                }
        case 4: {
                    //graphdiff_ADC_Layer2_nch_simulation->Draw("APE"); // ladder 3 n-channel is not working properly!
                    break;
                }
        default: break;
        } 
    }


    TCanvas *c4 = new TCanvas( "c4","Bethe ADC_pch_noise nominal Layer1 - Layer2", 0, 0, 1200, 800 );

    c4->Divide(2,2,0.001,0.001);
    for (int i=1; i<5; i++) {
        sprintf(cpad,"c4_%d",i);
        ptpad = (TPad*) c4->FindObject(cpad);
        ptpad->SetLeftMargin(0.15);
        ptpad->SetBottomMargin(0.15);
        ptpad->SetRightMargin(0.15);
        ptpad->SetTopMargin(0.125);
        ptpad->SetFillColor(10);
        ptpad->SetLogx(0);
        ptpad->SetLogy(0);
        ptpad->SetGrid();
        c4->cd(i);
        switch(i) {
        case 1: {
                    printf("BetheADC_Layer1_pch_noise_nominal\n");
                    graphBetheADC_Layer1_pch_noise_nominal->Draw("APE");
                    BetheFitADC->SetParameter(0,p2ch_ADCconv);
                    BetheFitADC->FixParameter(0,p2ch_ADCconv);
                    graphBetheADC_Layer1_pch_noise_nominal->Fit("BetheFitADC");
                    //BetheFit->Draw("Same");
                    printf("\n");
                    graphBetheADCdata_ladder2_pch->Draw("PE");
                    //graphBetheADCdata_ladder3_pch->Draw("PE");
                    TLegend *legend_c2_1 = new TLegend(0.6,0.60,0.825,0.8);
                    legend_c2_1->AddEntry(BetheFitADC,"Bethe fit","l");
                    legend_c2_1->AddEntry(graphBetheADC_Layer1_pch_noise_nominal,"Tracker Layer 1","p");
                    legend_c2_1->AddEntry(graphBetheADCdata_ladder2_pch,"Data ladder 2 p","p");
                    //legend_c2_1->AddEntry(graphBetheADCdata_ladder3_pch,"Data ladder 3 p","p");
                    legend_c2_1->Draw();
                    break;
                }
        case 2: {
                    printf("BetheADC_Layer2_pch_noise_nominal\n");
                    graphBetheADC_Layer2_pch_noise_nominal->Draw("APE");
                    BetheFitADC->SetParameter(0,p3ch_ADCconv);
                    BetheFitADC->FixParameter(0,p3ch_ADCconv);
                    graphBetheADC_Layer2_pch_noise_nominal->Fit("BetheFitADC");
                    //BetheFit->Draw("Same");
                    printf("\n");
                    //graphBetheADCdata_ladder2_pch->Draw("PE");
                    graphBetheADCdata_ladder3_pch->Draw("PE");
                    TLegend *legend_c2_2 = new TLegend(0.6,0.60,0.825,0.8);
                    legend_c2_2->AddEntry(BetheFitADC,"Bethe fit","l");
                    legend_c2_2->AddEntry(graphBetheADC_Layer2_pch_noise_nominal,"Tracker Layer 2","p");
                    //legend_c2_2->AddEntry(graphBetheADCdata_ladder2_pch,"Data ladder 2 p","p");
                    legend_c2_2->AddEntry(graphBetheADCdata_ladder3_pch,"Data ladder 3 p","p");
                    legend_c2_2->Draw();
                    break;
                }
        case 3: {
                    graphdiff_ADC_Layer1_pch_noise_nominal->Draw("APE");
                    break;
                }
        case 4: {
                    graphdiff_ADC_Layer2_pch_noise_nominal->Draw("APE");
                    break;
                }
        default: break;
        } 
    }

    TCanvas *c41 = new TCanvas( "c41","Bethe ADC_pch_noise simulation Layer1 - Layer2", 0, 0, 1200, 800 );

    c41->Divide(2,2,0.001,0.001);
    for (int i=1; i<5; i++) {
        sprintf(cpad,"c41_%d",i);
        ptpad = (TPad*) c41->FindObject(cpad);
        ptpad->SetLeftMargin(0.15);
        ptpad->SetBottomMargin(0.15);
        ptpad->SetRightMargin(0.15);
        ptpad->SetTopMargin(0.125);
        ptpad->SetFillColor(10);
        ptpad->SetLogx(0);
        ptpad->SetLogy(0);
        ptpad->SetGrid();
        c41->cd(i);
        switch(i) {
        case 1: {
                    printf("BetheADC_Layer1_pch_noise_simulation\n");
                    graphBetheADC_Layer1_pch_noise_simulation->Draw("APE");
                    BetheFitADC->SetParameter(0,p2ch_ADCconv);
                    BetheFitADC->FixParameter(0,p2ch_ADCconv);
                    graphBetheADC_Layer1_pch_noise_simulation->Fit("BetheFitADC");
                    //BetheFit->Draw("Same");
                    printf("\n");
                    graphBetheADCdata_ladder2_pch->Draw("PE");
                    //graphBetheADCdata_ladder3_pch->Draw("PE");
                    TLegend *legend_c21_1 = new TLegend(0.6,0.60,0.825,0.8);
                    legend_c21_1->AddEntry(BetheFitADC,"Bethe fit","l");
                    legend_c21_1->AddEntry(graphBetheADC_Layer1_pch_noise_simulation,"Tracker Layer 1","p");
                    legend_c21_1->AddEntry(graphBetheADCdata_ladder2_pch,"Data ladder 2 p","p");
                    //legend_c21_1->AddEntry(graphBetheADCdata_ladder3_pch,"Data ladder 3 p","p");
                    legend_c21_1->Draw();
                    break;
                }
        case 2: {
                    printf("BetheADC_Layer2_pch_noise_simulation\n");
                    graphBetheADC_Layer2_pch_noise_simulation->Draw("APE");
                    BetheFitADC->SetParameter(0,p3ch_ADCconv);
                    BetheFitADC->FixParameter(0,p3ch_ADCconv);
                    graphBetheADC_Layer2_pch_noise_simulation->Fit("BetheFitADC");
                    //BetheFit->Draw("Same");
                    printf("\n");
                    //graphBetheADCdata_ladder2_pch->Draw("PE");
                    graphBetheADCdata_ladder3_pch->Draw("PE");
                    TLegend *legend_c21_2 = new TLegend(0.6,0.60,0.825,0.8);
                    legend_c21_2->AddEntry(BetheFitADC,"Bethe fit","l");
                    legend_c21_2->AddEntry(graphBetheADC_Layer2_pch_noise_simulation,"Tracker Layer 2","p");
                    //legend_c21_2->AddEntry(graphBetheADCdata_ladder2_pch,"Data ladder 2 p","p");
                    legend_c21_2->AddEntry(graphBetheADCdata_ladder3_pch,"Data ladder 3 p","p");
                    legend_c21_2->Draw();
                    break;
                }
        case 3: {
                    graphdiff_ADC_Layer1_pch_noise_simulation->Draw("APE");
                    break;
                }
        case 4: {
                    graphdiff_ADC_Layer2_pch_noise_simulation->Draw("APE");
                    break;
                }
        default: break;
        } 
    }

    TCanvas *c5 = new TCanvas( "c5","Bethe ADC_nch_noise nominal Layer1 - Layer2", 0, 0, 1200, 800 );

    c5->Divide(2,2,0.001,0.001);
    for (int i=1; i<5; i++) {
        sprintf(cpad,"c5_%d",i);
        ptpad = (TPad*) c5->FindObject(cpad);
        ptpad->SetLeftMargin(0.15);
        ptpad->SetBottomMargin(0.15);
        ptpad->SetRightMargin(0.15);
        ptpad->SetTopMargin(0.125);
        ptpad->SetFillColor(10);
        ptpad->SetLogx(0);
        ptpad->SetLogy(0);
        ptpad->SetGrid();
        c5->cd(i);
        switch(i) {
        case 1: {
                    printf("BetheADC_Layer1_nch_noise_nominal\n");
                    graphBetheADC_Layer1_nch_noise_nominal->Draw("APE");
                    BetheFitADC->SetParameter(0,n2ch_ADCconv);
                    BetheFitADC->FixParameter(0,n2ch_ADCconv);
                    graphBetheADC_Layer1_nch_noise_nominal->Fit("BetheFitADC");
                    //BetheFit->Draw("Same");
                    printf("\n");
                    graphBetheADCdata_ladder2_nch->Draw("PE");
                    //graphBetheADCdata_ladder3_nch->Draw("PE");
                    TLegend *legend_c3_1 = new TLegend(0.6,0.60,0.825,0.8);
                    legend_c3_1->AddEntry(BetheFitADC,"Bethe fit","l");
                    legend_c3_1->AddEntry(graphBetheADC_Layer1_nch_noise_nominal,"Tracker Layer 1","p");
                    legend_c3_1->AddEntry(graphBetheADCdata_ladder2_nch,"Data ladder 2 n","p");
                    //legend_c3_1->AddEntry(graphBetheADCdata_ladder3_nch,"Data ladder 3 n","p");
                    legend_c3_1->Draw();
                    break;
                }
        case 2: {
                    printf("BetheADC_Layer2_nch_noise_nominal\n");
                    graphBetheADC_Layer2_nch_noise_nominal->Draw("APE");
                    BetheFitADC->SetParameter(0,n3ch_ADCconv);
                    BetheFitADC->FixParameter(0,n3ch_ADCconv);
                    graphBetheADC_Layer2_nch_noise_nominal->Fit("BetheFitADC");
                    //BetheFit->Draw("Same");
                    printf("\n");
                    //graphBetheADCdata_ladder2_nch->Draw("PE");
                    graphBetheADCdata_ladder3_nch->Draw("PE");
                    TLegend *legend_c3_2 = new TLegend(0.6,0.60,0.825,0.8);
                    legend_c3_2->AddEntry(BetheFitADC,"Bethe fit","l");
                    legend_c3_2->AddEntry(graphBetheADC_Layer2_nch_noise_nominal,"Tracker Layer 2","p");
                    //legend_c3_2->AddEntry(graphBetheADCdata_ladder2_nch,"Data ladder 2 n","p");
                    legend_c3_2->AddEntry(graphBetheADCdata_ladder3_nch,"Data ladder 3 n","p");
                    legend_c3_2->Draw();
                    break;
                }
        case 3: {
                    graphdiff_ADC_Layer1_nch_noise_nominal->Draw("APE");
                    break;
                }
        case 4: {
                    //graphdiff_ADC_Layer2_nch_noise_nominal->Draw("APE");  // ladder 3 n-channel is not working properly!
                    break;
                }
        default: break;
        } 
    }

    TCanvas *c51 = new TCanvas( "c51","Bethe ADC_nch_noise simulation Layer1 - Layer2", 0, 0, 1200, 800 );

    c51->Divide(2,2,0.001,0.001);
    for (int i=1; i<5; i++) {
        sprintf(cpad,"c51_%d",i);
        ptpad = (TPad*) c51->FindObject(cpad);
        ptpad->SetLeftMargin(0.15);
        ptpad->SetBottomMargin(0.15);
        ptpad->SetRightMargin(0.15);
        ptpad->SetTopMargin(0.125);
        ptpad->SetFillColor(10);
        ptpad->SetLogx(0);
        ptpad->SetLogy(0);
        ptpad->SetGrid();
        c51->cd(i);
        switch(i) {
        case 1: {
                    printf("BetheADC_Layer1_nch_noise_simulation\n");
                    graphBetheADC_Layer1_nch_noise_simulation->Draw("APE");
                    BetheFitADC->SetParameter(0,n2ch_ADCconv);
                    BetheFitADC->FixParameter(0,n2ch_ADCconv);
                    graphBetheADC_Layer1_nch_noise_simulation->Fit("BetheFitADC");
                    //BetheFit->Draw("Same");
                    printf("\n");
                    graphBetheADCdata_ladder2_nch->Draw("PE");
                    //graphBetheADCdata_ladder3_nch->Draw("PE");
                    TLegend *legend_c31_1 = new TLegend(0.6,0.60,0.825,0.8);
                    legend_c31_1->AddEntry(BetheFitADC,"Bethe fit","l");
                    legend_c31_1->AddEntry(graphBetheADC_Layer1_nch_noise_simulation,"Tracker Layer 1","p");
                    legend_c31_1->AddEntry(graphBetheADCdata_ladder2_nch,"Data ladder 2 n","p");
                    //legend_c31_1->AddEntry(graphBetheADCdata_ladder3_nch,"Data ladder 3 n","p");
                    legend_c31_1->Draw();
                    break;
                }
        case 2: {
                    printf("BetheADC_Layer2_nch_noise_simulation\n");
                    graphBetheADC_Layer2_nch_noise_simulation->Draw("APE");
                    BetheFitADC->SetParameter(0,n3ch_ADCconv);
                    BetheFitADC->FixParameter(0,n3ch_ADCconv);
                    graphBetheADC_Layer2_nch_noise_simulation->Fit("BetheFitADC");
                    //BetheFit->Draw("Same");
                    printf("\n");
                    //graphBetheADCdata_ladder2_nch->Draw("PE");
                    graphBetheADCdata_ladder3_nch->Draw("PE");
                    TLegend *legend_c31_2 = new TLegend(0.6,0.60,0.825,0.8);
                    legend_c31_2->AddEntry(BetheFitADC,"Bethe fit","l");
                    legend_c31_2->AddEntry(graphBetheADC_Layer2_nch_noise_simulation,"Tracker Layer 2","p");
                    //legend_c31_2->AddEntry(graphBetheADCdata_ladder2_nch,"Data ladder 2 n","p");
                    legend_c31_2->AddEntry(graphBetheADCdata_ladder3_nch,"Data ladder 3 n","p");
                    legend_c31_2->Draw();
                    break;
                }
        case 3: {
                    graphdiff_ADC_Layer1_nch_noise_simulation->Draw("APE");
                    break;
                }
        case 4: {
                    //graphdiff_ADC_Layer2_nch_noise_simulation->Draw("APE");   // ladder 3 n-channel is not working properly!
                    break;
                }
        default: break;
        } 
    }

    return;
}
