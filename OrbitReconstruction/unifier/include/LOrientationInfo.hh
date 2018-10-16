#ifndef OrientationInfo_H
#define OrientationInfo_H

#include <iostream>
#include <stdio.h>

#include <TObject.h>
#include <TMatrixD.h>
#include <TString.h>
#include <TMath.h>

const unsigned int OBDH_TO_EPOCH_OFFSET_SEC=1230768000; // sec from 1970 01 01 to 2009 01 01
const double DEGUNIT=0.00005; // unit for satellite attitude angles 

class OrientationInfo : public TObject {

 private:

    Double_t a;// = 360/(2*TMath::Pi());
    Double_t Re;// = 6000000.0;

    // TMatrixD DirAxisGreenwich;
    // TMatrixD DirAxisGEO;
    // void SetDirAxisGreenwich(TMatrixD Aij){DirAxisGreenwich = Aij;}
    // void SetDirAxisGEO(TMatrixD Aij){DirAxisGEO = Aij;}
    //Double_t a = 360/(2*TMath::Pi());
    //Re = 6000000.0;

   double roll;  // angle expressed in radiants 
   double pitch; // angle expressed in radiants 
   double yaw;  // angle expressed in radiants 
   
   double q0, q1, q2, q3;   // quaternions
   
 public:

    OrientationInfo();
    ~OrientationInfo();

    Double_t PitchAngle;

   TMatrixD SatOrbitToSatBody(void); 
   TMatrixD SatBodyToJ2000(void);
   TMatrixD J2000toECF(TMatrixD Aij, UInt_t t);
   TMatrixD ECFtoGEO(Double_t lat, Double_t lon, TMatrixD Aij);
   
   void SetAttitudeAngles(unsigned char *L0roll_angle,unsigned char *L0pitch_angle,unsigned char *L0yaw_angle);
   void SetQuaternions(int q1, int q2, int q3);

   inline double GetSatRollAngle(void){return roll;};
   inline double GetSatPitchAngle(void){return pitch;};
   inline double GetSatYawAngle(void){return yaw;};
   inline double Getq0(void){return q0;};
   inline double Getq1(void){return q1;};
   inline double Getq2(void){return q2;};
   inline double Getq3(void){return q3;};


   //  Double_t GetPitchAngle(Double_t x1, Double_t y1, Double_t z1, Double_t x2, Double_t y2, Double_t z2);

    // Double_t GetPamMainGwX(){return DirAxisGreenwich(0,1);}
    // Double_t GetPamMainGwY(){return DirAxisGreenwich(1,1);}
    // Double_t GetPamMainGwZ(){return DirAxisGreenwich(2,1);}

    // Double_t GetPamMainGEOX(){return DirAxisGEO(0,1);}
    // Double_t GetPamMainGEOY(){return DirAxisGEO(1,1);}
    // Double_t GetPamMainGEOZ(){return DirAxisGEO(2,1);}

    // TMatrixD GetGw(){return DirAxisGreenwich;}
    // TMatrixD GetGEO(){return DirAxisGEO;}

};

#endif
