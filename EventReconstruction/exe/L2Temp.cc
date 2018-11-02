#include "LEvRec1File.hh"
#include "LEvRec1.hh"
#include "LEvRec0File.hh"
//#include "LOrientationInfo.hh"

#include "LEvRec0.hh"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TLine.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TPaveStats.h"
#include "TString.h"
#include "LOfflineL0Selector.hh"
#include "LCalibration.hh"
#include "LCaloEqualization.hh"

const int NBINS = 4096/4;
const double XMIN = 0.;
const double XMAX = 4096.;
const double NPMTT = 63;

const double BINPMT = 4100;
// const double BINPMT = 4100;
const double CALO_SIGTHD = 10;
const double CALO_VETOTHD = 3;
// const double KCALOMU = (31.85/8572.);
// const double OFFSETMU = 0;
const double KCALOFITNOTRIG = (1/265.1);
const double OFFSETFITNOTRIG = 656.3*KCALOFITNOTRIG;

const double KCALOFIT = (1./296.4);
const double OFFSETFIT = 3110*KCALOFIT;

const double KCALOFITVINC = (1./260.);
const double OFFSETFITVINC = 205*KCALOFITVINC;

const double KCALOFITLYSO = (1/4.67);
const double OFFSETFITLYSO = 12.5*KCALOFITLYSO;

const double PLANETOBAR_THRESH = 10.;

const int    PLANEINTRIG = 2;
const double TRIGEQ = 100.;
const double PLANEEQ = 200.;

typedef struct trigPaddle
{
   int    pad;
   double signal; 
}trigPaddle_t;


bool isContainedInUpperCalo(LEvRec1 ev);
double CaloSum(LEvRec1 ev, LCaloEqualization *eqHG);
double LysoSum(LEvRec1 ev, LCaloEqualization *eqLG);



bool isContainedInUpperCalo(LEvRec1 ev)
{
   
   for (int i=0; i<NLYSOCRYSTALS;i++)
   { 
      if( ev.lyso.sn_lg[i][0] >= CALO_SIGTHD ) 
	 return false;
   }
   
   if(ev.veto.sn_hg[4][0] >= CALO_VETOTHD || 
      ev.veto.sn_hg[4][1] >= CALO_VETOTHD   )
      return false;
   
   bool ret = !ev.isLatVetoHit(CALO_VETOTHD);
   return ret;   
}




double CaloSum(LEvRec1 ev, LCaloEqualization *eqHG)
{
   int OFFSET=NTRIGSCINT*2;
   double sum=0;
   
   for (int i=0; i<NSCINTPLANES;i++)
   {
      if (i == 4 && ev.scint.sn_hg[i][1] >= CALO_SIGTHD)
	 sum += ((ev.scint.cont_hg[i][1]*(PLANEEQ/eqHG->GetMPVFactor(OFFSET+2*i+1)))*2); // P5se correction
      else
	 if(ev.scint.sn_hg[i][0] >= CALO_SIGTHD &&
	    ev.scint.sn_hg[i][1] >= CALO_SIGTHD   )
	    sum += (ev.scint.cont_hg[i][0]*(PLANEEQ/eqHG->GetMPVFactor(OFFSET+2*i)) + 
		    ev.scint.cont_hg[i][1]*(PLANEEQ/eqHG->GetMPVFactor(OFFSET+2*i+1)));
   }
   
   return sum;   
}


double LysoSum(LEvRec1 ev, LCaloEqualization *eqLG)
{
   double sum=0;
   int OFFSET=NTRIGSCINT*2+NSCINTPLANES*2+NVETOSCINT*2; 
   for (int i=0; i<NLYSOCRYSTALS;i++)
      if(ev.lyso.sn_lg[i][0] >= CALO_SIGTHD)
	 sum += (ev.lyso.cont_lg[i][0]*(PLANEEQ/eqLG->GetMPVFactor(OFFSET+i)));  


   return sum;   
}


// double HEPDangle(LEvRec1 ev)
// {

//    double angle;

//    TMatrixD A(3,3);
//    TMatrixD B(3,3);
//    TMatrixD C(3,3);
//    TMatrixD D(3,3);
//    TMatrixD HEPD_dir_SO(3,1) ;
//    HEPD_dir_SO(0,0) = 0;
//    HEPD_dir_SO(1,0) = 0; 
//    HEPD_dir_SO(2,0) = -1;

//    TMatrixD HEPD_dir_ECF(3,1) ;
//    OrientationInfo oi;
   

//    oi.SetAttitudeAngles(ev.lev0MD.broadcast.AOCC.roll_angle, 
// 			ev.lev0MD.broadcast.AOCC.pitch_angle, 
// 			ev.lev0MD.broadcast.AOCC.yaw_angle);
   
//    oi.SetQuaternions(ev.lev0MD.broadcast.AOCC.q1, 
// 		     ev.lev0MD.broadcast.AOCC.q2, 
// 		     ev.lev0MD.broadcast.AOCC.q3);
   
//    A = oi.SatOrbitToSatBody();
//    B = oi.SatBodyToJ2000();
//    C = oi.J2000toECF(B, ev.abstime);

//    HEPD_dir_ECF = A*HEPD_dir_SO;   // Sat Orbit - Sat Body 
//    HEPD_dir_ECF = B*HEPD_dir_ECF;  // Sat Body - J2000
//    HEPD_dir_ECF = C*HEPD_dir_ECF;  // J2000 - ECF
   
//    double Btot = pow((pow(ev.B_x, 2)+ pow(ev.B_y, 2) + pow(ev.B_z, 2)), 1./2.);
//    angle = acos((ev.B_x*HEPD_dir_ECF(0,0) + ev.B_y*HEPD_dir_ECF(1,0) + ev.B_z*HEPD_dir_ECF(2,0)) / Btot ) * TMath::RadToDeg();

//    std::cout << "HEPD angle =  " << angle << std::endl;
//    return angle;

// }
 

int main(int argc, char **argv) {
   
   LEvRec1 ev;
   LEvRec1File fin(argv[1],"READ");
   fin.SetTheEventPointer(ev);
   fin.SetMDPointer(ev);

   std::string eqHGFileName = "/storage/gpfs_data/limadou/asotgiu/EQUALIZATION_2/eqHG_MIPcut.txt";
   std::string eqLGFileName = "/storage/gpfs_data/limadou/asotgiu/EQUALIZATION_2/eqLG_MIPcut.txt";

   LCaloEqualization *eqHG = LCaloEqualization::Read(eqHGFileName);
   LCaloEqualization *eqLG = LCaloEqualization::Read(eqLGFileName);

   eqHG->Dump();
   eqLG->Dump();

   int OFFSET = 0;


   TFile fout(argv[2],"recreate"); // NEW

   
   Double_t     planeSum[16];
   Double_t     lysoCrystal[9];
   Bool_t       isPlaneHit[16];

   Double_t     trigSum[6];
   Double_t     trigSumMIP[6];
   Bool_t       isTrigHit[6];
   Bool_t       isLysoHit[9];

   Bool_t       isP5seHit3;
   Bool_t       isP5seHit5;   
   Bool_t       isaTrigPMTSat;  
   Bool_t       isaScintPMTSat;   
   Bool_t       isaLysoPMTSat;   
   Bool_t       isaVetoPMTSat;   
   Bool_t       offlineTrig;
   Bool_t       isAutoVeto;
   Bool_t       isLateralVetoHit;
   Bool_t       isBotVetoHit;
   Int_t        LastPlane;
   Double_t     UPcaloSignal;
   Double_t     LYSOSignal;
   Double_t     LYSOEnergy;
   Double_t     Etot;
   Double_t     UPcaloEAS1Signal;
   Double_t     UPcaloEAS2Signal;
   Double_t     UPcaloNOP5Energy;
   Double_t     UPcaloEnergy;
   Double_t     UPcaloTrigSignal;
   Double_t     UPcaloTrigEnergy;

   Double_t     TrackerMSCounts;
   Double_t     TrackerMSSN;
   Double_t     PlaneToBar;

   Double_t     rig, B, alt, lat, lon, L, B_x, B_y, B_z;
   unsigned int abstime;
   

   Int_t            TrigMult;
   Int_t            PlaneMult;
   Int_t            LysoMult;

   trigPaddle_t     Etrig;
   trigPaddle_t     EtrigMIP;
   Int_t            mostSigPad;
   double           mostSigPadSignal;
   double           mostSigPadSignalMIP;
   double           mostSigPadSignalMIP_GM;

   double           HEPDaxisDir;
   double           HEPDangleStep;
   double           HEPDangleStart;
   unsigned int     runDuration;
   unsigned int     runStartTime;
   
   unsigned short   boot_nr;
   unsigned short   run_id;
   unsigned int     event_index;
   unsigned int     alive_time;
   unsigned int     dead_time;
   unsigned int     hepd_time;
   unsigned int     startRunTime;
   unsigned int     stopRunTime;
   // unsigned char    roll_angle[3];
   // unsigned char    pitch_angle[3];
   // unsigned char    yaw_angle[3];
   // unsigned int     q1;
   // unsigned int     q2;
   // unsigned int     q3;

   LEvRec0Md   lev0MD;
   TTree * level2MD = new TTree("L2md","L2md");
   level2MD->Branch("boot_nr", &lev0MD.boot_nr);
   level2MD->Branch("run_id", &lev0MD.run_id);
   level2MD->Branch("run_type", &lev0MD.run_type);
   level2MD->Branch("run_duration", &lev0MD.run_duration);
   level2MD->Branch("orbitZone", &lev0MD.orbit_Zone);
   level2MD->Branch("silConfiguration", &lev0MD.silConfig.ladder_on[0],
	    "ladder_on[3]/O:ladder_mask[3]/O:plane_HV[2]/s:adj_strip/s"
	     ":zero_supp_thrd/s:thrd_CN_HIGH/b:thrd_CN_LOW/b:calib_event_CN/s"
	     ":calib_event_ped/s:calib_event_RMS/s:calib_event_gauss/s:gauss_check/s");
   level2MD->Branch("trigger_mask[2]", &lev0MD.trigger_mask[0]);
   level2MD->Branch("easiroc_conf[60]", &lev0MD.easiroc_config[0]);
   level2MD->Branch("PMT_mask[64]", &lev0MD.PMT_mask[0]);
   level2MD->Branch("HV_mask[12]", &lev0MD.HV_mask[0]);
   level2MD->Branch("HV_value[10]", &lev0MD.HV_value[0]);
   level2MD->Branch("gen_trig_mask[18]", &lev0MD.gen_trig_mask[0]);
   // broadcasta data
   level2MD->Branch("OBDH_info", &lev0MD.broadcast.OBDH.sec,
		    "sec_OBDH/i:ms_OBDH/s:NU[2]/b" );
   level2MD->Branch("GPS_info", &lev0MD.broadcast.GPS.sec,
		    "sec_GPS/i:axis_pos[3]/I:axis_vel[3]/I:dataID/b"
		    ":lon/I:lat/I:NU[3]/b");
   level2MD->Branch("AOCC_info", &lev0MD.broadcast.AOCC.sec,
		    "sec_AOCC/i:us_AOCC/i:x_coord[3]/b:y_coord[3]/b"
		    ":z_coord[3]/b:roll_angle[3]/b:pitch_angle[3]/b"
		    ":yaw_angle[3]/b:roll_angl_vel/s:pitch_angl_vel/s"
		    ":yaw_angl_vel/s:attitude_det/b:magn_torquer/b"
		    ":magn_torquer_pulse[3]/s:q1/i:q2/i:q3/i");
   level2MD->Branch("star_sensor_info", &lev0MD.broadcast.star_sensor.sec_ss11,
		    "sec_ss11/i:us_ss11/i:att_quat_ss11[4]/i:sec_ss21/i:us_ss21/i"
		    ":att_quat_ss21[4]/i:sec_ss31/i:us_ss31/i:att_quat_ss31[4]/i"
		    ":quat_valID_x1/b:sec_ss12/i:us_ss12/i:att_quat_ss12[4]/i"
		    ":sec_ss22/i:us_ss22/i:att_quat_ss22[4]/i:sec_ss32/i:us_ss32/i"
		    ":att_quat_ss32[4]/i:quat_valID_x2/b:attitude_det/b:NU/b");
   // scientific packet
   level2MD->Branch("PMT_rate_meter[65]", &lev0MD.PMT_rate_meter[0]);
   level2MD->Branch("CPU_temp_start_stop_Run[2]", &lev0MD.CPU_temp[0]);
   level2MD->Branch("PMT_temp_start_stop_Run[2]", &lev0MD.PMT_temp[0]);
   level2MD->Branch("CPU_time_start_stop_Run[2]", &lev0MD.CPU_time[0]);
   // timestamp CPU
   level2MD->Branch("CPU_timestamp", &lev0MD.timestamp.OBDH,
		    "OBDH/i:GPS/i:AOCC/i:star_sensor/i");
   
   // board status
   level2MD->Branch("status_register", &lev0MD.status_reg.statusDAQ,
	       "statusDAQ/s:statusPMT/s:statusTM_TC/s:statusHV_PS/s"
	       ":CPU_board_boot/s:statusCPU/s");
  
   TTree * level2 = new TTree("L2","L2");

   level2->Branch("boot_nr", &boot_nr);
   level2->Branch("run_id", &run_id);
   level2->Branch("hepd_time", &hepd_time);
   level2->Branch("startRunTime", &startRunTime);
   level2->Branch("stopRunTime", &stopRunTime);
   level2->Branch("event_index", &event_index);
   level2->Branch("alive_time", &alive_time);
   level2->Branch("dead_time", &dead_time);
   // level2->Branch("roll_angle[3]", &roll_angle[0]);
   // level2->Branch("pitch_angle[3]", &pitch_angle[0]);
   // level2->Branch("yaw_angle[3]", &yaw_angle[0]);
   // level2->Branch("q1", &q1);
   // level2->Branch("q2", &q2);
   // level2->Branch("q3", &q3);

   level2->Branch("TrackerMSCounts", &TrackerMSCounts, "TrackerMSCounts/D");
   level2->Branch("TrackerMSSN", &TrackerMSSN, "TrackerMSSN/D");

   level2->Branch("trigSumSig[6]", &trigSum[0]);
   level2->Branch("trigSumMIP[6]", &trigSumMIP[0]);
   level2->Branch("mostSigPad", &mostSigPad);
   level2->Branch("mostSigPadSignal", &mostSigPadSignal);
   level2->Branch("mostSigPadSignalMIP", &mostSigPadSignalMIP);
   level2->Branch("mostSigPadSignalMIP_GM", &mostSigPadSignalMIP_GM);
   level2->Branch("planeSumSig[16]", &planeSum[0]);
   level2->Branch("lysoCrystalSig[9]", &lysoCrystal[0]);

   level2->Branch("isTrigHit[6]", &isTrigHit[0]);
   level2->Branch("isPlaneHit[16]", &isPlaneHit[0]);
   level2->Branch("isLysoHit[9]", &isLysoHit[0]);

   level2->Branch("offlineTrigger", &offlineTrig);
   level2->Branch("isAutoVeto", &isAutoVeto);
   level2->Branch("isLatVetoHit", &isLateralVetoHit);
   level2->Branch("isBotVetoHit", &isBotVetoHit); 
   level2->Branch("isP5seHit3", &isP5seHit3);
   level2->Branch("isP5seHit5", &isP5seHit5);
   level2->Branch("isaTrigPMTSat", &isaTrigPMTSat);
   level2->Branch("isaScintPMTSat", &isaScintPMTSat);
   level2->Branch("isaLysoPMTSat", &isaLysoPMTSat);
   level2->Branch("isaVetoPMTSat", &isaVetoPMTSat);

   level2->Branch("UPcaloTrigEnergy", &UPcaloTrigEnergy);
   level2->Branch("UPcaloSignal", &UPcaloSignal);
   level2->Branch("LYSOSignal", &LYSOSignal);
   level2->Branch("LYSOEnergy", &LYSOEnergy);
   level2->Branch("Etot", &Etot);

   level2->Branch("TrigMult", &TrigMult);
   level2->Branch("planeMult", &PlaneMult);
   level2->Branch("LysoMult", &LysoMult);

   level2->Branch("lastPlane", &LastPlane);
   level2->Branch("PlaneToBar", &PlaneToBar, "PlaneToBar/D");
  
  
   // level2->Branch("rig", &rig);  
   // level2->Branch("abstime", &abstime);  
   // level2->Branch("B", &B);  
   // level2->Branch("lon", &lon);  
   // level2->Branch("lat", &lat);  
   // level2->Branch("alt", &alt);  
   // level2->Branch("L", &L);  
   // level2->Branch("B_x", &B_x);  
   // level2->Branch("B_y", &B_y);  
   // level2->Branch("B_z", &B_z);  
   // level2->Branch("HEPDaxisDir", &HEPDaxisDir);  


   int nent = fin.GetEntries();

   fin.GetEntry(0);   
   fin.GetMDEntry(0);
   // runStartTime = ev.abstime;  // first event time
   // HEPDangleStart = HEPDangle(ev);  // first event angle

   // fin.GetMDEntry(1);
   // fin.GetEntry(fin.GetEntries()-1); 
   // HEPDangleStep = HEPDangle(ev);   // last event angle
   // runDuration = ev.abstime;          // last event time


   // HEPDangleStep -= HEPDangleStart; 
   // runDuration -= runStartTime;        
   // HEPDangleStep /= (double)runDuration;



   // fin.GetEntry(nent-1);
   // unsigned int halfRunTime = ev.hepd_time;
   // fin.GetEntry(0);
   // halfRunTime -= ev.hepd_time;
   // halfRunTime /=2.;

   std::cout << " entries = " << nent << std::endl;
   double trigSignal = 0;
   bool isHG=true;	      

   for(int imd=0; imd<fin.GetMDEntries() ; imd++)
   {
      fin.GetMDEntry(imd);
      lev0MD=ev.lev0MD;
      level2MD->Fill();
      
   }
   
   for(int iev=0; iev<nent ; iev++)
//   for(int iev=0; iev<100000 ; ++iev)
   {
      fin.GetEntry(iev);

      if(iev%100000 == 0)
	 std::cout << ((double)iev/(double)nent)*100 << "%" << std::endl;
      
      mostSigPad =  ev.trig.GetTheMostSignificantUnit(isHG, CALO_SIGTHD);

      if(mostSigPad >= 0 )
      {
	 trigSignal = (ev.trig.cont_hg[mostSigPad][0] * 
		       (TRIGEQ/eqHG->GetMPVFactor(2*mostSigPad)) + 
		       ev.trig.cont_hg[mostSigPad][1] * 
		       (TRIGEQ/eqHG->GetMPVFactor(2*mostSigPad+1)));
	 
	 mostSigPadSignal = trigSignal;
	 mostSigPadSignalMIP = (ev.trig.cont_hg[mostSigPad][0]/eqHG->GetMPVFactor(2*mostSigPad) + 
				ev.trig.cont_hg[mostSigPad][1]/eqHG->GetMPVFactor(2*mostSigPad+1) )/2.;
	 
	 mostSigPadSignalMIP_GM = pow(((ev.trig.cont_hg[mostSigPad][0]/eqHG->GetMPVFactor(2*mostSigPad))* 
				       (ev.trig.cont_hg[mostSigPad][1]/eqHG->GetMPVFactor(2*mostSigPad)))
					 ,1./2.);
	 isAutoVeto = ev.AutoVeto(CALO_VETOTHD, mostSigPad);
	 
      }
      else
      {
	 mostSigPadSignal = -999;
	 mostSigPadSignalMIP = -999;
	 mostSigPadSignalMIP_GM = -999;
      }


      boot_nr = ev.boot_nr;
      run_id = ev.run_id;
      event_index = ev.event_index;
      alive_time = ev.alive_time;
      dead_time = ev.dead_time;
      hepd_time = ev.hepd_time;

      // if(ev.hepd_time > halfRunTime)
      // 	 fin.GetMDEntry(1);
      
      // for(int ia =0; ia<3; ia++ )
      // {
      // 	 roll_angle[ia] = ev.lev0MD.broadcast.AOCC.roll_angle[ia];
      // 	 pitch_angle[ia] = ev.lev0MD.broadcast.AOCC.pitch_angle[ia];
      // 	 yaw_angle[ia] = ev.lev0MD.broadcast.AOCC.yaw_angle[ia];
      // }

      // unsigned int     startRunTime;
      // unsigned int     stopRunTime;
      // q1 = ev.lev0MD.broadcast.AOCC.q1;
      // q2 = ev.lev0MD.broadcast.AOCC.q2;
      // q3 = ev.lev0MD.broadcast.AOCC.q3;



      // no lat veto hit
      isLateralVetoHit = ev.isLatVetoHit(CALO_VETOTHD);
      offlineTrig = ev.PreSelection(CALO_SIGTHD, mostSigPad, PLANEINTRIG);
      
      // hit on T1... T6 paddle
      for(int i = 0; i<6; i++)
      {
	 trigSum[i] = (ev.trig.cont_hg[i][0] * (TRIGEQ/eqHG->GetMPVFactor(2*i)) +
		       ev.trig.cont_hg[i][1] * (TRIGEQ/eqHG->GetMPVFactor(2*i+1)));
	 trigSumMIP[i] = (ev.trig.cont_hg[i][0] /eqHG->GetMPVFactor(2*i) +    
			  ev.trig.cont_hg[i][1] /eqHG->GetMPVFactor(2*i+1))/2.;

	 isTrigHit[i] = ( (ev.trig.sn_hg[i][0] > CALO_VETOTHD) ||
			  (ev.trig.sn_hg[i][1] > CALO_VETOTHD)   );
      }

      // hit on P1... P16 plane except P5
      OFFSET = NTRIGSCINT*2;
      for(int i = 0; i<16; i++)
      {
      	 if(i==4)
	 {
	    planeSum[i] = (ev.scint.cont_hg[i][1]*(PLANEEQ/eqHG->GetMPVFactor(OFFSET+2*i+1))*2);
	    isPlaneHit[i] = (ev.scint.sn_hg[i][1] > CALO_VETOTHD );
	 }
	 else
	 {
	    planeSum[i] = (ev.scint.cont_hg[i][0]* (PLANEEQ/eqHG->GetMPVFactor(OFFSET+2*i)) +
			   ev.scint.cont_hg[i][1]* (PLANEEQ/eqHG->GetMPVFactor(OFFSET+2*i+1)));
	    isPlaneHit[i] = ( (ev.scint.sn_hg[i][0] > CALO_VETOTHD) ||
			      (ev.scint.sn_hg[i][1] > CALO_VETOTHD)   );
	 }
	 
      }


      // hit on lyso
      OFFSET = NTRIGSCINT*2+NSCINTPLANES*2+NVETOSCINT*2; 
      for(int i = 0; i<NLYSOCRYSTALS; i++)
      {
	 isLysoHit[i] =(ev.lyso.sn_lg[i][0] > CALO_VETOTHD);
	 lysoCrystal[i] = (ev.lyso.cont_lg[i][0] * (PLANEEQ/eqLG->GetMPVFactor(OFFSET+i)));
      }
      
      // hit on bottom veto
      isBotVetoHit = ev.isBotVetoHit(CALO_VETOTHD);


      // P5se booleans
      isP5seHit3 = (ev.scint.sn_hg[4][0] > 3);
      isP5seHit5 = (ev.scint.sn_hg[4][0] > 5);

      // is a PMT saturated?
      isaTrigPMTSat = ev.trig.CheckSaturation();
      isaScintPMTSat = ev.scint.CheckSaturation();
      isaLysoPMTSat = ev.lyso.CheckSaturation();
      isaVetoPMTSat = ev.veto.CheckSaturation();

      // level 2 variables
      LastPlane = ev.lastPlaneHit(CALO_SIGTHD);
      TrigMult = ev.triggerMultiplicity(CALO_SIGTHD);
      PlaneMult = ev.planeMultiplicity(CALO_SIGTHD);
      LysoMult = ev.lysoMultiplicity(CALO_SIGTHD);

      if(mostSigPad > -1)
      {
	 UPcaloTrigSignal = CaloSum(ev, eqHG) + mostSigPadSignal;
	 UPcaloTrigEnergy = (CaloSum(ev, eqHG) + mostSigPadSignal)*KCALOFITVINC + OFFSETFITVINC;
      }
      else
      {
	 UPcaloTrigSignal = -999;
	 UPcaloTrigEnergy = -999;
      }
      
      UPcaloSignal = CaloSum(ev, eqHG);

      LYSOSignal   = LysoSum(ev, eqLG);
      LYSOEnergy   = LysoSum(ev, eqLG)*KCALOFITLYSO+OFFSETFITLYSO ;

      if(LysoMult > 0)  
	 Etot = UPcaloTrigEnergy + LYSOEnergy;
      else
	 Etot = UPcaloTrigEnergy;

      PlaneToBar = ev.GetMSPlaneToMSBarRatio(PLANETOBAR_THRESH);


      // orbital info variables
      // rig = ev.rig;
      // abstime = ev.abstime;
      // B = ev.B;
      // L = ev.L ;
      // alt = ev.alt;
      // lon = ev.lon;
      // lat = ev.lat;
      // B_x = ev.B_x;
      // B_y = ev.B_y;  
      // B_z = ev.B_z;
      // HEPDaxisDir = HEPDangleStart + HEPDangleStep*(abstime-runStartTime);

      TrackerMSCounts = ev.tracker.GetMSClusterCounts();      
      TrackerMSSN = ev.tracker.GetMSClusterSN();      

      level2->Fill();
 
   }


   fout.Write();
   fout.Close();
   
   return 0;
}




// // new equalization factors 03/10/2018 calculated on the data sample 20180801-20180807
// double mpvHG_trigger[12] = {160.122, 105.319, 223.409, 231.457, 243.364, 281.156, 
// 			       197.559, 287.353, 198.553, 199.217, 159.034, 340.039};

  
// double mpvHG_plane[32] = {129.911, 113.054, 99.708, 95.5058, 140.519, 107.913,
// 			     166.225, 139.717, -1, 77.6728, 74.2986, 80.9133,
// 			     148.674, 133.407, 129.921, 121.277, 87.6364, 124.701,
// 			     97.3621, 76.6955, 94.3065, 138.371, 100.506, 97.9831, 
// 			     132.813, 130.105, 161.09, 202.034, 160.96, 160.264,
// 			     162.938, 140.652 };

   

// double mpvLG_lyso[9] = {313.233, 542.868, 297.211, 440.209, 287.004,
// 			   298.896, 238.178, 297.297, 345.133};












