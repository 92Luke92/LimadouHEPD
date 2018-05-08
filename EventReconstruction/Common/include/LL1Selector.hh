#ifndef __LL1SELECTOR__
#define __LL1SELECTOR__ "LL1Selector ############### "

#include <string>
#include <functional>
#include "LEvRec1.hh"
#include "detector_const.hh"

class LL1Selector {
public:
	LL1Selector();
	void LoadConfiguration(const std::string conf_str);
	inline bool Select(const LEvRec1 ev) const {return _function(ev);};
	inline std::string GetConfiguration(void) const { return conf;};
	void SetThreshold(const double threshIn) {threshold=threshIn; return;};

private:
	void SetConfiguration(const std::string conf_inp);
	void SetTheFunction(void);
	bool CheckConfiguration(const std::string conf_inp);
	void CopySignal(const LEvRec1 ev);
	const bool TriggerFired(const int i) const;
	const bool PlaneFired(const int i) const;
	const bool VetoFired(const int i) const;
	const bool LysoFired(const int i) const;
	const bool TriggerDFired(const int i) const;
	const bool PlaneDFired(const int i) const;
	const bool VetoDFired(const int i) const;
	
	std::string conf;

	double trigger[NTRIGSCINT][2];
	double plane[NSCINTPLANES][2];
	double lyso[NLYSOCRYSTALS];
	double veto[NVETOSCINT][2];

	double threshold;


	std::function<bool(const LEvRec1)> _function;

	inline bool _default(const LEvRec1 ev) {return true;};
	//################## Starting point of the offline selections ####################
	bool Mask0(const LEvRec1 ev); // flag: OR(T)
	bool Mask1(const LEvRec1 ev); // flag: OR(T)&&(P1)
	bool Mask2(const LEvRec1 ev); // flag: OR(T)&&(P1 OR P2)
	bool Mask3(const LEvRec1 ev); // flag: (T3 OR T4)&&(P1 OR P2)
	bool Mask4(const LEvRec1 ev); // flag: OR(T)&&(P1 && P2)
	bool Mask5(const LEvRec1 ev); // flag: OR(T)&&(P1 && P2 && P3)
	bool Mask6(const LEvRec1 ev); // flag: OR(T)&&(P1 OR P2)&&(P15 OR P16)
	bool Mask7(const LEvRec1 ev); // flag: OR(T)&&(P1 OR P2)&& OR(LYSO)
	bool LateralVeto(const LEvRec1 ev); // true if the event is vetoed
	bool BottomVeto(const LEvRec1 ev); // true if the event is vetoed
	bool Veto(const LEvRec1 ev); // true if the event is vetoed
	bool AntiMask3(const LEvRec1 ev); // not Mask3
	bool Planes34(const LEvRec1 ev);
	bool DMask3(const LEvRec1 ev); // flag: (DT3 OR DT4)&&(DP1 OR DP2)
	bool NoCorrT1T6(const LEvRec1 ev); // flag: (T1 && T6)
	bool NoCorrT1Any(const LEvRec1 ev); // flag: too complex... take your time to read the code...
	bool NoCorrT2Any(const LEvRec1 ev); // flag: too complex... take your time to read the code...
	bool NoCorrT3Any(const LEvRec1 ev); // flag: too complex... take your time to read the code...
	bool NoCorrT4Any(const LEvRec1 ev); // flag: too complex... take your time to read the code...
	bool NoCorrT5Any(const LEvRec1 ev); // flag: too complex... take your time to read the code...
	bool NoCorrT6Any(const LEvRec1 ev); // flag: too complex... take your time to read the code...
	bool NoCorrTAny(const LEvRec1 ev); // flag: too complex... take your time to read the code...
	bool NoCorrVL1Any(const LEvRec1 ev); // flag: VL1>HadThreshold
	bool NoCorrVL2Any(const LEvRec1 ev); // flag: VL2>HadThreshold
	bool NoCorrVL3Any(const LEvRec1 ev); // flag: VL3>HadThreshold
	bool NoCorrVL4Any(const LEvRec1 ev); // flag: VL4>HadThreshold
	bool NoCorrVBAny(const LEvRec1 ev); // flag: VB>HadThreshold
	bool NoCorrVAny(const LEvRec1 ev); // flag: or of the others
	bool NoPlane5PMT0(const LEvRec1 ev); 
	bool InsidePlane1Cone(const LEvRec1 ev); // it does not use any threshold
	bool NoT6(const LEvRec1 ev); 
};




#endif