#ifndef MC2PHYSDET_HH
#define MC2PHYSDET_HH

#include <TROOT.h>
#include <TString.h>
#include <TMath.h>
#include <TVector3.h>
#include <iostream>

TVector3 MCtoPhysicalDetectorFrame(TVector3 mcVector)
{
	/*
	MC version: HEPDSW_had_v2
	This function allows to convert a vector in the MC simulation into a vector in the physical detector reference frame
	*/

	TVector3 physVector;	// this vector is initialized to zero

	// position of the origin used in the simulation with respect to the physical detector reference frame
	Double_t x0_sim = 175.0;	// mm
	Double_t y0_sim = -152.0;	// mm
	Double_t zTrkLayer1_sim = 359.10;	// mm
	Double_t z0_sim = 376.62;	// mm  // this value corresponds to the distance from the outer thermal blanket layer (alluminum), which makes up the HEPD box, to the origin used in the simulation
	/*
	the previous value does not contain the chinese update!
	*/

	Double_t x_phys = x0_sim -mcVector.X();	// the x-axis in pointed in the opposite direction
	Double_t y_phys = y0_sim +mcVector.Y();	// the y-axis is pointed in the same direction
	Double_t z_phys = z0_sim -mcVector.Z();	// the z-axis is pointed in the opposite direction

	physVector.SetX(x_phys);
	physVector.SetY(y_phys);
	physVector.SetZ(z_phys);

	return physVector;
}

#endif /* MC2PHYSDET_HH */
