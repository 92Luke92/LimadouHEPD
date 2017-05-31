#include "LOfflineTrigger.hh"
#include <iostream>

LOfflineTrigger::LOfflineTrigger(){
	conf = "default";
	SetTheFunction();
}

bool LOfflineTrigger::CheckConfiguration(const std::string conf_inp){
	bool check = false;
	// if conf_imp == one configuration in the list then true
	if(conf_inp.compare("default")) check = true;
	if(conf_inp.compare("(T2.OR.T3).AND.S1")) check = true;
	return check;
}

void LOfflineTrigger::SetConfiguration(const std::string conf_inp){
	if (CheckConfiguration(conf)==true) {
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
	if(conf.compare("default")) {
		_function = std::bind(&LOfflineTrigger::_default, this, std::placeholders::_1);
	} else if(conf.compare("(T2.OR.T3).AND.S1")) {
		_function = std::bind(&LOfflineTrigger::I_T2_OR_T3_I_AND_S1, this, std::placeholders::_1);
	} else {
		_function = std::bind(&LOfflineTrigger::_default, this, std::placeholders::_1);
	}
	return;
}
//################## Starting point of the offline trigger functions ####################
bool LOfflineTrigger::I_T2_OR_T3_I_AND_S1(LEvRec1 ev) const{
	LTriggerSignal TRS = ev.trig;
	LScintillatorSignal SCS = ev.scint;
	double threshold = 5.;
	if(((TRS.sn_hg[2][0]+TRS.sn_hg[2][1])>threshold||
		(TRS.sn_hg[3][0]+TRS.sn_hg[3][1])>threshold)&& 
		 SCS.sn_hg[0][0]+SCS.sn_hg[0][1]>threshold) return true;
	else return false;

}


