/**
 * \author Matteo Martucci
 *
 *
 */

#ifndef hepdviewer_h
#define hepdviewer_h

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

#include <TEveManager.h>

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
#include <TGIcon.h>
#include <TLatex.h>
#include <TGLabel.h>

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

#include "RootEvent.hh"


using namespace std;
using namespace TMath;

class HEPDView3D{

public:

  HEPDView3D(); //constructor
  ~HEPDView3D(); // destructor

  Bool_t     SetInputFile(const char* iFilePath, Int_t tmpfirstevent, Int_t tmplastevent);
  Bool_t     SetGeoFile(const char*geomFile);
  void       make_gui();
  void       load_event(Int_t event_id);
  void       update_image(Int_t event_id,const char* geometryFile);
  void       hepd_esd_read();
  void       Fwd();
  void       Bck();
  void       run_hepd(Bool_t resize,const char* geometryFile);
  void       SaveAs();
  void       CompleteText();
  void       TerminateEVE();
  void       stringcopy(TString s1, const TString s2, Int_t from, Int_t to);
  void       GetPalette();
  void       GetLegend();
  void       jumpto();
  void       SetLateralVis1();
  void       SetLateralVis2();
  void       SetLateralVis3();
  void       ResetCameraVis();
  Int_t      GetEventID();
  void       CloseWindow();

  
  TFile *esd_file;
  TTree *esd_tree;
   Int_t event_id;

  Int_t minevent;
  Int_t maxevent;
  FILE *fptr;

  Bool_t ispressed;
  Bool_t flag_trk;
  Bool_t flag_photon;
  Bool_t flag_digi=false;

  TGCheckButton *fchktrk;
  TGCheckButton *fchkph;
  TGCheckButton *f_tof_or_trk1;
  TGCheckButton *f_tof_or_trk2;

  TGButtonGroup* bgr;
  TGRadioButton* fR[3];
  TGRadioButton* fM[2];


  TEveTrackList *gTrackList;

  TEveGeoTopNode* trk2;
  TEveGeoTopNode* trk;
  TEveGeoTopNode* trk1;
  TEveGeoTopNode* trk3;

  UInt_t jentry;


  TGTextEntry *gTextEntry;
  TGHProgressBar *gProgress;
  TGTextEntry *gTextEntry_info;
  TGTextEntry *gTextEntry_info_2;
  TGColorPalette *gPalette;
  TGTextEntry* gTextEntry_pdg_info;
  TGTextEntry* gTextEntry_pdg2_info;
  TGTextEntry *gTextEntry_ntofhit;
  TGTextEntry *gTextEntry_ntrkhit;
  TGTextEntry *gTextEntry_totedep;
  TGTextEntry *gTextEntry_nhit;

  TGTextEntry *gTextEntry_1311;
  TGTextEntry *gTextEntry_1312; 
  TGTextEntry *gTextEntry_1313;


  TGTextEntry *gTextEntry_1321;
  TGTextEntry *gTextEntry_1322;
  TGTextEntry *gTextEntry_1323;


  TGTextEntry *gTextEntry_lateralveto;
  TGTextEntry *gTextEntry_bottomveto;

  TGTextEntry *gTextEntry_angle;

  Int_t n_tof_hit;
  Int_t n_trk_hit;

 
  Bool_t islateral;
  Bool_t istop;

  TEvePointSet* list_markers;
  TEveStraightLineSet* list_tracks;
  TEveBoxSet* list_boxes;

  TEvePointSet* list_primary_marker;
  TEvePointSet* list_trk_marker;
  TEvePointSet* list_calo_marker;


  TGTextEntry *fText4;
  TGComboBox *fCombo;
  TGLViewer *v;
  Double_t totedep;

  Int_t firstevent;
  Int_t lastevent;
  Int_t tracked_eve_number;
  TString filename;
  string filegeometry;
  TString filedigitized;

  const char* geomFile;

  Bool_t isFileDigi;

 

  TEveBrowser *browser;

  const char *iFilePath;
  const char* oFileName;
  const char* geometryFile;
 

  TDirectory * dir;
  Long64_t nentries;
  TTree* fTree;
  RootEvent *Event;
  TBranch *b_Event;
  Int_t nevents;
  TFile *iFile;
  TFile *igFile;
  Long64_t nbytes = 0, nb = 0;
 

  ClassDef(HEPDView3D,0);

private:

  Int_t NCOLOR;
  Int_t color[6] = {kOrange+8,kYellow, kWhite, kGreen, kAzure, kMagenta};
  float log10_paletteMin;
  float log10_paletteMax;

  TGeoManager* geom;
   TGeoManager* ggaps;

  TGeoMaterial *matVacuum;
  TGeoMaterial *matAl;
  TGeoMedium *Vacuum;
  TGeoMedium *Al;

  TGeoVolume *world_tof;
  TGeoVolume *top;
  TGeoVolume *world_trk;

};

#endif
