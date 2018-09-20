/*
 * alwayslep.cc
 *
 *
 *
 */


#include "ldsimplevars.hh"
#include "array"
#include "LStatTools.hh"


std::vector<float> ldsimpleVars::varShaper(const LEvRec1Stream* L1ev) {

   std::vector<float> shapedVars;
   std::array<float, NSCINTPLANES> splane;

   float sum=0;
   for (int i=0; i<NSCINTPLANES; i++) {
      float thisplane=L1ev->scint_snHG[2*i]+L1ev->scint_snHG[2*i+1];
      splane[i]=thisplane;
      sum+=thisplane;
   }
   const float mean= sum/NSCINTPLANES;

   float curmin=10000;
   float curmax=0;
   float argmin=-1;
   float argmax=-1;
   float runningvar=0;
   int lastlayer=-1;

   for (int i=0; i<NSCINTPLANES; i++) {
      const float val=splane[i];
      if (val>curmax) {
         argmax=i;
         curmax=val;
      }
      if (val<curmin) {
         argmin=i;
         curmin=val;
      }
      runningvar+=(val-mean)*(val-mean);
      if (val>10) lastlayer=i;
   }

   shapedVars[stdev]= sqrt( runningvar/NSCINTPLANES);
   shapedVars[range]=curmax-curmin;
   shapedVars[evol] =lastlayer>-1?splane[lastlayer]-splane[0]:0;
   shapedVars[maxm2]=argmax>1?splane[argmax]-splane[argmax-2]:splane[argmax];

   return shapedVars;

};











float ldsimpleVars::discriminate(std::vector<float> shapedVars) {

    if ( shapedVars[range] <= 2566.06787109375 ) {
   if ( shapedVars[evol] <= -32.23457717895508 ) {
     if ( shapedVars[range] <= 405.9726257324219 ) {
       if ( shapedVars[evol] <= -91.7039794921875 ) {
         if ( shapedVars[maxm2] <= 1423.4638671875 ) {
           return  4541.0 / ( 764.0 + 4541.0 );
         } else {
           return  68.0 / ( 189.0 + 68.0 );
         }
       } else {
         if ( shapedVars[stdev] <= 72.494384765625 ) {
           return  974.0 / ( 310.0 + 974.0 );
         } else {
           return  1399.0 / ( 1752.0 + 1399.0 );
         }
       }
     } else {
       if ( shapedVars[stdev] <= 130.0373992919922 ) {
         if ( shapedVars[maxm2] <= 277.5450744628906 ) {
           return  758.0 / ( 301.0 + 758.0 );
         } else {
           return  1943.0 / ( 145.0 + 1943.0 );
         }
       } else {
         if ( shapedVars[stdev] <= 513.42626953125 ) {
           return  39278.0 / ( 1582.0 + 39278.0 );
         } else {
           return  6760.0 / ( 774.0 + 6760.0 );
         }
       }
     }
   } else {
     if ( shapedVars[evol] <= 612.7119140625 ) {
       if ( shapedVars[maxm2] <= 266.5987854003906 ) {
         if ( shapedVars[stdev] <= 136.1554412841797 ) {
           return  6498.0 / ( 19058.0 + 6498.0 );
         } else {
           return  4388.0 / ( 1747.0 + 4388.0 );
         }
       } else {
         if ( shapedVars[maxm2] <= 1822.142333984375 ) {
           return  34322.0 / ( 10348.0 + 34322.0 );
         } else {
           return  1012.0 / ( 5850.0 + 1012.0 );
         }
       }
     } else {
       if ( shapedVars[maxm2] <= 737.2838134765625 ) {
         if ( shapedVars[maxm2] <= 597.7427978515625 ) {
           return  863.0 / ( 20715.0 + 863.0 );
         } else {
           return  607.0 / ( 2190.0 + 607.0 );
         }
       } else {
         if ( shapedVars[evol] <= 1114.88671875 ) {
           return  2202.0 / ( 579.0 + 2202.0 );
         } else {
           return  1477.0 / ( 1832.0 + 1477.0 );
         }
       }
     }
   }
 } else {
   if ( shapedVars[range] <= 2935.872314453125 ) {
     if ( shapedVars[evol] <= -169.51904296875 ) {
       if ( shapedVars[maxm2] <= 3249.61328125 ) {
         if ( shapedVars[stdev] <= 859.2388305664062 ) {
           return  1365.0 / ( 96.0 + 1365.0 );
         } else {
           return  272.0 / ( 67.0 + 272.0 );
         }
       } else {
         if ( shapedVars[evol] <= -956.055908203125 ) {
           return  123.0 / ( 119.0 + 123.0 );
         } else {
           return  24.0 / ( 596.0 + 24.0 );
         }
       }
     } else {
       if ( shapedVars[maxm2] <= 3231.931396484375 ) {
         if ( shapedVars[maxm2] <= 1831.751953125 ) {
           return  296.0 / ( 2279.0 + 296.0 );
         } else {
           return  427.0 / ( 292.0 + 427.0 );
         }
       } else {
         if ( shapedVars[maxm2] <= 3691.4248046875 ) {
           return  47.0 / ( 5883.0 + 47.0 );
         } else {
           return  12.0 / ( 1.0 + 12.0 );
         }
       }
     }
   } else {
     if ( shapedVars[evol] <= -147.67694091796875 ) {
       if ( shapedVars[maxm2] <= 1851.1026611328125 ) {
         if ( shapedVars[evol] <= -892.41943359375 ) {
           return  1127.0 / ( 324.0 + 1127.0 );
         } else {
           return  378.0 / ( 602.0 + 378.0 );
         }
       } else {
         if ( shapedVars[stdev] <= 827.0838623046875 ) {
           return  372.0 / ( 231.0 + 372.0 );
         } else {
           return  1418.0 / ( 38051.0 + 1418.0 );
         }
       }
     } else {
       if ( shapedVars[stdev] <= 726.1719970703125 ) {
         if ( shapedVars[maxm2] <= 2567.92919921875 ) {
           return  16.0 / ( 134.0 + 16.0 );
         } else {
           return  34.0 / ( 10.0 + 34.0 );
         }
       } else {
         if ( shapedVars[evol] <= 1590.152587890625 ) {
           return  784.0 / ( 41330.0 + 784.0 );
         } else {
           return  337.0 / ( 59102.0 + 337.0 );
         }
       }
     }
   }
 }
}




