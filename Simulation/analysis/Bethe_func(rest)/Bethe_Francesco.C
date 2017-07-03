//serve a convertire l'energia cinetica associata a una particella di data massa in beta gamma
float Conv(double x, double mass){
    double toverm = x/mass;
    return sqrt(toverm*toverm+2*toverm);
}
//calcola la delta della bethe
double delta(Double_t * x, double x_0, double x_1, double Cbar, double a, double k, double delta_0 ){//x_0 par_0,x_1 par_1,C par_2,a par_3,k par_4, delta_0 par_5
    double result;
    double Logeta = TMath::Log10(x[0]);
    if(Logeta>=x_1){result = 2*TMath::Log(10)*Logeta-Cbar;}
    else if((Logeta>x_0)&&(Logeta<x_1)){result = 2*TMath::Log(10)*Logeta-Cbar+a*pow(x_1-Logeta,k);}
    else {result=0;}
    //else {result = delta_0*pow(10,2*(Logeta-x_0));}//for conductor, 0 for semiconduction
    return result;
}

Double_t Bethe_p(Double_t *x, Double_t *par){
    double MASS = 938.27;// MeV
    double K = 0.307075; //MeV*g^-1*cm^2 for A= 1 g/mol 
    double Z = 14; // atomic number of Silicon
    //double Z = 29; // atomic number of Copper
    double z2 = 1; // charge squared of the incident particle
    double mass_e = 0.511; //MeV
    //double rho = 8.92;// copper g cm^-3
    double rho = 2.33 ;// silicon g cm^-3
    double A = 28; // mass number of Silicon
    //double A = 63.5; // mass number of Copper
    double r_e = 2.817*pow(10,-15);//radius electron in meters
    double moverM = mass_e/MASS;
    double MeanExEn = 12.5 * pow(10,-6) *Z;// Mean exitation energy Silicon Z = 14 in eV
    //double MeanExEn = 11.1 * pow(10,-6) *Z;// Mean exitation energy Silicon Z = 29 in eV
    double numT_max = 2*mass_e*x[0]*x[0];
    double denTmax = 1+2.*sqrt(x[0]*x[0]+1)*moverM+(moverM*moverM);
    double beta2inv =((1+x[0]*x[0])/(x[0]*x[0]));
    double fact1 = 0.5*TMath::Log(2*mass_e*x[0]*x[0]*numT_max/(MeanExEn*MeanExEn*denTmax));
    double fact2 = 1./beta2inv;
    double Enplasma = sqrt(rho*Z/A)*28.816*pow(10,-6);
    //double fact3 = 0.5*delta(x,-0.0254,3.2792,4.4190,0.1434,2.9044,0.08);//x_0 par_0,x_1 par_1,C par_2,a par_3,m par_4, delta_0 par_5
    double fact3 = 0.5*delta(x,0.2014,2.8715,4.4351,0.1492,3.2546,0.058);
    //double fact3 =0.;// TMath::Log(Enplasma/MeanExEn)+TMath::Log(x[0])-0.5;
    double result = par[0]*K/A*Z*z2*beta2inv*(fact1-fact2-fact3) * rho;
    return result;
}
