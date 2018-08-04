#include "detector_const.hh"
#include "analysis_const.hh"

#include "LEvTemp.hh"
#include <iostream>



LEvTemp::LEvTemp(){



 trig_mult=0;

 calo_mult=0;

 lyso_mult=0;

 for(int i=0; i<6; ++i) {
 bars_status[i];
 }

 trig_signal_sum=0;
 
 calo_signal_sum=0;
 
 lyso_signal_sum=0;
  
 for(int i=0; i<16; ++i) {
 plane_signal[i];
 }

 trig_ener_dep=0;

 calo_ener_dep=0;

 lyso_ener_dep=0;

 ener_dep=0;

 IsVetoLatHit=0; 
 
 IsLYSOHit=0; 
 
 IsVetoBottomHit=0;

}//--------------------------------------------------------------------



