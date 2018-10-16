/*
 *
 *
 *  Created on: May 14, 2010
 *      Author: vannucci
 */

#ifndef GEOMANAGERC_H_
#define GEOMANAGERC_H_


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


#include <Tle.h>
#include <SGP4.h>
#include <Observer.h>
#include <CoordGeodetic.h>
#include <CoordTopocentric.h>
#include <geomag.h>

//namespace GeoManagerClass {

  /*
   * Class to define a color palette
   */
  //class GeoManagerClass : public TObject{
  class GeoManagerClass{

  private:

    TFile *f;
    TFile *f_hepd;
    TTree*fTreeMD;
    //    TBranch*OBDH_info_br;
    //TLeaf*leaf;
    //UInt_t sec_OBDH;
    UInt_t OBDH_info;
    TTree* fTree;
    TTree* fTreeOrb; 

  public:

    BField *bfield;
    TTree *tree_out;
    // TFile* fout_out;
    //BField bfield[86399][86399];  



    //TString filename="";


    void PropagateTLE(const char* infileL1, const char* infile);
     void tokenize(const std::string& str, std::vector<std::string>& tokens);
     void RunTle(Tle tle, double start, double end, double inc,const char*infile, const char* infileL1); //inc = incremento!


    //time_t GetEpoch(int year,int month, int day, int hour, int minute, int second);
    UInt_t GetEpoch(UInt_t year,UInt_t month, UInt_t day, UInt_t hour, UInt_t minute, UInt_t second);
    UInt_t GetOBDHstart(TString filename);
    UInt_t GetOBDHstop(TString filename);
    //void CreateTTreeROOT(const char* outROOTfile);
  
    
 
    //ClassDef(GeoManagerClass,1);

  };
//};

#endif
