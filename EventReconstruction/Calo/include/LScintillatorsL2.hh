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


  void input_calib_pars(double *teq, double *equ, double *leq);
  int DeviceStatus(double sn1, double sn2);
  int Calc(int hw_condition, int ascii_dump);//hw_condition = 0 all pmts; =1 no pmt 5east
  int Test(int hw_cond) {return Calc(hw_cond, 1);}
  int Filter(int type, double *argums);//
 
  LScintillatorsL2(LScintillatorsL2 const&) = delete;
  void operator=(LScintillatorsL2 const&) = delete;

  

};




#endif
