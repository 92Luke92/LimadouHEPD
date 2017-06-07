#ifndef __LScintillatorsL2__
#define __LScintillatorsL2__ "LScintillatorsL2  ########## "
#include "LEvRec1.hh"
#include "LEvRec1File.hh"

class LScintillatorsL2 {

private:
  LScintillatorsL2();
  ~LScintillatorsL2();

 

  LEvRec1File* RunFile;
 

  
 

 public:
  static LScintillatorsL2& GetInstance();
  int LoadRun(const std::string inputFileROOT);
  
  int Test(int bar);
  int Calc();


  LScintillatorsL2(LScintillatorsL2 const&) = delete;
  void operator=(LScintillatorsL2 const&) = delete;

  

};




#endif
