#include "TROOT.h"
#include <TTimeStamp.h>
#include <TDatime.h>
#include <TFile.h>
#include <TTree.h>
#include "TString.h"
#include "TObjString.h"
//#include "sgp4.h"
//#include <sys/stat.h>
#include <fstream>
#include <iostream>
#include <iomanip>

#include <CoordTopocentric.h>
#include <CoordGeodetic.h>
#include <Observer.h>
#include <SGP4.h>
#include <Tle.h>

#include <GeoManagerClass.hh>
#include <geomag.h>

using namespace std; 
class GeoManagerClass;

const char* inputTLE=NULL;  //input file (full path)
//const char* outputL1=NULL;
const char* inputL2=NULL;
const char* inputFolder=NULL;  //input file (full path)

void usage();
void getarg(int argc, char * argv[]);


// ------------------------------------------
// main
// ------------------------------------------
int main(int argc, char * argv[])
{

  getarg(argc,argv);

  GeoManagerClass plll;

  cout << " launching propagate TLE " << endl;

  plll.PropagateTLE(inputL2,inputTLE);
  //plll.Associate(inputL2,outputL1);

  cout << "... done propagating TLE ... Importing in ROOT ...."<<endl;
    
  return 1;
}

void usage(){

  printf("\nUsage:\n");
  //printf("\n SimpleDigi [ options ] -if TLE in -of output txt file \n");
  printf("\n Options are: \n\n");
  printf(" -h || --help print this help and exit \n");
  printf(" -l2 INPUT-L2 FILE full path of input file (ROOT L2) \n");
  //printf(" -olev OUTPUT-L1 FILE full path of output file (ROOT L1) \n");
  printf(" -tle INPUT-TLE full path of TLE file \n");
  printf("\n\n\n ");

}


void getarg(int argc, char * argv[]){

  if ( argc > 1 ){
    Int_t i = 0;
    while ( i < argc ){
      Bool_t found = false;
      //------------------------------------------------------
      if ( !strcmp(argv[i],"-h") || !strcmp(argv[i],"--help") ){
	usage();
	exit(0);
      };
      //------------------------------------------------------
      if ( !strcmp(argv[i],"-l2") ) {
        if ( argc-1 < i+1 ) {
          usage();
          //    throw -3;
        };
        i++;
        found = true;
        inputL2 = argv[i];

      };

      //------------------------------------------------------
    //  if ( !strcmp(argv[i],"-olev") ) {
    //    if ( argc-1 < i+1 ) {
    //      usage();
    //      //    throw -3;
    //    };
    //    i++;
    //    found = true;
    //    outputL1 = argv[i];
    //
    //  };
      //------------------------------------------------------
      if ( !strcmp(argv[i],"-tle") ) {
	if ( argc-1 < i+1 ) {
	  usage();
	  //    throw -3;
	};
	i++;
	found = true;
	inputTLE = argv[i];

      };

  //------------------------------------------------------
      if ( !strcmp(argv[i],"-tfold") ) {
	if ( argc-1 < i+1 ) 
	  usage();
	i++;
	found = true;
	inputFolder = argv[i];

      };

  //------------------------------------------------------
  
      if ( !found && i > 0 ){
	usage();
	printf(" Unknown input number %i, that is \"%s\" \n",i,argv[i]);
	//  throw -22;
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

}
