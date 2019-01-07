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

const int    NBINS = 4096/4;
const double XMIN = 0.;
const double XMAX = 4096.;
const double NPMTT = 63;

const double BINPMT = 4100;
// const double BINPMT = 4100;
const double CALO_SIGTHD = 5;
const double CALO_VETOTHD = 5;
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

const double PLANETOBAR_THRESH = 5.;

const int    PLANEINTRIG = 2;
const double TRIGEQ = 100.;
const double PLANEEQ = 200.;

const int    SIGTHD3 = 3;
const int    SIGTHD5 = 5;
const int    SIGTHD10 = 10;

typedef struct trigPaddle
{
   int    pad;
   double signal; 
}trigPaddle_t;


double CaloSum(LEvRec1 ev, LCaloEqualization *eqHG, int thd);
double CaloSum_lg(LEvRec1 ev, LCaloEqualization *eqHG, int thd);
double LysoSum(LEvRec1 ev, LCaloEqualization *eqLG, int thd);



double CaloSum(LEvRec1 ev, LCaloEqualization *eqHG, int thd)
{
   int OFFSET=NTRIGSCINT*2;
   double sum=0;
   
   for (int i=0; i<NSCINTPLANES;i++)
   {
      if (i == 4 && ev.scint.sn_hg[i][1] >= thd) // P5se correction
	 sum += ((ev.scint.cont_hg[i][1]*(PLANEEQ/eqHG->GetMPVFactor(OFFSET+2*i+1)))*2); 
      else
	 if(ev.scint.sn_hg[i][0] >= thd &&
	    ev.scint.sn_hg[i][1] >= thd   )
	    sum += (ev.scint.cont_hg[i][0]*(PLANEEQ/eqHG->GetMPVFactor(OFFSET+2*i)) + 
		    ev.scint.cont_hg[i][1]*(PLANEEQ/eqHG->GetMPVFactor(OFFSET+2*i+1)));
   }
   
   return sum;   
}

double CaloSum_lg(LEvRec1 ev, LCaloEqualization *eqHG, int thd)
{
   int OFFSET=NTRIGSCINT*2;
   double sum=0;
   
   for (int i=0; i<NSCINTPLANES;i++)
   {
      if (i == 4 && ev.scint.sn_lg[i][1] >= thd) // P5se correction
	 sum += ((ev.scint.cont_lg[i][1]*(PLANEEQ/eqHG->GetMPVFactor(OFFSET+2*i+1)))*2); 
      else
	 if(ev.scint.sn_lg[i][0] >= thd &&
	    ev.scint.sn_lg[i][1] >= thd   )
	    sum += (ev.scint.cont_lg[i][0]*(PLANEEQ/eqHG->GetMPVFactor(OFFSET+2*i)) + 
		    ev.scint.cont_lg[i][1]*(PLANEEQ/eqHG->GetMPVFactor(OFFSET+2*i+1)));
   }
   
   return sum;   
}


double LysoSum(LEvRec1 ev, LCaloEqualization *eqLG, int thd)
{
   double sum=0;
   int OFFSET=NTRIGSCINT*2+NSCINTPLANES*2+NVETOSCINT*2; 
   for (int i=0; i<NLYSOCRYSTALS;i++)
      if(ev.lyso.sn_lg[i][0] >= thd)
	 sum += (ev.lyso.cont_lg[i][0]*(PLANEEQ/eqLG->GetMPVFactor(OFFSET+i)));  


   return sum;   
}

 

int main(int argc, char **argv) {
   
   LEvRec1 ev;
   LEvRec1File fin(argv[1],"READ");
   fin.SetTheEventPointer(ev);
   fin.SetMDPointer(ev);

   // eq files and calibration numbers must be read in the reco12manager
   std::string eqHGFileName = "/home/fool/LIMADOU/Analysis/protons/L1_flightEQ/eqHG_MIPcut.txt";
   std::string eqLGFileName = "/home/fool/LIMADOU/Analysis/protons/L1_flightEQ/eqLG_MIPcut.txt";

   LCaloEqualization *eqHG = LCaloEqualization::Read(eqHGFileName);
   LCaloEqualization *eqLG = LCaloEqualization::Read(eqLGFileName);

   eqHG->Dump();
   eqLG->Dump();

   std::string outfile(argv[1]);
   size_t index = outfile.rfind("/");
   if(index != std::string::npos)
      outfile.erase(outfile.begin(), outfile.begin()+index+1);
   index = outfile.find("_L1");
   if(index != std::string::npos)
      outfile.replace(index, 3, "_L2");
   else
      std::cout << "L1 string NOT found in file: " << std::endl;
   
   TFile fout(outfile.c_str(), "recreate"); // NEW

   Double_t         TrackerMSCounts, TrackerMSSN;

   // plane/bar signals
   Double_t         trigSum[NTRIGSCINT];
   Double_t         trigSumMIP[NTRIGSCINT];
   Int_t            mostSigPad;
   Double_t         mostSigPadSignal;
   Double_t         mostSigPadSignalMIP;
   Int_t            mostSigPlane;
   Int_t            mostSigCry;
   Double_t         mostSigCrySignal;
   Double_t         mostSigCryTo2ndMostRatio;
   Int_t            secondMostSigCry;
   Double_t         planeSum[NSCINTPLANES];
   Double_t         lysoCrystal[NLYSOCRYSTALS];

   // hit planes
   Bool_t           isTrigHit[NTRIGSCINT][3];  // 3, 5, 10 sigma
   Bool_t           isPlaneHit[NSCINTPLANES][3]; // 3, 5, 10 sigma
   Bool_t           isLysoHit[NLYSOCRYSTALS][3]; // 3, 5, 10 sigma

   // selections
   Bool_t           offlineTrig[3]; // 3, 5, 10 sigma
   Bool_t           isAutoVeto[3]; // 3, 5, 10 sigma
   Bool_t           isLateralVetoHit[3];  // 3, 5, 10 sigma
   Bool_t           isBotVetoHit[3]; // 3, 5, 10 sigma
   Bool_t           isP5seHit[3]; // 3, 5, 10 sigma

   // saturated PMTs
   Bool_t           isaPMTSat = false;
   Bool_t           isaTrigPMTSat;  
   Bool_t           isaScintPMTSat;   
   Bool_t           isaLysoPMTSat;   
   Bool_t           isaVetoPMTSat;

   // L2 variables 
   Double_t         UPcaloTrigEnergy[3]; // 3, 5, 10 sigma
   Double_t         UPcaloSignal[3];  // 3, 5, 10 sigma
   Double_t         LYSOSignal[3]; // 3, 5, 10 sigma
   Double_t         LYSOEnergy[3]; // 3, 5, 10 sigma
   Double_t         Etot[3]; // 3, 5, 10 sigma
   Int_t            LastPlane[3]; // 3, 5, 10 sigma 
   Double_t         lPtoSlPRatio[3]; // 3, 5, 10 sigma
   Double_t         Etot_R[3]; // 3, 5, 10 sigma

   Double_t         etot_range[31] = {32.5, 43.75, 50.5, 56.75, 62.5, 67.5,
				      72.5, 77.25, 81.75, 86.25, 90.25, 94.5,
				      98., 102., 105.75, 109., 112.5, 116.,
				      119.5, 122.75, 125.75, 128.75, 132.25, 135.,
				      138., 140.75, 144., 146.75, 149.5, 152.5,
				      155 };
   
   // sub-detector multiplicitys
   Int_t            TrigMult[3];
   Int_t            PlaneMult[3];
   Int_t            LysoMult[3];

   Double_t         PlaneToBar;

   Double_t         trigSumLG[NTRIGSCINT];
   Double_t         planeSumLG[NSCINTPLANES];
   Double_t         UPcaloSignalLG[3];  // 3, 5, 10 sigma

   Double_t         trigSigHG[NTRIGSCINT][2];
   Double_t         planeSigHG[NSCINTPLANES][2];

   Double_t         trigSigLG[NTRIGSCINT][2];
   Double_t         planeSigLG[NSCINTPLANES][2];

   
   // orbital info
   Double_t         rig, B, alt, lat, lon, L, B_x, B_y, B_z;
   unsigned int     abstime;
   

   trigPaddle_t     Etrig;
   trigPaddle_t     EtrigMIP;

   
   unsigned short   boot_nr;
   unsigned short   run_id;
   unsigned int     event_index;
   unsigned int     alive_time;
   unsigned int     dead_time;
   unsigned int     hepd_time;
   unsigned int     startRunTime;
   unsigned int     stopRunTime;

   
   LEvRec0Md        lev0MD;

   // L2 meta data Tree
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


   // L2 Tree
   TTree * level2 = new TTree("L2","L2");

   level2->Branch("boot_nr", &boot_nr);
   level2->Branch("run_id", &run_id);
   level2->Branch("hepd_time", &hepd_time);
   level2->Branch("startRunTime", &startRunTime);
   level2->Branch("stopRunTime", &stopRunTime);
   level2->Branch("event_index", &event_index);
   level2->Branch("alive_time", &alive_time);
   level2->Branch("dead_time", &dead_time);

   level2->Branch("TrackerMSCounts", &TrackerMSCounts, "TrackerMSCounts/D");
   level2->Branch("TrackerMSSN", &TrackerMSSN, "TrackerMSSN/D");

   level2->Branch("trigSumSig[6]", &trigSum[0]);
   level2->Branch("trigSumMIP[6]", &trigSumMIP[0]);
   level2->Branch("mostSigPad", &mostSigPad);
   level2->Branch("mostSigPadSignal", &mostSigPadSignal);
   level2->Branch("mostSigPadSignalMIP", &mostSigPadSignalMIP);
   level2->Branch("planeSumSig[16]", &planeSum[0]);
   level2->Branch("mostSigPlane", &mostSigPlane);
   level2->Branch("lysoCrystalSig[9]", &lysoCrystal[0]);
   level2->Branch("mostSigCry", &mostSigCry);
   level2->Branch("mostSigCrySignal", &mostSigCrySignal);
   level2->Branch("mostSigCryTo2ndMostRatio", &mostSigCryTo2ndMostRatio);
      
   level2->Branch("isTrigHit[6][3]", &isTrigHit[0][0]);
   level2->Branch("isPlaneHit[16][3]", &isPlaneHit[0][0]);
   level2->Branch("isLysoHit[9][3]", &isLysoHit[0][0]);

   level2->Branch("offlineTrigger[3]", &offlineTrig[0]);
   level2->Branch("isAutoVeto[3]", &isAutoVeto[0]);
   level2->Branch("isLatVetoHit[3]", &isLateralVetoHit[0]);
   level2->Branch("isBotVetoHit[3]", &isBotVetoHit[0]); 
   level2->Branch("isP5seHit[3]", &isP5seHit[0]);
   //level2->Branch("isP5seHit5", &isP5seHit5);
   level2->Branch("isaPMTSat", &isaPMTSat);
   level2->Branch("isaTrigPMTSat", &isaTrigPMTSat);
   level2->Branch("isaScintPMTSat", &isaScintPMTSat);
   level2->Branch("isaLysoPMTSat", &isaLysoPMTSat);
   level2->Branch("isaVetoPMTSat", &isaVetoPMTSat);

   level2->Branch("UPcaloTrigEnergy[3]", &UPcaloTrigEnergy[0]);
   level2->Branch("UPcaloSignal[3]", &UPcaloSignal[0]);
   level2->Branch("LYSOSignal[3]", &LYSOSignal[0]);
   level2->Branch("LYSOEnergy[3]", &LYSOEnergy[0]);
   level2->Branch("Etot[3]", &Etot[0]);
   level2->Branch("lastPlane[3]", &LastPlane[0]);
   level2->Branch("lPtoSlPRatio[3]", &lPtoSlPRatio[0]);
   level2->Branch("Etot_R[3]", &Etot_R[0]);

   level2->Branch("trigMult[3]", &TrigMult[0]);
   level2->Branch("planeMult[3]", &PlaneMult[0]);
   level2->Branch("lysoMult[3]", &LysoMult[0]);

   level2->Branch("planeToBar", &PlaneToBar, "PlaneToBar/D");

   level2->Branch("trigSumSigLG[6]", &trigSumLG[0]);
   level2->Branch("planeSumSigLG[16]", &planeSumLG[0]);
   level2->Branch("UPcaloSignalLG[3]", &UPcaloSignalLG[0]);
  
   level2->Branch("trigSigHG[6][2]", &trigSigHG[0][0]);
   level2->Branch("planeSigHG[16][2]", &planeSigHG[0][0]);

   level2->Branch("trigSigLG[6][2]", &trigSigLG[0][0]);
   level2->Branch("planeSigLG[16][2]", &planeSigLG[0][0]);
   


   int nent = fin.GetEntries();
   fin.GetEntry(0);   
   fin.GetMDEntry(0);
   std::cout << " entries = " << nent << std::endl;

   // filling meta data Tree
   for(int imd=0; imd<fin.GetMDEntries() ; imd++)
   {
      fin.GetMDEntry(imd);
      lev0MD=ev.lev0MD;
      level2MD->Fill();
   }

   double trigSignal = 0;
   bool isHG=true;	      
   int OFFSET = 0;
   int  index_Erange[3];


   // event loop
   for(int iev=0; iev<nent ; iev++)
   {
      fin.GetEntry(iev);

      if(iev%100000 == 0)
	 std::cout << ((double)iev/(double)nent)*100 << "%" << std::endl;
      
      mostSigPad =  ev.trig.GetTheMostSignificantUnit(isHG, CALO_SIGTHD);
      mostSigPlane =  ev.GetTheMostSignificantPlane();
      mostSigCry =  ev.lyso.GetTheMostSignificantUnit(!isHG, CALO_SIGTHD);
      
      if(mostSigPad > -1 )
      {
	 trigSignal = (ev.trig.cont_hg[mostSigPad][0] * 
		       (TRIGEQ/eqHG->GetMPVFactor(2*mostSigPad)) + 
		       ev.trig.cont_hg[mostSigPad][1] * 
		       (TRIGEQ/eqHG->GetMPVFactor(2*mostSigPad+1)));
	 
	 mostSigPadSignal = trigSignal;
	 mostSigPadSignalMIP = (ev.trig.cont_hg[mostSigPad][0]/eqHG->GetMPVFactor(2*mostSigPad) + 
				ev.trig.cont_hg[mostSigPad][1]/eqHG->GetMPVFactor(2*mostSigPad+1) )/2.;
	 
	 isAutoVeto[0] = ev.AutoVeto(SIGTHD3, mostSigPad, isHG);
	 isAutoVeto[1] = ev.AutoVeto(SIGTHD5, mostSigPad, isHG);
	 isAutoVeto[2] = ev.AutoVeto(SIGTHD10, mostSigPad, isHG);
	 
      }
      else
      {
	 mostSigPadSignal = -999;
	 mostSigPadSignalMIP = -999;
      }


      boot_nr = ev.boot_nr;
      run_id = ev.run_id;
      event_index = ev.event_index;
      alive_time = ev.alive_time;
      dead_time = ev.dead_time;
      hepd_time = ev.hepd_time;


      // trigger signals
      for(int i = 0; i<NTRIGSCINT; i++)
      {
	 trigSum[i] = (ev.trig.cont_hg[i][0] * (TRIGEQ/eqHG->GetMPVFactor(2*i)) +
		       ev.trig.cont_hg[i][1] * (TRIGEQ/eqHG->GetMPVFactor(2*i+1)));

	 trigSumLG[i] = (ev.trig.cont_lg[i][0] * (TRIGEQ/eqHG->GetMPVFactor(2*i)) +
			 ev.trig.cont_lg[i][1] * (TRIGEQ/eqHG->GetMPVFactor(2*i+1)));
	 
	 
	 trigSumMIP[i] = (ev.trig.cont_hg[i][0] /eqHG->GetMPVFactor(2*i) +    
			  ev.trig.cont_hg[i][1] /eqHG->GetMPVFactor(2*i+1))/2.;

	 isTrigHit[i][0] = ( (ev.trig.sn_hg[i][0] > SIGTHD3) &&
			     (ev.trig.sn_hg[i][1] > SIGTHD3)   );
	 isTrigHit[i][1] = ( (ev.trig.sn_hg[i][0] > SIGTHD5) &&
			     (ev.trig.sn_hg[i][1] > SIGTHD5)   );
	 isTrigHit[i][2] = ( (ev.trig.sn_hg[i][0] > SIGTHD10) &&
			     (ev.trig.sn_hg[i][1] > SIGTHD10)   );

	 for(int j =0; j<2; j++)
	 {
	    trigSigHG[i][j] = ev.trig.cont_hg[i][j]*(TRIGEQ/eqHG->GetMPVFactor(2*i+j));
	    trigSigLG[i][j] = ev.trig.cont_lg[i][j]*(TRIGEQ/eqHG->GetMPVFactor(2*i+j));
	 }
      }

      // plane signals
      OFFSET = NTRIGSCINT*2;
      for(int i = 0; i<NSCINTPLANES; i++)
      {
      	 if(i==4)
	 {
	    planeSum[i] = (ev.scint.cont_hg[i][1]*
			   (PLANEEQ/eqHG->GetMPVFactor(OFFSET+2*i+1))*2);
	    planeSumLG[i] = (ev.scint.cont_lg[i][1]*
			     (PLANEEQ/eqHG->GetMPVFactor(OFFSET+2*i+1))*2);
	    
	    isPlaneHit[i][0] = (ev.scint.sn_hg[i][1] > SIGTHD3 );
	    isPlaneHit[i][1] = (ev.scint.sn_hg[i][1] > SIGTHD5 );
	    isPlaneHit[i][2] = (ev.scint.sn_hg[i][1] > SIGTHD10 );
	 }
	 else
	 {
	    planeSum[i] = (ev.scint.cont_hg[i][0]*
			   (PLANEEQ/eqHG->GetMPVFactor(OFFSET+2*i)) +
			   ev.scint.cont_hg[i][1]*
			   (PLANEEQ/eqHG->GetMPVFactor(OFFSET+2*i+1)));
	    planeSumLG[i] = (ev.scint.cont_lg[i][0]*
			     (PLANEEQ/eqHG->GetMPVFactor(OFFSET+2*i)) +
			     ev.scint.cont_lg[i][1]*
			     (PLANEEQ/eqHG->GetMPVFactor(OFFSET+2*i+1)));
	    
	    isPlaneHit[i][0] = ( (ev.scint.sn_hg[i][0] > SIGTHD3) &&
				 (ev.scint.sn_hg[i][1] > SIGTHD3)   );
	    isPlaneHit[i][1] = ( (ev.scint.sn_hg[i][0] > SIGTHD5) &&
				 (ev.scint.sn_hg[i][1] > SIGTHD5)   );
	    isPlaneHit[i][2] = ( (ev.scint.sn_hg[i][0] > SIGTHD10) &&
				 (ev.scint.sn_hg[i][1] > SIGTHD10)   );
	 }
	 for(int j =0; j<2; j++)
	 {
	    planeSigHG[i][j] = ev.scint.cont_hg[i][j]*
	       (PLANEEQ/eqHG->GetMPVFactor(OFFSET+2*i+j));
	    planeSigLG[i][j] = ev.scint.cont_lg[i][j]*
	       (PLANEEQ/eqHG->GetMPVFactor(OFFSET+2*i+j));
	 }
      }


      // lyso signals
      OFFSET = NTRIGSCINT*2+NSCINTPLANES*2+NVETOSCINT*2; 
      for(int i = 0; i<NLYSOCRYSTALS; i++)
      {
	 lysoCrystal[i] = (ev.lyso.cont_lg[i][0]*(PLANEEQ/eqLG->GetMPVFactor(OFFSET+i)));

	 isLysoHit[i][0] = (ev.lyso.sn_lg[i][0] > SIGTHD3);
	 isLysoHit[i][1] = (ev.lyso.sn_lg[i][0] > SIGTHD5);
	 isLysoHit[i][2] = (ev.lyso.sn_lg[i][0] > SIGTHD10);

      }
      if(mostSigCry > -1)
      {
	 mostSigCrySignal = lysoCrystal[mostSigCry];
	 if(secondMostSigCry > -1)
	 {
	    secondMostSigCry = ev.lyso.GetThe2ndMostSignificantUnit(!isHG, CALO_SIGTHD);
	    mostSigCryTo2ndMostRatio =
	       lysoCrystal[mostSigCry]/lysoCrystal[secondMostSigCry];
	 }
	 else
	 {
	    secondMostSigCry = -999;
	    mostSigCryTo2ndMostRatio = -999;
	 }
      }
      else
	 mostSigCrySignal = -999;
	 
      // no lat veto hit
      isLateralVetoHit[0] = ev.isLatVetoHit(SIGTHD3, isHG);
      isLateralVetoHit[1] = ev.isLatVetoHit(SIGTHD5, isHG);
      isLateralVetoHit[2] = ev.isLatVetoHit(SIGTHD10, isHG);
      
      // pre selections
      offlineTrig[0] = ev.PreSelection(SIGTHD3, mostSigPad, PLANEINTRIG, isHG);
      offlineTrig[1] = ev.PreSelection(SIGTHD5, mostSigPad, PLANEINTRIG, isHG);
      offlineTrig[2] = ev.PreSelection(SIGTHD10, mostSigPad, PLANEINTRIG, isHG);

      // hit on bottom veto
      isBotVetoHit[0] = ev.isBotVetoHit(SIGTHD3, isHG);
      isBotVetoHit[1] = ev.isBotVetoHit(SIGTHD5, isHG);
      isBotVetoHit[2] = ev.isBotVetoHit(SIGTHD10, isHG);


      // P5se booleans
      isP5seHit[0] = (ev.scint.sn_hg[4][0] > SIGTHD3);
      isP5seHit[1] = (ev.scint.sn_hg[4][0] > SIGTHD5);
      isP5seHit[2] = (ev.scint.sn_hg[4][0] > SIGTHD10);


      // is a PMT saturated?
      isaTrigPMTSat = ev.trig.CheckSaturation();
      isaScintPMTSat = ev.scint.CheckSaturation();
      isaLysoPMTSat = ev.lyso.CheckSaturation();
      isaVetoPMTSat = ev.veto.CheckSaturation();
      if(isaTrigPMTSat || isaScintPMTSat || isaLysoPMTSat) // if saturation -> wrong energy reconstruction
	 isaPMTSat = true;
      
      // level 2 variables
      LastPlane[0] = ev.lastPlaneHit(SIGTHD3);
      LastPlane[1] = ev.lastPlaneHit(SIGTHD5);
      LastPlane[2] = ev.lastPlaneHit(SIGTHD10);

      for(int i=0; i<3; i++)
      {
	 if(LastPlane[i] >= 1 && LastPlane[i]<16)
	    lPtoSlPRatio[i] = planeSum[LastPlane[i]]/planeSum[LastPlane[i]-1];
	 else
	    lPtoSlPRatio[i] = -999;
      }
      
      // energy from range
      for(int i=0; i<3; i++)
      {
	 if(LastPlane[i]>=16)
	    Etot_R[i] = 0;
	 else
	 {
	    index_Erange[i] = 2*LastPlane[i];
	    if(lPtoSlPRatio[i] < 1 && lPtoSlPRatio[i] > -1 )
	       index_Erange[i]--;
	    Etot_R[i] = etot_range[index_Erange[i]];
	 }
      }
      
      TrigMult[0] = ev.triggerMultiplicity(SIGTHD3);
      TrigMult[1] = ev.triggerMultiplicity(SIGTHD5);
      TrigMult[2] = ev.triggerMultiplicity(SIGTHD10);
      PlaneMult[0] = ev.planeMultiplicity(SIGTHD3);
      PlaneMult[1] = ev.planeMultiplicity(SIGTHD5);
      PlaneMult[2] = ev.planeMultiplicity(SIGTHD10);
      LysoMult[0] = ev.lysoMultiplicity(SIGTHD3);
      LysoMult[1] = ev.lysoMultiplicity(SIGTHD5);
      LysoMult[2] = ev.lysoMultiplicity(SIGTHD10);

      if(mostSigPad > -1)
      {
	 UPcaloTrigEnergy[0] = (CaloSum(ev, eqHG, SIGTHD3)+mostSigPadSignal)*KCALOFITVINC +
	    OFFSETFITVINC;
	 UPcaloTrigEnergy[1] = (CaloSum(ev, eqHG, SIGTHD5)+mostSigPadSignal)*KCALOFITVINC +
	    OFFSETFITVINC;
	 UPcaloTrigEnergy[2] = (CaloSum(ev, eqHG, SIGTHD10)+mostSigPadSignal)*KCALOFITVINC +
	    OFFSETFITVINC;
      }
      else
      {
	 UPcaloTrigEnergy[0] = -999;
	 UPcaloTrigEnergy[1] = -999;
	 UPcaloTrigEnergy[2] = -999;
      }
      
      UPcaloSignal[0] = CaloSum(ev, eqHG, SIGTHD3);
      UPcaloSignal[1] = CaloSum(ev, eqHG, SIGTHD5);
      UPcaloSignal[2] = CaloSum(ev, eqHG, SIGTHD10);

      UPcaloSignalLG[0] = CaloSum_lg(ev, eqHG, SIGTHD3);
      UPcaloSignalLG[1] = CaloSum_lg(ev, eqHG, SIGTHD5);
      UPcaloSignalLG[2] = CaloSum_lg(ev, eqHG, SIGTHD10);
	 
      LYSOSignal[0]   = LysoSum(ev, eqLG, SIGTHD3);
      LYSOSignal[1]   = LysoSum(ev, eqLG, SIGTHD5);
      LYSOSignal[2]   = LysoSum(ev, eqLG, SIGTHD10);
	    
      LYSOEnergy[0]   = LYSOSignal[0] * KCALOFITLYSO + OFFSETFITLYSO ;
      LYSOEnergy[1]   = LYSOSignal[1] * KCALOFITLYSO + OFFSETFITLYSO ;
      LYSOEnergy[2]   = LYSOSignal[2] * KCALOFITLYSO + OFFSETFITLYSO ;

      for(int i=0; i<3; i++)
      {
	 if(LysoMult[i] > 0)  
	    Etot[i] = UPcaloTrigEnergy[i] + LYSOEnergy[i];
	 else
	    Etot[i] = UPcaloTrigEnergy[i];
      }


      if(mostSigPlane > -1)
	 PlaneToBar = planeSum[mostSigPlane]/trigSum[mostSigPad];


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












