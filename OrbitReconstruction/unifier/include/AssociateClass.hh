/*
 *
 *
 *  Created on: May 14, 2010
 *      Author: 
 */

#include <sstream>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <string.h>
#include <string>
#include <vector>


#include <TApplication.h>
#include <TBox.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TTree.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TSystemDirectory.h>

#include <TEveTrack.h>
#include <TEveTrackPropagator.h>
#include <TGButton.h>
#include <TEveGeoNode.h>
#include <TGTextEntry.h>
#include <TGProgressBar.h>
#include <TEveBoxSet.h>
#include <TGeoMaterial.h>
#include <TGeoMedium.h>
#include <TGComboBox.h>
#include <TGLViewer.h>
#include <TEveBrowser.h>
#include <TEveStraightLineSet.h>
#include <TGColorDialog.h>
#include <TGeoManager.h>
#include <TGeoVolume.h>
#include <TGClient.h>
#include <TEveManager.h>
#include <TGIcon.h>
#include <TLatex.h>
#include <TGLabel.h>
#include <TPaveText.h>
#include <TLine.h>
#include <TEllipse.h>
#include <TGraphErrors.h>
#include <TGaxis.h>
#include <TLeaf.h>



#include <TChain.h>
#include <TEveRGBAPalette.h>
#include <TEveRGBAPaletteOverlay.h>
#include <TGButtonGroup.h>
#include <TGFrame.h>
#include <TGMsgBox.h>
#include <TGPicture.h>
#include <TGTable.h>
#include <TGeoMatrix.h>
#include <TGeometry.h>
#include <TMath.h>
#include <TObjectTable.h>
#include <TRandom3.h>
#include <TRint.h>
#include <TRootBrowser.h>
#include <TRootEmbeddedCanvas.h>
#include <TROOT.h>
#include <TGraph.h>
#include <TTimeStamp.h>
#include <TString.h>

#include "LOrientationInfo.hh"

struct AOCC_struct_t
{
   unsigned int     sec;
   unsigned int     us;
   unsigned char    x_coord[3];
   unsigned char    y_coord[3];
   unsigned char    z_coord[3];
   unsigned char    roll_angle[3];
   unsigned char    pitch_angle[3];
   unsigned char    yaw_angle[3];
   unsigned short   roll_angl_vel;
   unsigned short   pitch_angl_vel;
   unsigned short   yaw_angl_vel;
   unsigned char    attitude_det;
   unsigned char    magn_torquer;
   unsigned short   magn_torquer_pulse[3];
   unsigned int     q1;
   unsigned int     q2;
   unsigned int     q3;
} ;


class AssociateClass{

private:

public:

  TFile* fout_out = NULL;
  //TTree* fTree = NULL;
  AOCC_struct_t AOCC;
  UInt_t n_abstime;
   Double_t rig, B, lon, lat, alt, L, n_rig, n_B, n_L, n_alt, n_lat, n_lon,B_x,B_y,B_z,n_B_x,n_B_y,n_B_z;
   Double_t HEPDaxisDir;

  UInt_t event_index,trigger_index,hepd_time,PMTBoard_trigger_counter,alive_time,dead_time;

  UShort_t orbitZone;
  UInt_t OBDH_info, abstime;

  TTree* fTree;
  TTree* fTreeMD;
  TTree* fTreeOrb;



  //TString filename="";

  void Associate(const char* infileL2,const char* outfileL2); //inc = incremento!
  double HEPDangle(AOCC_struct_t AOCC);    
 
  //ClassDef(GeoManagerClass,1);

};
//};

//#endif

