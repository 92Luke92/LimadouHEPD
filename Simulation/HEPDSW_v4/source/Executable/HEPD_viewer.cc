#include<TSystem.h>
#include<TString.h>
#include<TFile.h>
#include<TTree.h>
#include<TROOT.h>
#include<TTree.h>
//#include<Rotation3D.h>
#include<TSystem.h>
#include<TMath.h>
#include <TRandom3.h>
#include<TEveTrack.h>
#include<TGTextEntry.h>
#include<TChain.h>
#include<TGProgressBar.h>
#include<TGColorDialog.h>
#include<TRootBrowser.h>
#include<TGButton.h>
#include<TEveManager.h>
#include<TEveBrowser.h>
#include<TCanvas.h>
#include <TApplication.h>
#include <TRootBrowser.h>
//#include <TRint.h>
#include "TApplication.h"
#include <TSystemDirectory.h>


#include <TObjectTable.h>
#include <TGClient.h>
#include <TGButton.h>
#include <TGComboBox.h>
#include <TGLabel.h>
#include <TGTextEntry.h>
#include <TGFrame.h>
#include <TGButtonGroup.h>
#include <TGProgressBar.h>
#include <TGMsgBox.h>
#include <TGComboBox.h>
#include <TEveGeoNode.h>


#include<TGeometry.h>
#include<TGeoVolume.h>
#include<TGeoMaterial.h>
#include<TGeoManager.h>
#include<TGeoMatrix.h>
#include<TGeoMedium.h>


#include"HEPDView3D.hh"

#include<vector>
#include<map>
#include<string>
#include<stdlib.h>



using namespace std;

const char* inputFile=NULL;  //input file (full path)
const char* geomFile=NULL;
Int_t firstevent=0; //n.event to be start from
Int_t lastevent=-1; //n.event to stop at

//
// functions
//


void usage();
void getarg(int argc, char * argv[]);

// ------------------------------------------
// main
// ------------------------------------------
int main(int argc, char * argv[])
{

  HEPDView3D eve = HEPDView3D();

  getarg(argc,argv);

  cout << " STARTING... " << endl;

  TApplication theApp("App", &argc, argv);

  // MAIN INSTRUCTIONS
  //_______________________________________

  
  eve.GetLegend();
  eve.GetPalette();
  eve.SetGeoFile(geomFile);
  eve.SetInputFile(inputFile,firstevent,lastevent);
  eve.run_hepd(0,geomFile);
  

  //_______________________________________

  theApp.Run();

  cout <<" ... DONE " << endl;

  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



void usage(){

  printf("\nUsage:\n");
  printf("\n HEPD_viewer [ options ] -if file-name -gf geometry file-name \n");
  printf("\n Options are: \n\n");
  printf(" -h || --help      print this help and exit \n");
  printf(" -if INPUT-FILE* full path of input file \n");
  printf(" -gf GEOMETRY* (GDML) FILE input \n");
  printf("* = required\n");
  // printf("\n\n\n ");

}


void getarg(int argc, char * argv[]){
  bool ifb = false;
  bool gfb = false;
  if ( argc > 1 ) {
    Int_t i = 0;
    while ( i < argc ) {
      Bool_t found = false;
      //------------------------------------------------------
      if ( !strcmp(argv[i], "-h") || !strcmp(argv[i], "--help") ) {
        usage();
        exit(0);
      };
      //------------------------------------------------------
      if ( !strcmp(argv[i], "-if") ) {
        if ( argc - 1 < i + 1 ) {
          usage();
          //      throw -3;
        };
        i++;
        found = true;
        inputFile = argv[i];
        ifb = true;
      };

      //------------------------------------------------------
      if ( !strcmp(argv[i], "-gf") ) {
        if ( argc - 1 < i + 1 ) {
          usage();
          //        throw -3;
        };
        i++;
        found = true;
        geomFile = argv[i];
        gfb = true;
      };

      if ( !found && i > 0 ) {
        usage();
        printf(" Unknown input number %i, that is \"%s\" \n", i, argv[i]);
        //    throw -22;
        exit(0);
      };
      //
      i++;
    };
    //
  } else {
    //
    // no input parameters exit with error, we need at least one.
    //
    //      throw -1;
    usage();
    exit(0);
  };
  if (!(ifb && gfb)){
    usage();
    exit(0);
  }
}


