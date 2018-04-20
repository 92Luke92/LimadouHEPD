#include "LL1Selector.hh"
#include <math.h>
#include <iostream>

const double __DEFAULTTHRESHOLD=5.; //sigmas

LL1Selector::LL1Selector(){
	conf = "default";
	SetTheFunction();
	SetThreshold(__DEFAULTTHRESHOLD);
}

bool LL1Selector::CheckConfiguration(const std::string conf_inp){
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
	if(conf_inp.compare("DMask3")==0) check = true;
	if(conf_inp.compare("NoCorrT1T6")==0) check = true;
	if(conf_inp.compare("NoCorrT1Any")==0) check = true;
	if(conf_inp.compare("NoCorrT2Any")==0) check = true;
	if(conf_inp.compare("NoCorrT3Any")==0) check = true;
	if(conf_inp.compare("NoCorrT4Any")==0) check = true;
	if(conf_inp.compare("NoCorrT5Any")==0) check = true;
	if(conf_inp.compare("NoCorrT6Any")==0) check = true;
	if(conf_inp.compare("NoCorrTAny")==0) check = true;
	if(conf_inp.compare("NoCorrVL1Any")==0) check = true;
	if(conf_inp.compare("NoCorrVL2Any")==0) check = true;
	if(conf_inp.compare("NoCorrVL3Any")==0) check = true;
	if(conf_inp.compare("NoCorrVL4Any")==0) check = true;
	if(conf_inp.compare("NoCorrVBAny")==0) check = true;
	if(conf_inp.compare("NoCorrVAny")==0) check = true;
	if(conf_inp.compare("NoPlane5PMT0")==0) check = true;
	if(conf_inp.compare("InsidePlane1Cone")==0) check = true;
	if(conf_inp.compare("NoT6")==0) check = true;
	if(check==false) {
		std::cout << __LL1SELECTOR__ << " LL1Selector::CheckConfiguration failed. configuration unexpected" << std::endl;
	}
	return check;
}

void LL1Selector::SetConfiguration(const std::string conf_inp){
	if (CheckConfiguration(conf)==false) {
		std::cout <<__LL1SELECTOR__	<< "Impossible to go ahead, wrong configuration flag:"
											<< conf_inp << std::endl;
		return;
	}
	conf = conf_inp;
	return;
}

void LL1Selector::LoadConfiguration(const std::string conf_str){
	SetConfiguration(conf_str);
	SetTheFunction();
	return;
}

void LL1Selector::SetTheFunction(void){
	if(conf.compare("default")==0) {	
		_function = std::bind(&LL1Selector::_default, this, std::placeholders::_1);
	} else if(conf.compare("Mask0")==0) {
		_function = std::bind(&LL1Selector::Mask0, this, std::placeholders::_1);
	} else if(conf.compare("Mask1")==0) {
		_function = std::bind(&LL1Selector::Mask1, this, std::placeholders::_1);
	} else if(conf.compare("Mask2")==0) {
		_function = std::bind(&LL1Selector::Mask2, this, std::placeholders::_1);
	} else if(conf.compare("Mask3")==0) {
		_function = std::bind(&LL1Selector::Mask3, this, std::placeholders::_1);
	} else if(conf.compare("Mask4")==0) {
		_function = std::bind(&LL1Selector::Mask4, this, std::placeholders::_1);
	} else if(conf.compare("Mask5")==0) {
		_function = std::bind(&LL1Selector::Mask5, this, std::placeholders::_1);
	} else if(conf.compare("Mask6")==0) {
		_function = std::bind(&LL1Selector::Mask6, this, std::placeholders::_1);
	} else if(conf.compare("Mask7")==0) {
		_function = std::bind(&LL1Selector::Mask7, this, std::placeholders::_1);
	} else if(conf.compare("AntiMask3")==0) {
		_function = std::bind(&LL1Selector::AntiMask3, this, std::placeholders::_1);
	} else if(conf.compare("Planes34")==0) {
		_function = std::bind(&LL1Selector::Planes34, this, std::placeholders::_1);
	} else if(conf.compare("DMask3")==0) {
		_function = std::bind(&LL1Selector::DMask3, this, std::placeholders::_1);
	} else if(conf.compare("NoCorrT1T6")==0) {
		_function = std::bind(&LL1Selector::NoCorrT1T6, this, std::placeholders::_1);
	} else if(conf.compare("NoCorrT1Any")==0) {
		_function = std::bind(&LL1Selector::NoCorrT1Any, this, std::placeholders::_1);
	} else if(conf.compare("NoCorrT2Any")==0) {
		_function = std::bind(&LL1Selector::NoCorrT2Any, this, std::placeholders::_1);
	} else if(conf.compare("NoCorrT3Any")==0) {
		_function = std::bind(&LL1Selector::NoCorrT3Any, this, std::placeholders::_1);
	} else if(conf.compare("NoCorrT4Any")==0) {
		_function = std::bind(&LL1Selector::NoCorrT4Any, this, std::placeholders::_1);
	} else if(conf.compare("NoCorrT5Any")==0) {
		_function = std::bind(&LL1Selector::NoCorrT5Any, this, std::placeholders::_1);
	} else if(conf.compare("NoCorrT6Any")==0) {
		_function = std::bind(&LL1Selector::NoCorrT6Any, this, std::placeholders::_1);
	} else if(conf.compare("NoCorrTAny")==0) {
		_function = std::bind(&LL1Selector::NoCorrTAny, this, std::placeholders::_1);
	} else if(conf.compare("NoCorrVL1Any")==0) {
		_function = std::bind(&LL1Selector::NoCorrVL1Any, this, std::placeholders::_1);
	} else if(conf.compare("NoCorrVL2Any")==0) {
		_function = std::bind(&LL1Selector::NoCorrVL2Any, this, std::placeholders::_1);
	} else if(conf.compare("NoCorrVL3Any")==0) {
		_function = std::bind(&LL1Selector::NoCorrVL3Any, this, std::placeholders::_1);
	} else if(conf.compare("NoCorrVL4Any")==0) {
		_function = std::bind(&LL1Selector::NoCorrVL4Any, this, std::placeholders::_1);
	} else if(conf.compare("NoCorrVBAny")==0) {
		_function = std::bind(&LL1Selector::NoCorrVBAny, this, std::placeholders::_1);
	} else if(conf.compare("NoCorrVAny")==0) {
		_function = std::bind(&LL1Selector::NoCorrVAny, this, std::placeholders::_1);
	} else if(conf.compare("NoPlane5PMT0")==0) {
		_function = std::bind(&LL1Selector::NoPlane5PMT0, this, std::placeholders::_1);
	} else if(conf.compare("InsidePlane1Cone")==0) {
		_function = std::bind(&LL1Selector::InsidePlane1Cone, this, std::placeholders::_1);
	} else if(conf.compare("NoT6")==0) {
		_function = std::bind(&LL1Selector::NoT6, this, std::placeholders::_1);
	} else {
		_function = std::bind(&LL1Selector::_default, this, std::placeholders::_1);
	}
	return;
}

void LL1Selector::CopySignal(const LEvRec1 ev) {
	for(int i=0; i<NTRIGSCINT; ++i) {
		trigger[i][0] = ev.trig.sn_hg[i][0];
		trigger[i][1] = ev.trig.sn_hg[i][1];
	}
	for(int i=0; i<NSCINTPLANES; ++i) {
		plane[i][0] = ev.scint.sn_hg[i][0];
		plane[i][1] = ev.scint.sn_hg[i][1]; 
	}
	for(int i=0; i<NLYSOCRYSTALS; ++i) lyso[i] = ev.lyso.sn_lg[i][0];
	
	for(int i=0; i<NVETOSCINT; ++i) {
		// First index: north, east, south, west, bottom
		// Second index: up, down (ne/sw for bottom)
		veto[i][0] = ev.veto.sn_hg[i][0];
		veto[i][1] = ev.veto.sn_hg[i][1];
	}
	return;
}

const bool LL1Selector::TriggerFired(const int i) const {
	if((trigger[i][0] >= threshold) ||
		(trigger[i][1] >= threshold)) return true;
	else return false;
}

const bool LL1Selector::PlaneFired(const int i) const {
	if((plane[i][0] >= threshold) ||
		(plane[i][1] >= threshold)) return true;
	else return false;
}

const bool LL1Selector::LysoFired(const int i) const {
	if(lyso[i] >= threshold) return true;
	else return false;
}

const bool LL1Selector::VetoFired(const int i) const {
	if((veto[i][0] >= threshold) ||
		(veto[i][1] >= threshold)) return true;
	else return false;
}

const bool LL1Selector::TriggerDFired(const int i) const {
	if((trigger[i][0] >= threshold) &&
		(trigger[i][1] >= threshold)) return true;
	else return false;
}

const bool LL1Selector::PlaneDFired(const int i) const {
	if(i==4) return PlaneFired(i); // !!!!!!!! One PMTs of 5th plane is broken!
	if((plane[i][0] >= threshold) &&
		(plane[i][1] >= threshold)) return true;
	else return false;
}

const bool LL1Selector::VetoDFired(const int i) const {
	if((veto[i][0] >= threshold) &&
		(veto[i][1] >= threshold)) return true;
	else return false;
}

//################## Starting point of the offline selections ####################
bool LL1Selector::Mask0(const LEvRec1 ev)  {
	CopySignal(ev);
	for(int i=0; i<NTRIGSCINT; ++i){
		if(TriggerFired(i)) return true;
	}
	return false;
}

bool LL1Selector::Mask1(const LEvRec1 ev)  {
	CopySignal(ev);
	bool PlaneFLAG=false;
	if(PlaneFired(0)) PlaneFLAG=true;
	bool result = Mask0(ev) && PlaneFLAG;
	return result;
}

bool LL1Selector::Mask2(const LEvRec1 ev)  {
	CopySignal(ev);
	bool PlaneFLAG=false;
	if(PlaneFired(0)||PlaneFired(1)) PlaneFLAG=true;
	bool result = Mask0(ev) && PlaneFLAG;
	return result;
}

bool LL1Selector::Mask3(const LEvRec1 ev)  {
	CopySignal(ev);
	bool TriggerFLAG=false;
	if(TriggerFired(2)||TriggerFired(3)) TriggerFLAG=true;
	bool PlaneFLAG=false;
	if(PlaneFired(0)||PlaneFired(1)) PlaneFLAG=true;
	bool result = TriggerFLAG && PlaneFLAG;
	return result;
}

bool LL1Selector::DMask3(const LEvRec1 ev)  {
	CopySignal(ev);
	bool TriggerFLAG=false;
	if(TriggerDFired(2)||TriggerDFired(3)) TriggerFLAG=true;
	bool PlaneFLAG=false;
	if(PlaneDFired(0)||PlaneDFired(1)) PlaneFLAG=true;
	bool result = TriggerFLAG && PlaneFLAG;
	return result;
}

bool LL1Selector::Mask4(const LEvRec1 ev)  {
	CopySignal(ev);
	bool PlaneFLAG=false;
	if(PlaneFired(0)&&PlaneFired(1)) PlaneFLAG=true;
	bool result = Mask0(ev) && PlaneFLAG;
	return result;
}

bool LL1Selector::Mask5(const LEvRec1 ev)  {
	CopySignal(ev);
	bool result = Mask4(ev)&&PlaneFired(2);
	return result;
}

bool LL1Selector::Mask6(const LEvRec1 ev)  {
	CopySignal(ev);
	bool result = Mask2(ev)&&(PlaneFired(14)||PlaneFired(15));
	return result;
}

bool LL1Selector::Mask7(const LEvRec1 ev)  {
	CopySignal(ev);
	bool LysoFLAG=false;
	for(int i=0; i<NLYSOCRYSTALS; ++i) if(LysoFired(i)) LysoFLAG=true;
	bool result = Mask2(ev)&&LysoFLAG;
	return result;
}

bool LL1Selector::AntiMask3(const LEvRec1 ev)  {
	return !Mask3(ev);
}

bool LL1Selector::Planes34(const LEvRec1 ev) {
	CopySignal(ev);
	bool PlaneFLAG=false;
	if(PlaneFired(2)||PlaneFired(3)) PlaneFLAG=true;
	return PlaneFLAG;
}

bool LL1Selector::NoCorrT1T6(const LEvRec1 ev) {
	CopySignal(ev);
	bool FLAG=false;
	if(!(TriggerFired(0)&&TriggerFired(5))) FLAG=true;
	return FLAG;
}

bool LL1Selector::NoCorrT1Any(const LEvRec1 ev) { // The Trigger bar must not correlate with any other non-adjacent trigger bar (adjacent admitted)
	CopySignal(ev);
	if((TriggerFired(0)&&TriggerFired(2))) return false;
	if((TriggerFired(0)&&TriggerFired(3))) return false;
	if((TriggerFired(0)&&TriggerFired(4))) return false;
	if((TriggerFired(0)&&TriggerFired(5))) return false;
	return true; // by default it assumes no correlation is there
}

bool LL1Selector::NoCorrT2Any(const LEvRec1 ev) { // The Trigger bar must not correlate with any other non-adjacent trigger bar (adjacent admitted)
	CopySignal(ev);
	if((TriggerFired(1)&&TriggerFired(3))) return false;
	if((TriggerFired(1)&&TriggerFired(4))) return false;
	if((TriggerFired(1)&&TriggerFired(5))) return false;
	return true; // by default it assumes no correlation is there
}

bool LL1Selector::NoCorrT3Any(const LEvRec1 ev) { // The Trigger bar must not correlate with any other non-adjacent trigger bar (adjacent admitted)
	CopySignal(ev);
	if((TriggerFired(2)&&TriggerFired(0))) return false;
	if((TriggerFired(2)&&TriggerFired(4))) return false;
	if((TriggerFired(2)&&TriggerFired(5))) return false;
	return true; // by default it assumes no correlation is there
}

bool LL1Selector::NoCorrT4Any(const LEvRec1 ev) { // The Trigger bar must not correlate with any other non-adjacent trigger bar (adjacent admitted)
	CopySignal(ev);
	if((TriggerFired(3)&&TriggerFired(0))) return false;
	if((TriggerFired(3)&&TriggerFired(1))) return false;
	if((TriggerFired(3)&&TriggerFired(5))) return false;
	return true; // by default it assumes no correlation is there
}

bool LL1Selector::NoCorrT5Any(const LEvRec1 ev) { // The Trigger bar must not correlate with any other non-adjacent trigger bar (adjacent admitted)
	CopySignal(ev);
	if((TriggerFired(4)&&TriggerFired(0))) return false;
	if((TriggerFired(4)&&TriggerFired(1))) return false;
	if((TriggerFired(4)&&TriggerFired(2))) return false;
	return true; // by default it assumes no correlation is there
}

bool LL1Selector::NoCorrT6Any(const LEvRec1 ev) { // The Trigger bar must not correlate with any other non-adjacent trigger bar (adjacent admitted)
	CopySignal(ev);
	if((TriggerFired(5)&&TriggerFired(0))) return false;
	if((TriggerFired(5)&&TriggerFired(1))) return false;
	if((TriggerFired(5)&&TriggerFired(2))) return false;
	if((TriggerFired(5)&&TriggerFired(3))) return false;
	return true; // by default it assumes no correlation is there
}

bool LL1Selector::NoCorrTAny(const LEvRec1 ev) { // The Trigger bars must not correlate with any non-adjacent trigger bar (adjacent admitted)
	CopySignal(ev);
	if(!NoCorrT1Any(ev)) return false;
	if(!NoCorrT2Any(ev)) return false;
	if(!NoCorrT3Any(ev)) return false;
	if(!NoCorrT4Any(ev)) return false;
	if(!NoCorrT5Any(ev)) return false;
	if(!NoCorrT6Any(ev)) return false;
	return true; // by default it assumes no correlation is there
}

bool LL1Selector::NoCorrVL1Any(const LEvRec1 ev) { // Hard having on two veto planes more signal than that expected for an electron (BTF electron @30MeV: (400 pm 90) ADC)
	CopySignal(ev);
	if(VetoFired(0)&&VetoFired(1)) return false;
	if(VetoFired(0)&&VetoFired(2)) return false;
	if(VetoFired(0)&&VetoFired(3)) return false;
	if(VetoFired(0)&&VetoFired(4)) return false;
	return true; // by default it assumes no correlation is there
}

bool LL1Selector::NoCorrVL2Any(const LEvRec1 ev) { // Hard having on two veto planes more signal than that expected for an electron (BTF electron @30MeV: (400 pm 90) ADC)
	CopySignal(ev);
	if(VetoFired(1)&&VetoFired(0)) return false;
	if(VetoFired(1)&&VetoFired(2)) return false;
	if(VetoFired(1)&&VetoFired(3)) return false;
	if(VetoFired(1)&&VetoFired(4)) return false;
	return true; // by default it assumes no correlation is there
}

bool LL1Selector::NoCorrVL3Any(const LEvRec1 ev) { // Hard having on two veto planes more signal than that expected for an electron (BTF electron @30MeV: (400 pm 90) ADC)
	CopySignal(ev);
	if(VetoFired(2)&&VetoFired(0)) return false;
	if(VetoFired(2)&&VetoFired(1)) return false;
	if(VetoFired(2)&&VetoFired(3)) return false;
	if(VetoFired(2)&&VetoFired(4)) return false;
	return true; // by default it assumes no correlation is there
}

bool LL1Selector::NoCorrVL4Any(const LEvRec1 ev) { // Hard having on two veto planes more signal than that expected for an electron (BTF electron @30MeV: (400 pm 90) ADC)
	CopySignal(ev);
	if(VetoFired(3)&&VetoFired(0)) return false;
	if(VetoFired(3)&&VetoFired(1)) return false;
	if(VetoFired(3)&&VetoFired(2)) return false;
	if(VetoFired(3)&&VetoFired(4)) return false;
	return true; // by default it assumes no correlation is there
}

bool LL1Selector::NoCorrVBAny(const LEvRec1 ev) { // Hard having on two veto planes more signal than that expected for an electron (BTF electron @30MeV: (400 pm 90) ADC)
	CopySignal(ev);
	if(VetoFired(4)&&VetoFired(0)) return false;
	if(VetoFired(4)&&VetoFired(1)) return false;
	if(VetoFired(4)&&VetoFired(2)) return false;
	if(VetoFired(4)&&VetoFired(3)) return false;
	return true; // by default it assumes no correlation is there
}

bool LL1Selector::NoCorrVAny(const LEvRec1 ev) { // Hard having on two veto planes more signal than that expected for an electron (BTF electron @30MeV: (400 pm 90) ADC)
	CopySignal(ev);
	if(!NoCorrVL1Any(ev)) return false;
	if(!NoCorrVL2Any(ev)) return false;
	if(!NoCorrVL3Any(ev)) return false;
	if(!NoCorrVL4Any(ev)) return false;
	if(!NoCorrVBAny(ev)) return false;
	return true; // by default it assumes no correlation is there
}

bool LL1Selector::NoPlane5PMT0(const LEvRec1 ev) { 
	CopySignal(ev);
	if(veto[4][0]>threshold) return false;
	else return true;
}

bool LL1Selector::NoT6(const LEvRec1 ev) { 
	CopySignal(ev);
	if(TriggerFired(5)) return false;
	else return true;
}

bool LL1Selector::InsidePlane1Cone(const LEvRec1 ev) { // it does not use any threshold
	double c0 = ev.scint.cont_hg[0][0];
	double c1 = ev.scint.cont_hg[0][1];

	// Distance from the c0=c1 line
	double dist = fabs(c0-c1)/sqrt(2.);
	// Distance from the origin of the c0=c1 line point closest to P(c0,c1)
	double L = sqrt(2.)*fabs(c0+c1);
	// Parameters from test beam electrons
	const double alpha = 135.;
	const double beta = 0.213;

	return dist <= alpha + beta*L ? true : false;
}

