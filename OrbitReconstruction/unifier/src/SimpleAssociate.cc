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

#include <AssociateClass.hh>


using namespace std; 
class AssociateClass;

const char* inputL2=NULL;
const char* outputL2=NULL;


void usage();
void getarg(int argc, char * argv[]);


// ------------------------------------------
// main
// ------------------------------------------
int main(int argc, char * argv[])
{

  getarg(argc,argv);

  AssociateClass plll;

  
  plll.Associate(inputL2,outputL2);
  
  cout << "... done associating ...."<<endl;
    
  return 1;
}

void usage(){

  printf("\nUsage:\n");
  //printf("\n SimpleDigi [ options ] -if TLE in -of output txt file \n");
  printf("\n Options are: \n\n");
  printf(" -h || --help print this help and exit \n");
  printf(" -l2 INPUT-L2 FILE full path of input file (ROOT L2) \n");
  printf(" -o OUTPUT-L2 FILE full path of output file (ROOT L2) \n");
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
      if ( !strcmp(argv[i],"-o") ) {
	if ( argc-1 < i+1 ) {
	  usage();
	  //    throw -3;
	};
	i++;
	found = true;
	outputL2 = argv[i];
  
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
