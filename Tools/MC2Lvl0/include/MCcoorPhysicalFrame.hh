#ifndef MC2PHYSDET_HH
#define MC2PHYSDET_HH

#include "TVector3.h"


class MCtoPhysicalDetectorFrame {
   public:
      MCtoPhysicalDetectorFrame( TVector3 inputVector) : mcVector( inputVector) {};
      TVector3 GetPhysicalVector();
    private:
        TVector3 mcVector;
};

#endif /* MC2PHYSDET_HH */
