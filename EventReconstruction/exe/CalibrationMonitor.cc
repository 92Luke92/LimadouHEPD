#include "LCalibrationMonitor.hh"
#include "LCalibration.hh"
#include "detector_const.hh"
int main(int argc,char **argv){
  if(argc==3){

    double err_pmt_LG[NPMT];
    double  err_pmt_HG[NPMT];
    double  err_trk[NCHAN];
    LCalibration* average=CalculateWindow(argv[1],err_pmt_LG, err_pmt_HG, err_trk,true);
  
    LCalibration* latest=LCalibration::ReadROOT(argv[2]);
    if(IsGood(average,err_pmt_LG, err_pmt_HG, err_trk,latest)) std::cout<<"The calibration is compatible"<<std::endl;
    else std::cout<<"The selected calibration is not compatible with the previous"<<std::endl;
    IsGoodGraphics(average,err_pmt_LG, err_pmt_HG, err_trk,latest);
    return 1;
  }
  else{
    std::cout<<"Please type: .CalibrationMonitor <filelist> <file to test>"<<std::endl;

    return 0;
  }
}
