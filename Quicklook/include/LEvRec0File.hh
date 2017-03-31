#ifndef __LEVREC0FILE__
#define __LEVREC0FILE__ 1

#include "LEvRec0.hh"

#include "TFile.h"
#include "TTree.h"


class LEvRec0File {
public:
   LEvRec0File(const char *inpFile);
   int SetTheEventPointer(LEvRec0 &event);
   int SetTmdPointer(LEvRec0Md &metaData);

   // bool GetEntry(int iEntry, LEvRec0 &event); // for future... NO ROOT!
   int GetEntry(int iEntry);
   int GetEntries();
   int GetTmdEntries();
   void Close();
   inline bool IsOpen() {return inputCalib->IsOpen();}
   //inline int GetRunId(){return RunId;}; // todo: to be removed?

   unsigned short GetRunID(int iEntry) ;
   unsigned short GetBootNr(int iEntry) ;


   
   ~LEvRec0File();
  
private:
   TFile *inputCalib;
   TTree *treeCalib;
   TTree *Tmd;
   unsigned short RunId;
   
};


#endif
