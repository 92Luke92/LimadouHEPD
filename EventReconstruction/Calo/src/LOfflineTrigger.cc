#include "LOfflineTrigger.hh"
#include <iostream>

LOfflineTrigger::LOfflineTrigger(){
	conf = "default";
	SetTheFunction();
}

bool LOfflineTrigger::CheckConfiguration(const std::string conf_inp){
	bool check = false;
	// if conf_imp == one configuration in the list then true
	if(conf_inp.compare("default")==0) check = true;
	if(conf_inp.compare("(T2.OR.T3).AND.S0")==0) check = true;
	if(conf_inp.compare("(T2.OR.T3).AND.L4")==0) check = true;
	if(conf_inp.compare("(T2.OR.T3).AND.S15")==0) check = true;
	if(conf_inp.compare("(T2.OR.T3).AND.VB")==0) check = true;
	if(conf_inp.compare("central_muon_tight")==0) check = true;
	if(conf_inp.compare("central_muon")==0) check = true;
	if(conf_inp.compare("right_muon")==0) check = true;
	if(conf_inp.compare("right_muon_lyso5")==0) check = true;
	if(conf_inp.compare("left_muon")==0) check = true;
	if(conf_inp.compare("VL")==0) check = true;
	return check;
}

void LOfflineTrigger::SetConfiguration(const std::string conf_inp){
	if (CheckConfiguration(conf)==false) {
		std::cout <<__LOFFLINETRIGGER__	<< "Impossible to go ahead, wrong configuration flag:"
											<< conf_inp << std::endl;
		return;
	}
	conf = conf_inp;
	return;
}

void LOfflineTrigger::LoadConfiguration(const std::string conf_str){
	SetConfiguration(conf_str);
	SetTheFunction();
	return;
}

void LOfflineTrigger::SetTheFunction(void){
	if(conf.compare("default")==0) {	
		_function = std::bind(&LOfflineTrigger::_default, this, std::placeholders::_1);
	} else if(conf.compare("(T2.OR.T3).AND.S0")==0) {
		_function = std::bind(&LOfflineTrigger::I_T2_OR_T3_I_AND_S0, this, std::placeholders::_1);
	} else if(conf.compare("(T2.OR.T3).AND.L4")==0) {
		_function = std::bind(&LOfflineTrigger::I_T2_OR_T3_I_AND_L4, this, std::placeholders::_1);
	} else if(conf.compare("(T2.OR.T3).AND.S15")==0) {
		_function = std::bind(&LOfflineTrigger::I_T2_OR_T3_I_AND_S15, this, std::placeholders::_1);
	} else if(conf.compare("(T2.OR.T3).AND.VB")==0) {
		_function = std::bind(&LOfflineTrigger::I_T2_OR_T3_I_AND_VB, this, std::placeholders::_1);
	} else if(conf.compare("central_muon_tight")==0) {
		_function = std::bind(&LOfflineTrigger::central_muon_tight, this, std::placeholders::_1);
	} else if(conf.compare("central_muon")==0) {
		_function = std::bind(&LOfflineTrigger::central_muon, this, std::placeholders::_1);
	} else if(conf.compare("right_muon")==0) {
		_function = std::bind(&LOfflineTrigger::right_muon, this, std::placeholders::_1);
	} else if(conf.compare("right_muon_lyso5")==0) {
		_function = std::bind(&LOfflineTrigger::right_muon_lyso5, this, std::placeholders::_1);
	} else if(conf.compare("left_muon")==0) {
		_function = std::bind(&LOfflineTrigger::left_muon, this, std::placeholders::_1);
	} else if(conf.compare("VL")==0) {
		_function = std::bind(&LOfflineTrigger::VL, this, std::placeholders::_1);
	} else {
		_function = std::bind(&LOfflineTrigger::_default, this, std::placeholders::_1);
	}
	return;
}
//################## Starting point of the offline trigger functions ####################
bool LOfflineTrigger::I_T2_OR_T3_I_AND_S0(LEvRec1 ev) const{
	LTriggerSignal TRS = ev.trig;
	LScintillatorSignal SCS = ev.scint;
	double threshold = 5.;

	if(((TRS.sn_hg[2][0]+TRS.sn_hg[2][1])>threshold||
		(TRS.sn_hg[3][0]+TRS.sn_hg[3][1])>threshold)&& 
		 (SCS.sn_hg[0][0]+SCS.sn_hg[0][1]>threshold)) return true;
	else return false;
}
// ------------------ Reference case for muons -----------------------
bool LOfflineTrigger::I_T2_OR_T3_I_AND_L4(LEvRec1 ev) const{
	LTriggerSignal TRS = ev.trig;
	LLysoSignal LYS = ev.lyso;
	double threshold = 5.;

	if(((TRS.sn_hg[2][0]+TRS.sn_hg[2][1])>threshold||
		(TRS.sn_hg[3][0]+TRS.sn_hg[3][1])>threshold)&&
		(LYS.sn_hg[4][0]>threshold)) return true;
	else return false;
}

bool LOfflineTrigger::I_T2_OR_T3_I_AND_S15(LEvRec1 ev) const{
	LTriggerSignal TRS = ev.trig;
	LScintillatorSignal SCS = ev.scint;
	double threshold = 5.;

	if(((TRS.sn_hg[2][0]+TRS.sn_hg[2][1])>threshold||
		(TRS.sn_hg[3][0]+TRS.sn_hg[3][1])>threshold)&& 
		 (SCS.sn_hg[15][0]+SCS.sn_hg[15][1]>threshold)) return true;
	else return false;
}

bool LOfflineTrigger::I_T2_OR_T3_I_AND_VB(LEvRec1 ev) const{
	LTriggerSignal TRS = ev.trig;
	LVetoSignal VS = ev.veto;
	double threshold = 5.;

	if(((TRS.sn_hg[2][0]+TRS.sn_hg[2][1])>threshold||
		(TRS.sn_hg[3][0]+TRS.sn_hg[3][1])>threshold)&& 
		 (VS.sn_hg[4][0]+VS.sn_hg[4][1]>threshold)) return true;
	else return false;
}

bool LOfflineTrigger::central_muon_tight(LEvRec1 ev) const{
	LTriggerSignal TRS = ev.trig;
	LVetoSignal VS = ev.veto;
	LLysoSignal LS = ev.lyso;
	double threshold = 5.;

	if(  (((TRS.sn_hg[2][0]+TRS.sn_hg[2][1])>threshold &&
		 !( (TRS.sn_hg[0][0]+TRS.sn_hg[0][1])>threshold ||
		 	(TRS.sn_hg[1][0]+TRS.sn_hg[1][1])>threshold ||
			(TRS.sn_hg[3][0]+TRS.sn_hg[3][1])>threshold ||
			(TRS.sn_hg[4][0]+TRS.sn_hg[4][1])>threshold ||
			(TRS.sn_hg[5][0]+TRS.sn_hg[5][1])>threshold))
			||
         ((TRS.sn_hg[3][0]+TRS.sn_hg[3][1])>threshold &&
		 !( (TRS.sn_hg[0][0]+TRS.sn_hg[0][1])>threshold ||
		 	(TRS.sn_hg[1][0]+TRS.sn_hg[1][1])>threshold ||
			(TRS.sn_hg[2][0]+TRS.sn_hg[2][1])>threshold ||
			(TRS.sn_hg[4][0]+TRS.sn_hg[4][1])>threshold ||
			(TRS.sn_hg[5][0]+TRS.sn_hg[5][1])>threshold)))
		&&
		 (VS.sn_hg[4][0]+VS.sn_hg[4][1]>threshold) &&
		 ((LS.sn_hg[4][0]>threshold) &&
		 !( LS.sn_hg[0][0]>threshold ||
		 	LS.sn_hg[1][0]>threshold ||
		 	LS.sn_hg[2][0]>threshold ||
		 	LS.sn_hg[3][0]>threshold ||
		 	LS.sn_hg[5][0]>threshold ||
		 	LS.sn_hg[6][0]>threshold ||
		 	LS.sn_hg[7][0]>threshold ||
		 	LS.sn_hg[8][0]>threshold))) return true;
	else return false;
}

bool LOfflineTrigger::central_muon(LEvRec1 ev) const{
	LTriggerSignal TRS = ev.trig;
	LVetoSignal VS = ev.veto;
	LScintillatorSignal SCS = ev.scint;
	double threshold = 5.;

	if(  (((TRS.sn_hg[2][0]+TRS.sn_hg[2][1])>threshold || 
			(TRS.sn_hg[3][0]+TRS.sn_hg[3][1])>threshold) &&
		 !( (TRS.sn_hg[0][0]+TRS.sn_hg[0][1])>threshold ||
		 	(TRS.sn_hg[1][0]+TRS.sn_hg[1][1])>threshold ||
			(TRS.sn_hg[4][0]+TRS.sn_hg[4][1])>threshold ||
			(TRS.sn_hg[5][0]+TRS.sn_hg[5][1])>threshold))
		&&
			(SCS.sn_hg[0][0]+SCS.sn_hg[0][1]>threshold) &&
			(SCS.sn_hg[15][0]+SCS.sn_hg[15][1]>threshold) &&
			(VS.sn_hg[4][0]+VS.sn_hg[4][1]>threshold)

		 ) return true;
	else return false;
}

bool LOfflineTrigger::right_muon(LEvRec1 ev) const{
	LTriggerSignal TRS = ev.trig;
	LVetoSignal VS = ev.veto;
	LScintillatorSignal SCS = ev.scint;
	double threshold = 5.;

	if(  (((TRS.sn_hg[4][0]+TRS.sn_hg[4][1])>threshold || 
			(TRS.sn_hg[5][0]+TRS.sn_hg[5][1])>threshold) &&
		 !( (TRS.sn_hg[0][0]+TRS.sn_hg[0][1])>threshold ||
		 	(TRS.sn_hg[1][0]+TRS.sn_hg[1][1])>threshold ||
			(TRS.sn_hg[2][0]+TRS.sn_hg[2][1])>threshold ||
			(TRS.sn_hg[3][0]+TRS.sn_hg[3][1])>threshold))
		&&
			(SCS.sn_hg[0][0]+SCS.sn_hg[0][1]>threshold) &&
			(SCS.sn_hg[15][0]+SCS.sn_hg[15][1]>threshold) &&
			(VS.sn_hg[4][0]+VS.sn_hg[4][1]>threshold)

		 ) return true;
	else return false;
}

bool LOfflineTrigger::left_muon(LEvRec1 ev) const{
	LTriggerSignal TRS = ev.trig;
	LVetoSignal VS = ev.veto;
	LScintillatorSignal SCS = ev.scint;
	double threshold = 5.;

	if(  (((TRS.sn_hg[0][0]+TRS.sn_hg[0][1])>threshold || 
			(TRS.sn_hg[1][0]+TRS.sn_hg[1][1])>threshold) &&
		 !( (TRS.sn_hg[4][0]+TRS.sn_hg[4][1])>threshold ||
		 	(TRS.sn_hg[5][0]+TRS.sn_hg[5][1])>threshold ||
			(TRS.sn_hg[2][0]+TRS.sn_hg[2][1])>threshold ||
			(TRS.sn_hg[3][0]+TRS.sn_hg[3][1])>threshold))
		&&
			(SCS.sn_hg[0][0]+SCS.sn_hg[0][1]>threshold) &&
			(SCS.sn_hg[15][0]+SCS.sn_hg[15][1]>threshold) &&
			(VS.sn_hg[4][0]+VS.sn_hg[4][1]>threshold)

		 ) return true;
	else return false;
}

bool LOfflineTrigger::right_muon_lyso5(LEvRec1 ev) const{
	LTriggerSignal TRS = ev.trig;
	LVetoSignal VS = ev.veto;
	LLysoSignal LS = ev.lyso;
	double threshold = 5.;

	if(  (((TRS.sn_hg[4][0]+TRS.sn_hg[4][1])>threshold || 
			(TRS.sn_hg[5][0]+TRS.sn_hg[5][1])>threshold) &&
		 !( (TRS.sn_hg[0][0]+TRS.sn_hg[0][1])>threshold ||
		 	(TRS.sn_hg[1][0]+TRS.sn_hg[1][1])>threshold ||
			(TRS.sn_hg[2][0]+TRS.sn_hg[2][1])>threshold ||
			(TRS.sn_hg[3][0]+TRS.sn_hg[3][1])>threshold))
		&&
		 (VS.sn_hg[4][0]+VS.sn_hg[4][1]>threshold) &&
		 ((LS.sn_hg[7][0]>threshold) &&
		 !( LS.sn_hg[0][0]>threshold ||
		 	LS.sn_hg[1][0]>threshold ||
		 	LS.sn_hg[2][0]>threshold ||
		 	LS.sn_hg[3][0]>threshold ||
		 	LS.sn_hg[4][0]>threshold ||
		 	LS.sn_hg[5][0]>threshold ||
		 	LS.sn_hg[6][0]>threshold ||
		 	LS.sn_hg[8][0]>threshold))) return true;
	else return false;
}

bool LOfflineTrigger::VL(LEvRec1 ev) const{
	LVetoSignal VS = ev.veto;
	double threshold = 5.;

	if(VS.sn_hg[0][0]+VS.sn_hg[0][1]>threshold) return true;
	if(VS.sn_hg[1][0]+VS.sn_hg[1][1]>threshold) return true;
	if(VS.sn_hg[2][0]+VS.sn_hg[2][1]>threshold) return true;
	if(VS.sn_hg[3][0]+VS.sn_hg[3][1]>threshold) return true;
	return false;
}

