#ifndef __LEVREC0FILE__
#define __LEVREC0FILE__ "LEvRec0File    ############ "

#include "LEvRec0.hh"

#include "TFile.h"
#include "TTree.h"


class LEvRec0File {
public:
  LEvRec0File();
  LEvRec0File(const char *outFile, LEvRec0 &event, LEvRec0Md &metadata); 
  LEvRec0File(const char *inpFile);
  int SetTheEventPointer(LEvRec0 &event);
  int SetMdPointer(LEvRec0Md &metadata);
  int SetTConfPointer(LEvRec0Conf &dummyPKT);   // Added according to QuickLook
  int SetTHVpmtPointer(LEvRec0HVpmt &HVpkt);   // Added according to QuickLook
  void FillTTree(LEvRec0 &event);
  void FillTmdTree(LEvRec0Md &metaData);
  bool GetEntry(int iEntry, LEvRec0 &event); // for future... NO ROOT!
  void Fill();
  void Write();
  // bool GetEntry(int iEntry, LEvRec0 &event); // for future... NO ROOT!
  int GetEntry(int iEntry);
  int GetMDEntry(int iEntry);

  int GetTConfEntry(int iEntry); // Added according to QuickLook
  int GetTHVpmtEntry(int iEntry); // Added according to QuickLook
  int GetTConfEntries(); // Added according to QuickLook
  int GetTHVpmtEntries(); // Added according to QuickLook
  int GetMDEntries(); // Added according to QuickLook
  int GetEntries();
 
  void Open(const char* inpFile);
  void Reset(void);
  void Close();
  inline bool IsOpen() {return fFile->IsOpen();};
  inline bool IsTConf() {return fFile->GetListOfKeys()->Contains("TConf");};   //Added according to QuickLook
  inline bool IsTHVpmt() {return fFile->GetListOfKeys()->Contains("THVpmt");};   //Added according to QuickLook


  inline int GetRunId(){return RunId;};
  inline int GetBootNr(){return BootNr;};
  inline unsigned short GetRunType(){return runType;};
  inline bool IsZeroSuppressed(){return (GetRunType()==0x36 || GetRunType()==0x0055);};
  inline bool IsVirgin(){return GetRunType()==0x4e;};
  inline bool IsMixed(){return GetRunType()==0x63;};
  inline const char* GetName(){return fFile->GetName();};
  ~LEvRec0File();
  
   
private:

   TFile *fFile;
   TTree *fTree;
   TTree *Tmd; 
   TTree *TConf;
   TTree *THVpmt;
   int RunId;
   int BootNr;
   unsigned short runType;
};


#endif
