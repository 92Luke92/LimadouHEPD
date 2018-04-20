#include "LOfflineL0Selector.hh"
#include "LCaloCalibration.hh"

#include <iostream>

const double __DEFAULTTHRESHOLD=5.; //sigmas

LOfflineL0Selector::LOfflineL0Selector(){
	conf = "default";
	calibration =0;
	SetTheFunction();
	SetThreshold(__DEFAULTTHRESHOLD);
}

bool LOfflineL0Selector::CheckConfiguration(const std::string conf_inp){
	bool check = false;
	// if conf_imp == one configuration in the list then true
	if(conf_inp.compare("default")==0) check = true;
	if(conf_inp.compare("Mask0")==0) check = true;
	if(conf_inp.compare("Mask1")==0) check = true;
	if(conf_inp.compare("Mask2")==0) check = true;
	if(conf_inp.compare("Mask3")==0) check = true;
	if(conf_inp.compare("Mask4")==0) check = true;
	if(conf_inp.compare("Mask5")==0) check = true;
	if(conf_inp.compare("Mask6")==0) check = true;
	if(conf_inp.compare("Mask7")==0) check = true;
	if(conf_inp.compare("AntiMask3")==0) check = true;
	if(conf_inp.compare("Planes34")==0) check = true;
	if(check==false) {
		std::cout << __LOFFLINEL0SELECTOR__ << " LOfflineL0Selector::CheckConfiguration failed. configuration unexpected" << std::endl;
	}
	return check;
}

void LOfflineL0Selector::SetConfiguration(const std::string conf_inp){
	if (CheckConfiguration(conf)==false) {
		std::cout <<__LOFFLINEL0SELECTOR__	<< "Impossible to go ahead, wrong configuration flag:"
											<< conf_inp << std::endl;
		return;
	}
	conf = conf_inp;
	return;
}

void LOfflineL0Selector::LoadConfiguration(const std::string conf_str){
	SetConfiguration(conf_str);
	SetTheFunction();
	return;
}

bool LOfflineL0Selector::CalibrationCheck(void) const {
	if(calibration == 0) {
		std::cout << __LOFFLINEL0SELECTOR__ << " error! LOfflineTrigger::LoadCalibration did not load anything (null pointer)..." << std::endl;
		return false;
	} else return true;
}

void LOfflineL0Selector::LoadCalibration(const LCalibration *calin) {
	calibration=calin->GetCaloHGCalibration();
	LCaloCalibration *calibrationLG = calin->GetCaloLGCalibration();

	if(CalibrationCheck()==false) return;

	const double *pedestal = calibration->GetPedestal();
	const double *sigma = calibration->GetSigma();
	const double *pedestalLG = calibrationLG->GetPedestal();
	const double *sigmaLG = calibrationLG->GetSigma();
	
	// pedestals
	for(int i=0; i<NTRIGSCINT; ++i) {
		trigger_pedestal[i][0] = pedestal[i];
		trigger_pedestal[i][1] = pedestal[i+NPMT/2];
	}
	for(int i=0; i<NSCINTPLANES; ++i) {
		plane_pedestal[i][0] = pedestal[NTRIGSCINT+i];
		plane_pedestal[i][1] = pedestal[NTRIGSCINT+i+NPMT/2];
	}
	lyso_pedestal[0] = pedestalLG[28]; // North West
	lyso_pedestal[1] = pedestalLG[62]; // North
	lyso_pedestal[2] = pedestalLG[29]; // North East
	lyso_pedestal[3] = pedestalLG[30]; // West
	lyso_pedestal[4] = pedestalLG[31]; // Center
	lyso_pedestal[5] = pedestalLG[60]; // East
	lyso_pedestal[6] = pedestalLG[27]; // South West
	lyso_pedestal[7] = pedestalLG[61]; // South
	lyso_pedestal[8] = pedestalLG[59]; // South East

	for(int i=0; i<NVETOSCINT; ++i) {
		// First index: north, east, south, west, bottom
		// Second index: up, down (ne/sw for bottom)
		veto_pedestal[i][0] = pedestal[NTRIGSCINT+NSCINTPLANES+i];
		veto_pedestal[i][1] = pedestal[NTRIGSCINT+NSCINTPLANES+i+NPMT/2];
	}

	// sigmas
	for(int i=0; i<NTRIGSCINT; ++i) {
		trigger_sigma[i][0] = sigma[i];
		trigger_sigma[i][1] = sigma[i+NPMT/2];
	}
	for(int i=0; i<NSCINTPLANES; ++i) {
		plane_sigma[i][0] = sigma[NTRIGSCINT+i];
		plane_sigma[i][1] = sigma[NTRIGSCINT+i+NPMT/2];
	}
	lyso_sigma[0] = sigmaLG[28]; // North West
	lyso_sigma[1] = sigmaLG[62]; // North
	lyso_sigma[2] = sigmaLG[29]; // North East
	lyso_sigma[3] = sigmaLG[30]; // West
	lyso_sigma[4] = sigmaLG[31]; // Center
	lyso_sigma[5] = sigmaLG[60]; // East
	lyso_sigma[6] = sigmaLG[27]; // South West
	lyso_sigma[7] = sigmaLG[61]; // South
	lyso_sigma[8] = sigmaLG[59]; // South East


	for(int i=0; i<NVETOSCINT; ++i) {
		// First index: north, east, south, west, bottom
		// Second index: up, down (ne/sw for bottom)
		veto_sigma[i][0] = sigma[NTRIGSCINT+NSCINTPLANES+i];
		veto_sigma[i][1] = sigma[NTRIGSCINT+NSCINTPLANES+i+NPMT/2];
	}

	return;
};

void LOfflineL0Selector::SetTheFunction(void){
	if(conf.compare("default")==0) {	
		_function = std::bind(&LOfflineL0Selector::_default, this, std::placeholders::_1);
	} else if(conf.compare("Mask0")==0) {
		_function = std::bind(&LOfflineL0Selector::Mask0, this, std::placeholders::_1);
	} else if(conf.compare("Mask1")==0) {
		_function = std::bind(&LOfflineL0Selector::Mask1, this, std::placeholders::_1);
	} else if(conf.compare("Mask2")==0) {
		_function = std::bind(&LOfflineL0Selector::Mask2, this, std::placeholders::_1);
	} else if(conf.compare("Mask3")==0) {
		_function = std::bind(&LOfflineL0Selector::Mask3, this, std::placeholders::_1);
	} else if(conf.compare("Mask4")==0) {
		_function = std::bind(&LOfflineL0Selector::Mask4, this, std::placeholders::_1);
	} else if(conf.compare("Mask5")==0) {
		_function = std::bind(&LOfflineL0Selector::Mask5, this, std::placeholders::_1);
	} else if(conf.compare("Mask6")==0) {
		_function = std::bind(&LOfflineL0Selector::Mask6, this, std::placeholders::_1);
	} else if(conf.compare("Mask7")==0) {
		_function = std::bind(&LOfflineL0Selector::Mask7, this, std::placeholders::_1);
	} else if(conf.compare("AntiMask3")==0) {
		_function = std::bind(&LOfflineL0Selector::AntiMask3, this, std::placeholders::_1);
	} else if(conf.compare("Planes34")==0) {
		_function = std::bind(&LOfflineL0Selector::Planes34, this, std::placeholders::_1);
	} else {
		_function = std::bind(&LOfflineL0Selector::_default, this, std::placeholders::_1);
	}
	return;
}

void LOfflineL0Selector::CopySignal(const LEvRec0 ev) {
	for(int i=0; i<NTRIGSCINT; ++i) {
		trigger[i][0] = static_cast<double>(ev.trigger(i,0));
		trigger[i][1] = static_cast<double>(ev.trigger(i,1));
	}
	for(int i=0; i<NSCINTPLANES; ++i) {
		plane[i][0] = static_cast<double>(ev.plane(i,0));
		plane[i][1] = static_cast<double>(ev.plane(i,1));
	}
	for(int i=0; i<NLYSOCRYSTALS; ++i) lyso[i] = static_cast<double>(ev.lyso(i));
	
	for(int i=0; i<NVETOSCINT; ++i) {
		// First index: north, east, south, west, bottom
		// Second index: up, down (ne/sw for bottom)
		veto[i][0] = static_cast<double>(ev.veto(i,0));
		veto[i][1] = static_cast<double>(ev.veto(i,1));
	}
	return;
}

const bool LOfflineL0Selector::TriggerFired(const int i) const {
	if((trigger[i][0] >= trigger_pedestal[i][0]+threshold*trigger_sigma[i][0]) ||
		(trigger[i][1] >= trigger_pedestal[i][1]+threshold*trigger_sigma[i][1])) return true;
	else return false;
}

const bool LOfflineL0Selector::PlaneFired(const int i) const {
	if((plane[i][0] >= plane_pedestal[i][0]+threshold*plane_sigma[i][0]) ||
		(plane[i][1] >= plane_pedestal[i][1]+threshold*plane_sigma[i][1])) return true;
	else return false;
}

const bool LOfflineL0Selector::LysoFired(const int i) const {
	if(lyso[i] >= lyso_pedestal[i]+threshold*lyso_sigma[i]) return true;
	else return false;
}

const bool LOfflineL0Selector::VetoFired(const int i) const {
	if((veto[i][0] >= veto_pedestal[i][0]+threshold*veto_sigma[i][0]) ||
		(veto[i][1] >= veto_pedestal[i][1]+threshold*veto_sigma[i][1])) return true;
	else return false;
}

//################## Starting point of the offline selections ####################
bool LOfflineL0Selector::Mask0(const LEvRec0 ev)  {
	if(CalibrationCheck()==false) return false;
	CopySignal(ev);
	for(int i=0; i<NTRIGSCINT; ++i){
		if(TriggerFired(i)) return true;
	}
	return false;
}

bool LOfflineL0Selector::Mask1(const LEvRec0 ev)  {
	if(CalibrationCheck()==false) return false;
	CopySignal(ev);
	bool PlaneFLAG=false;
	if(PlaneFired(0)) PlaneFLAG=true;
	bool result = Mask0(ev) && PlaneFLAG;
	return result;
}

bool LOfflineL0Selector::Mask2(const LEvRec0 ev)  {
	if(CalibrationCheck()==false) return false;
	CopySignal(ev);
	bool PlaneFLAG=false;
	if(PlaneFired(0)||PlaneFired(1)) PlaneFLAG=true;
	bool result = Mask0(ev) && PlaneFLAG;
	return result;
}

bool LOfflineL0Selector::Mask3(const LEvRec0 ev)  {
	if(CalibrationCheck()==false) return false;
	CopySignal(ev);
	bool TriggerFLAG=false;
	if(TriggerFired(2)||TriggerFired(3)) TriggerFLAG=true;
	bool PlaneFLAG=false;
	if(PlaneFired(0)||PlaneFired(1)) PlaneFLAG=true;
	bool result = TriggerFLAG && PlaneFLAG;
	return result;
}

bool LOfflineL0Selector::Mask4(const LEvRec0 ev)  {
	if(CalibrationCheck()==false) return false;
	CopySignal(ev);
	bool PlaneFLAG=false;
	if(PlaneFired(0)&&PlaneFired(1)) PlaneFLAG=true;
	bool result = Mask0(ev) && PlaneFLAG;
	return result;
}

bool LOfflineL0Selector::Mask5(const LEvRec0 ev)  {
	if(CalibrationCheck()==false) return false;
	CopySignal(ev);
	bool result = Mask4(ev)&&PlaneFired(2);
	return result;
}

bool LOfflineL0Selector::Mask6(const LEvRec0 ev)  {
	if(CalibrationCheck()==false) return false;
	CopySignal(ev);
	bool result = Mask2(ev)&&(PlaneFired(14)||PlaneFired(15));
	return result;
}

bool LOfflineL0Selector::Mask7(const LEvRec0 ev)  {
	if(CalibrationCheck()==false) return false;
	CopySignal(ev);
	bool LysoFLAG=false;
	for(int i=0; i<NLYSOCRYSTALS; ++i) if(LysoFired(i)) LysoFLAG=true;
	bool result = Mask2(ev)&&LysoFLAG;
	return result;
}

bool LOfflineL0Selector::AntiMask3(const LEvRec0 ev)  {
	return !Mask3(ev);
}

bool LOfflineL0Selector::Planes34(const LEvRec0 ev) {
	if(CalibrationCheck()==false) return false;
	CopySignal(ev);
	bool PlaneFLAG=false;
	if(PlaneFired(2)||PlaneFired(3)) PlaneFLAG=true;
	return PlaneFLAG;
}

