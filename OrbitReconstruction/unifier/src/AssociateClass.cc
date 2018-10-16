#include "AssociateClass.hh"

#pragma pack(push, 1)

struct CPUtimestamp_t
{
   unsigned int       OBDH;
   unsigned int       GPS;
   unsigned int       AOCC;
   unsigned int       star_sensor; 
} ;

struct OBDH_struct_t 
{
   unsigned int     sec;
   unsigned short   ms;
   unsigned char    NU[2];
} ;

#pragma pack(pop)

void AssociateClass::Associate(const char* infileL2, const char* outfileL2){

 
  TFile* fin = new TFile(infileL2,"read");

  fTree = (TTree*)fin->Get("L2");
  fTreeMD = (TTree*)fin->Get("L2md");
  fTreeOrb = (TTree*)fin->Get("OrbitalInfo");

  fTree->SetBranchAddress("hepd_time",&hepd_time);
  fTreeMD->SetBranchAddress("orbitZone",&orbitZone);

  OBDH_struct_t OBDH;
  fTreeMD->SetBranchAddress("OBDH_info",&OBDH.sec);

  fTreeMD->SetBranchAddress("AOCC_info", &AOCC.sec);
  fTreeOrb->SetBranchAddress("rig",&rig);
  fTreeOrb->SetBranchAddress("abstime",&abstime);
  fTreeOrb->SetBranchAddress("B",&B);
  fTreeOrb->SetBranchAddress("lon",&lon);
  fTreeOrb->SetBranchAddress("lat",&lat);
  fTreeOrb->SetBranchAddress("L",&L);
  fTreeOrb->SetBranchAddress("alt",&alt);
  fTreeOrb->SetBranchAddress("B_x",&B_x);
  fTreeOrb->SetBranchAddress("B_y",&B_y);
  fTreeOrb->SetBranchAddress("B_z",&B_z);

  double HEPDaxisDir;
  TBranch *b_rig = fTree->Branch("rig",&n_rig);
  TBranch *b_abstime = fTree->Branch("abstime",&n_abstime);
  TBranch *b_B = fTree->Branch("B",&n_B);
  TBranch *b_lon = fTree->Branch("lon",&n_lon);
  TBranch *b_lat = fTree->Branch("lat",&n_lat);
  TBranch *b_alt = fTree->Branch("alt",&n_alt);
  TBranch *b_L  = fTree->Branch("L",&n_L);
  TBranch *b_B_x = fTree->Branch("B_x",&n_B_x);
  TBranch *b_B_y = fTree->Branch("B_y",&n_B_y);
  TBranch *b_B_z = fTree->Branch("B_z",&n_B_z);
  TBranch *b_HEPDaxisDir = fTree->Branch("HEPDaxisDir",&HEPDaxisDir);

  UInt_t nentriesT = fTree->GetEntries();
  UInt_t nentriesTlt = fTreeMD->GetEntries();
  UInt_t nentriesOrb = fTreeOrb->GetEntries();

  TMatrixD AA(3,3);
  TMatrixD BB(3,3); 
  TMatrixD CC(3,3);
  TMatrixD HEPD_dir_SO(3,1) ;
  HEPD_dir_SO(0,0) = 0;
  HEPD_dir_SO(1,0) = 0; 
  HEPD_dir_SO(2,0) = -1;

  TMatrixD HEPD_dir_ECF(3,1) ;
  OrientationInfo oi;

  fout_out = new TFile(outfileL2,"recreate");
  TTree *newtree = fTree->CloneTree(0);
  TTree *newtreeMD = fTreeMD->CloneTree(0);

  double HEPDangleStep;
  double HEPDangleStart;
  unsigned int     runDuration;
  unsigned int     runStartTime;
  
  fTree->GetEntry(0);  
  fTreeOrb->GetEntry(0);    // B components of the first event 
  fTreeMD->GetEntry(0);
  runStartTime = hepd_time;  // first event time
  // std::cout << " runstarttime  =  " << hepd_time << std::endl;
  HEPDangleStart = HEPDangle(AOCC);  // first event angle
  // std::cout << " starting angle  =  " <<  HEPDangleStart<< std::endl;

  fTreeMD->GetEntry(1);
  fTreeOrb->GetEntry(fTreeOrb->GetEntries()-1); // B components for the last event
  fTree->GetEntry(fTree->GetEntries()-1); 
  HEPDangleStep = HEPDangle(AOCC) - HEPDangleStart;   // last event angle
  // std::cout << " last angle  =  " << HEPDangle(AOCC)  << std::endl;
  runDuration = hepd_time-runStartTime;          // last event time
  // std::cout << " run duration  =  " << runDuration << std::endl;

  HEPDangleStep /= (double)runDuration;


  for(UInt_t entryMD=0; entryMD<nentriesTlt; entryMD++) {
     fTreeMD->GetEntry(entryMD);
     newtreeMD->Fill();
  }

  for(UInt_t entry=0; entry<nentriesT; entry++) {
    fTree->GetEntry(entry);
      
    double time_s = hepd_time/100000;

    for(UInt_t entryorb=0; entryorb<nentriesOrb; entryorb++) {

      fTreeOrb->GetEntry(entryorb);
      fTreeMD->GetEntry(0);

      UInt_t time = time_s+OBDH.sec;
   
      n_abstime = abstime;
      n_rig = rig;
      n_lat = lat;
      n_lon = lon;
      n_L= L;
      n_alt = alt;
      n_B = B;
      n_B_x = B_x;
      n_B_y = B_y;
      n_B_z = B_z;
      HEPDaxisDir = HEPDangleStart + HEPDangleStep*(hepd_time-runStartTime);

      
      if(time==abstime){

	newtree->Fill();
     
      }
      
    }
  }
  
  
  newtree->AutoSave();
  newtreeMD->AutoSave();
  //newtree->Write("L2");
  //fTreeMD->Write("L2md");
  fout_out->Close();
  fin->Close();
}


double AssociateClass::HEPDangle(AOCC_struct_t AOCC)
{

   double angle;

   TMatrixD A(3,3);
   TMatrixD B(3,3);
   TMatrixD C(3,3);
   TMatrixD HEPD_dir_SO(3,1) ;
   HEPD_dir_SO(0,0) = 0;
   HEPD_dir_SO(1,0) = 0; 
   HEPD_dir_SO(2,0) = -1;

   TMatrixD HEPD_dir_ECF(3,1) ;
   OrientationInfo oi;
   

   oi.SetAttitudeAngles(AOCC.roll_angle, 
			AOCC.pitch_angle, 
			AOCC.yaw_angle);
   
   oi.SetQuaternions(AOCC.q1, 
		     AOCC.q2, 
		     AOCC.q3);
   
   A = oi.SatOrbitToSatBody();
   B = oi.SatBodyToJ2000();
   C = oi.J2000toECF(B, abstime);

   HEPD_dir_ECF = A*HEPD_dir_SO;   // Sat Orbit - Sat Body 
   HEPD_dir_ECF = B*HEPD_dir_ECF;  // Sat Body - J2000
   HEPD_dir_ECF = C*HEPD_dir_ECF;  // J2000 - ECF
   
   double Btot = pow((pow(B_x, 2)+ pow(B_y, 2) + pow(B_z, 2)), 1./2.);
   angle = acos((B_x*HEPD_dir_ECF(0,0) + B_y*HEPD_dir_ECF(1,0) + B_z*HEPD_dir_ECF(2,0)) / Btot ) * TMath::RadToDeg();

   return angle;

}
