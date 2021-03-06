#ifndef LIMADOU_CONST
#define LIMADOU_CONST

const int VA_CHAN=64;
const int ADC_CHAN=3*VA_CHAN;  // 192
const int LADDER_CHAN=4*ADC_CHAN; // 768
const int COLUMN_CHAN=2*LADDER_CHAN; // 1536
const int SIDE_CHAN=LADDER_CHAN/2; 
const int NCHAN = 6*LADDER_CHAN; // 4608
const int SIDE_VA=6;
const int NADC = 4096;
const int NPMT = 64;
const int NPS = 12; // nr of power supply for High Voltages
const int NPSPMT = 10; // nr of power supply for PMT High Voltages
const int NCALOPLANE = 18; // nr of planes (+ trigger + lyso plane)
const int NRATEMETER = 9; // 
const int NTRIGSCINT=6;
const int NSCINTPLANES=16;
const int NVETOSCINT=5;
const int NLYSOCRYSTALS=9;
const int N_VA=NCHAN/VA_CHAN;

const int N_SIDES=2;
const int N_LADDER=6;
const int N_COLUMN=3;
const int LADDER_BIN=NCHAN/N_LADDER;
const double PITCH=182.e-6; // in meters
const double LADDERDIST=0.01; // distance in meter between the two ladders

const int EASIROC_CONF=60; // registers for easiroc configuration
const int SINGLE_PMT_RM=65; // single pmt rate meter


// Calo dimensions
const double SILICONTHICKNESS=0.3;

const double TRIGBARTHICKNESS=5.;
const double TRIGBARLENGTH=200.;
const double TRIGBARWIDTH=30.;

const double SCINTPLANESIDE=150.;
const double SCINTPLANETHICKNESS=10.;

const double VETOTHICKNESS=5.;
const double BVETOSIDE=150.;
const double XLVETOWIDTH=199.72;
const double YLVETOWIDTH=170.;
const double LVETOLENGTH=322.40;

const double LYSOTHICKNESS=40.;
const double LYSOSIDE=48.;


// z gaps
const double SILOUT_SILIN_GAP=10.;
//const double SILIN_TRIGGER_GAP=10.;  //see below
const double TRIG_SCINT_GAP=10.; // ???
const double SCINT_SCINT_GAP=2.; // ???
const double SCINT_LYSO_GAP=2; // ???
const double LYSO_BVETO_GAP=10.; // ???
const double TRIG_LVETO_ZGAP=10.; // ???

// x, y gaps
const double SCINT_XLVETO_GAP=10.; // ???
const double SCINT_YLVETO_GAP=10.; // ???
const double LYSO_LYSO_GAP=10.; // ???

const double LADDER_X_SEPARATION = 4.9e-3; // in meters from the ASSIEME design
const double LADDER_WIDTH = 77.6e-3; // in meters from the ASSIEME design
const double LADDER_LENGTH = 219.36e-3; // in meters from the ASSIEME design
const double LADDER_Z_SEPARATION = 10.e-3; // in meters from the ASSIEME design. Centre-to-centre of thicknesses

const double SILIN_TRIGGER_GAP=16.66e-3;  // in meters from the ASSIEME design. Side-to-faced-side, no thicknesses accounted for


enum pmt_calib_type {HIGH, LOW};

#endif
