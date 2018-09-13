#ifndef __LOFFLINEL0SELECTOR__
#define __LOFFLINEL0SELECTOR__ "LOfflineL0Selector ######## "

#include <string>
#include <functional>
#include "LEvRec0.hh"
#include "LCalibration.hh"
#include "LCaloCalibration.hh"
#include "detector_const.hh"

class LOfflineL0Selector {
public:
	LOfflineL0Selector();
	void LoadConfiguration(const std::string conf_str);
	inline bool Select(const LEvRec0 ev) const {return _function(ev);};
	inline std::string GetConfiguration(void) const { return conf;};
	void LoadCalibration(const LCalibration *calin);
	void SetThreshold(const double threshIn) {threshold=threshIn; return;};

private:
	void SetConfiguration(const std::string conf_inp);
	void SetTheFunction(void);
	bool CheckConfiguration(const std::string conf_inp);
	void CopySignal(const LEvRec0 ev);
	bool CalibrationCheck(void) const;
	const bool TriggerFired(const int i) const;
	const bool PlaneFired(const int i) const;
	const bool VetoFired(const int i) const;
	const bool LysoFired(const int i) const;

	std::string conf;
	std::function<bool(const LEvRec0)> _function;

	inline bool _default(const LEvRec0 ev) {return true;};
	//################## Starting point of the offline selections ####################
	bool Mask0(const LEvRec0 ev); // flag: OR(T)
	bool Mask1(const LEvRec0 ev); // flag: OR(T)&&(P1)
	bool Mask2(const LEvRec0 ev); // flag: OR(T)&&(P1 OR P2)
	bool Mask3(const LEvRec0 ev); // flag: (T3 OR T4)&&(P1 OR P2)
	bool Mask4(const LEvRec0 ev); // flag: OR(T)&&(P1 && P2)
	bool Mask5(const LEvRec0 ev); // flag: OR(T)&&(P1 && P2 && P3)
	bool Mask6(const LEvRec0 ev); // flag: OR(T)&&(P1 OR P2)&&(P15 OR P16)
	bool Mask7(const LEvRec0 ev); // flag: OR(T)&&(P1 OR P2)&& OR(LYSO)
	bool AntiMask3(const LEvRec0 ev); // not Mask3
	bool Planes34(const LEvRec0 ev);
	

	double trigger[NTRIGSCINT][2];
	double plane[NSCINTPLANES][2];
	double lyso[NLYSOCRYSTALS];
	double veto[NVETOSCINT][2];

	const LCaloCalibration *calibration;
	double trigger_pedestal[NTRIGSCINT][2];
	double plane_pedestal[NSCINTPLANES][2];
	double lyso_pedestal[NLYSOCRYSTALS];
	double veto_pedestal[NVETOSCINT][2];

	double trigger_sigma[NTRIGSCINT][2];
	double plane_sigma[NSCINTPLANES][2];
	double lyso_sigma[NLYSOCRYSTALS];
	double veto_sigma[NVETOSCINT][2];

	double threshold;
};




#endif