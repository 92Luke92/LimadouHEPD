#include "LOrientationInfo.hh"

OrientationInfo::OrientationInfo() : TObject(){
    a = 360/(2*TMath::Pi());
    Re = 6000000;
    
    roll = 0; 
    pitch = 0;
    yaw = 0;  
   
    q0 = 0;
    q1 = 0;
    q2 = 0;
    q3 = 0;   

}

OrientationInfo::~OrientationInfo(){
}

void OrientationInfo::SetAttitudeAngles(unsigned char *L0roll_angle,
					unsigned char *L0pitch_angle,
					unsigned char *L0yaw_angle){

   // to trasform level 0 information into the attitude angles
   roll = (-1*(floor(L0roll_angle[0]/128))*128*pow(256,2) + (L0roll_angle[0] % 128)*pow(256,2) + 
	   L0roll_angle[1]*256 + L0roll_angle[2])*DEGUNIT;
   
   pitch = (-1*(floor(L0pitch_angle[0]/128))*128*pow(256,2) + (L0pitch_angle[0]%128)*pow(256,2) + 
	    L0pitch_angle[1]*256 + L0pitch_angle[2])*DEGUNIT;
   
   yaw = (-1*(floor(L0yaw_angle[0]/128))*128*pow(256,2) +	(L0yaw_angle[0]%128)*pow(256,2) + 
	  L0yaw_angle[1]*256 +L0yaw_angle[2])*DEGUNIT;

   roll *= TMath::DegToRad();
   pitch *= TMath::DegToRad();
   yaw *= TMath::DegToRad();
   
}

TMatrixD OrientationInfo::SatOrbitToSatBody(void){ 

   TMatrixD Aij(3,3);
   TMatrixD Bij(3,3);
   TMatrixD Cij(3,3);
   


   Aij(0,0) = cos(roll);
   Aij(0,1) = 0;
   Aij(0,2) = -sin(roll);
   Aij(1,0) = 0;
   Aij(1,1) = 1;
   Aij(1,2) = 0; 
   Aij(2,0) = sin(roll);
   Aij(2,1) = 0;
   Aij(2,2) = cos(roll);

   Bij(0,0) = 1;
   Bij(0,1) = 0;
   Bij(0,2) = 0;
   Bij(1,0) = 0;
   Bij(1,1) = cos(pitch);
   Bij(1,2) = sin(pitch);
   Bij(2,0) = 0;
   Bij(2,1) = -sin(pitch);
   Bij(2,2) = cos(pitch);

   Cij(0,0) = cos(yaw);
   Cij(0,1) = sin(yaw);
   Cij(0,2) = 0;
   Cij(1,0) = -sin(yaw);
   Cij(1,1) = cos(yaw);
   Cij(1,2) = 0;
   Cij(2,0) = 0;
   Cij(2,1) = 0;
   Cij(2,2) = 1;

   return Aij*Bij*Cij;
}


void OrientationInfo::SetQuaternions(int L0q1, int L0q2, int L0q3){

   // to trasform level 0 information into the quaternions

   q1 = (-1*(floor(((L0q1 >> 24) & 0xFF)/128)) *128*pow(256,3) + (((L0q1 >> 24) & 0xFF) %128)*pow(256,3) +  
	 ((L0q1 >> 16) & 0xFF)*pow(256,2) + (((L0q1 >> 8) & 0xFF)*256) + (L0q1 & 0xFF))*pow(2,-30);	

   q2 = (-1*(floor(((L0q2 >> 24) & 0xFF)/128)) *128*pow(256,3) + (((L0q2 >> 24) & 0xFF) %128)*pow(256,3) + 
	 ((L0q2 >> 16) & 0xFF)*pow(256,2) +  (((L0q2 >> 8) & 0xFF)*256) + (L0q2 & 0xFF))*pow(2,-30);	

   q3 = (-1*(floor(((L0q3 >> 24) & 0xFF)/128)) *128*pow(256,3) + (((L0q3 >> 24) & 0xFF) %128)*pow(256,3) + 
	 ((L0q3 >> 16) & 0xFF)*pow(256,2) +  (((L0q3 >> 8) & 0xFF)*256) + (L0q3 & 0xFF))*pow(2,-30);	

   
   q0 = pow(1 -pow(q1,2) -pow(q2,2) -pow(q3,2), 1./2.);

   
}

TMatrixD OrientationInfo::SatBodyToJ2000(void){

   
   TMatrixD Pij(3,3);
    Pij(0,0) = pow(q0,2) + pow(q1,2) - pow(q2,2) - pow(q3,2);
    Pij(0,1) = 2*(q1*q2 + q0*q3);
    Pij(0,2) = 2*(q1*q3 - q0*q2);
    Pij(1,0) = 2*(q1*q2 - q0*q3);
    Pij(1,1) = pow(q0,2) - pow(q1,2) + pow(q2,2) - pow(q3,2);
    Pij(1,2) = 2*(q2*q3 + q0*q1);
    Pij(2,0) = 2*(q1*q3 + q0*q2);
    Pij(2,1) = 2*(q2*q3 - q0*q1);
    Pij(2,2) = pow(q0,2) - pow(q1,2) - pow(q2,2) + pow(q3,2);


    return Pij;
}

TMatrixD OrientationInfo::J2000toECF(TMatrixD Aij, UInt_t time){
   //t=1154304000+86400*365;
   UInt_t t =  OBDH_TO_EPOCH_OFFSET_SEC + time;

   TMatrixD Gij(3,3);
   Double_t omg = (7.292115e-5)*a; // Earth rotation velocity (Around polar axis);
   //Double_t t1 = 0;
   //if(t<=1158883200) t1 = 1127347200+229.2732;		     //absTime at 22/09/2005 + diference between Solar midnight and Greenwich sidereal midnight
   //if(t>1158883200&&t<=1190419200) t1 = 1158883200+172.3415;//absTime at 22/09/2006 + diference between Solar midnight and Greenwich sidereal midnight
   //if(t>=1190419200&&t<1222041600) t1 = 1190419200+115.39;  //absTime at 22/09/2007 + diference between Solar midnight and Greenwich sidereal midnight
   //if(t>=1222041600) t1 = 1222041600 + 294.9361;	     //absTime at 22/09/2008 + diference between Solar midnight and Greenwich sidereal midnight
   //UInt_t Nd = (t-t1)/86400;
   //Int_t DifSuSt = Nd*236.55;
   Double_t d = (t-10957*86400-43200); 		     //Number of day, passing from 01/01/2000 12:00:00 to t;
   d = d/86400;
   //d = t-da*86400+DifSuSt
   //cout<<"t = "<<t<<"\n";
   //cout<<"t - 2000y = "<<t-10957*86400-43200<<"\n";
   //cout<<"d = "<<d<<"\n";
   //Int_t tl = t%86400;    //!!!!!!!!!!!!!!!!!!!!!!!!
   Double_t T = d/36525;				     //Number of Julian centuries;
    
   //Double_t tl = t-t1-Nd*86400-DifSuSt;
   Double_t Se = 6*3600+41*60+236.555367908*d+0.093104*pow(T,2)-(6.2e-6)*pow(T,3);
   //cout<<"Se = "<<Se<<"\n";
   //cout<<t<<endl<<d<<endl<<tl<<endl<<Se+omg*tl*86400/360<<endl;
   Int_t tr = (t-10957*86400)%86400;
   //cout<<"tr = "<<tr<<endl;
   Double_t Somg = (Se+49.077+omg*86400*tr/360)*360/86400;
   //cout<<"t1 = "<<(t-10957*86400)%86400<<"\n";
   //cout<<"tr = "<<tr<<"\n";
   //cout<<"Somg = "<<Se+omg*86400*tr/360<<"\n";
   //cout<<"Somg = "<<((Somg-360*6)*86400/360/3600-20)*60<<"\n";
   //cout<<cos(Somg/a)<<endl;
   Gij(0,0) = cos(Somg/a);
   Gij(0,1) = -sin(Somg/a);
   Gij(0,2) = 0;
   Gij(1,0) = sin(Somg/a);
   Gij(1,1) = cos(Somg/a);
   Gij(1,2) = 0;
   Gij(2,0) = 0;
   Gij(2,1) = 0;
   Gij(2,2) = 1;
   Gij.Invert();
   //SetDirAxisGreenwich(Aij);
   //cout<<(Somg/a)<<endl<<Aij(0,0)<<" "<<Aij(1,0)<<" "<<Aij(2,0)<<endl;
   return Gij*Aij;
}

TMatrixD OrientationInfo::ECFtoGEO(Double_t lat, Double_t lon, TMatrixD Aij){
    //Double_t a = 360/(2*TMath::Pi());
    //Double_t Re = 6000000;
    TMatrixD Gij(3,3);
    TMatrixD Fij(3,3);
    
    TMatrixD Hij(3,3); //TEST
    TMatrixD Iij(3,3); //TEST
    
//    if((lat<0.1)&&(lat>-0.1)){
	//cout<<"lon = "<<lon<<" lat = "<<lat<<endl;
	lon=(-lon)/a; lat=(-lat)/a;
	//cout<<"lon = "<<lon<<" lat = "<<lat<<endl;
//    
//        cout<<"Quaternions Array"<<endl;
	//cout<<Aij(0,0)<<" "<<Aij(0,1)<<" "<<Aij(0,2)<<endl;
        //cout<<Aij(1,0)<<" "<<Aij(1,1)<<" "<<Aij(1,2)<<endl;
	//cout<<Aij(2,0)<<" "<<Aij(2,1)<<" "<<Aij(2,2)<<endl<<endl;
//    }
    //Double_t x0 = (alt+Re)*sin(lat)*sin(lon);
    //Double_t y0 = (alt+Re)*sin(lat)*cos(lon);
    //Double_t Sa = lon-a*atan(y0/x0);
    //if (y0>0&&x0<0) Sa=-Sa+90;
    //if (y0<0&&x0<0) Sa=Sa-90;
    //if (y0>0&&x0==0) Sa=90;
    //if (y0<0&&x0==0) Sa=-90;

    Gij(0,0) = cos(lon);
    Gij(0,1) = -sin(lon);
    Gij(0,2) = 0;
    Gij(1,0) = sin(lon);
    Gij(1,1) = cos(lon);
    Gij(1,2) = 0;
    Gij(2,0) = 0;
    Gij(2,1) = 0;
    Gij(2,2) = 1;

    //cout<<"First rotation"<<endl;
    //cout<<Gij(0,0)<<" "<<Gij(0,1)<<" "<<Gij(0,2)<<endl;
    //cout<<Gij(1,0)<<" "<<Gij(1,1)<<" "<<Gij(1,2)<<endl;
    //cout<<Gij(2,0)<<" "<<Gij(2,1)<<" "<<Gij(2,2)<<endl<<endl;
    
    //Gij.Invert();
    
    Fij(0,0) = cos(lat);
    Fij(0,1) = 0;
    Fij(0,2) = -sin(lat);
    Fij(1,0) = 0;
    Fij(1,1) = 1;
    Fij(1,2) = 0;
    Fij(2,0) = sin(lat);
    Fij(2,1) = 0;
    Fij(2,2) = cos(lat);
    
    //Fij.Invert();
    
    //if((lat<0.1)&&(lat>-0.1)){
    /*    Hij=Gij*Aij;  //TEST
    
        cout<<"First rotation"<<endl;
        cout<<Hij(0,0)<<" "<<Hij(0,1)<<" "<<Hij(0,2)<<endl;
        cout<<Hij(1,0)<<" "<<Hij(1,1)<<" "<<Hij(1,2)<<endl;
        cout<<Hij(2,0)<<" "<<Hij(2,1)<<" "<<Hij(2,2)<<endl<<endl;
    
        Iij = Fij*(Gij*Aij); //TEST
    
        cout<<"Second rotation"<<endl;
	cout<<Iij(0,0)<<" "<<Iij(0,1)<<" "<<Iij(0,2)<<endl;
        cout<<Iij(1,0)<<" "<<Iij(1,1)<<" "<<Iij(1,2)<<endl;
	cout<<Iij(2,0)<<" "<<Iij(2,1)<<" "<<Iij(2,2)<<endl;
//    
        Int_t ret;
	cin>>ret;*/
//    }
    return Fij*(Gij*Aij);
}

// TMatrixD OrientationInfo::PamelatoGEO(TMatrixD Aij, Double_t B1, Double_t B2, Double_t B3){
//     //TMatrixD Gij(3,3);
//     TMatrixD Hij(3,1);
//     TMatrixD Bij(3,1);
//     Bij(0,0) = B1;
//     Bij(1,0) = B2;
//     Bij(2,0) = B3;
//     //Double_t alfa = TMath::ASin(sqrt(1/((Aij(1,2))/Aij(0,2)+1))) * TMath::RadToDeg();
//     //Gij(0,0) = cos(alfa/a);   		
//     //Gij(0,1) = -sin(alfa/a);  		
//     //Gij(0,2) = 0;	  			
//     //Gij(1,0) = 0;	  			
//     //Gij(1,1) = 1;	  			
//     //Gij(1,2) = 0;	  			
//     //Gij(2,0) = sin(alfa/a);  			
//     //Gij(2,1) = cos(alfa/a);  			
//     //Gij(2,2) = 0;				
    
//     Hij=Aij*Bij;
//     return Hij;
//     //cout<<0.25-Aij(2,2)/(Aij(2,1)*Aij(2,0))<<endl;
//     //cout<<Hij(0,0)<<endl;//" "<<Hij(0,1)<<" "<<Hij(0,2)<<endl;
//     //cout<<Hij(1,0)<<endl;//" "<<Hij(1,1)<<" "<<Hij(1,2)<<endl;
//     //cout<<Hij(2,0)<<endl;//" "<<Hij(2,1)<<" "<<Hij(2,2)<<endl;
// }

// TMatrixD OrientationInfo::ColPermutation(TMatrixD Aij){
//     TMatrixD Gij(3,3);
//     Gij(0,0) = 1; Gij(0,1) = 0; Gij(0,2) = 0;
//     Gij(1,0) = 0; Gij(1,1) = 0; Gij(1,2) = 1;
//     Gij(2,0) = 0; Gij(2,1) = -1; Gij(2,2) = 0;
//     return Aij*Gij;
// }

// Double_t OrientationInfo::GetPitchAngle(Double_t x1, Double_t y1, Double_t z1, Double_t x2, Double_t y2, Double_t z2){
//     return TMath::ACos((x1*x2 + y1*y2 + z1*z2)/(sqrt(pow(x1,2)+pow(y1,2)+pow(z1,2))*sqrt(pow(x2,2)+pow(y2,2)+pow(z2,2)))) * TMath::RadToDeg();
//}
