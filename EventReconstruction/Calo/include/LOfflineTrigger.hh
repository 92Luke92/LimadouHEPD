#ifndef __LOFFLINETRIGGER__
#define __LOFFLINETRIGGER__ "LOfflineTrigger  ########## "

#include <string>
#include <functional>
#include "LEvRec1.hh"

class LOfflineTrigger {
public:
	LOfflineTrigger();
	void LoadConfiguration(const std::string conf_str);
	inline bool Trigger(const LEvRec1 ev) const {return _function(ev);};
	inline std::string GetConfiguration(void) const { return conf;};

private:
	void SetConfiguration(const std::string conf_inp);
	void SetTheFunction(void);
	bool CheckConfiguration(const std::string conf_inp);

	std::string conf;
	std::function<bool(const LEvRec1)> _function;

	inline bool _default(const LEvRec1 ev) const {return true;};
	//################## Starting point of the offline trigger functions ####################
	bool I_T2_OR_T3_I_AND_S0(const LEvRec1 ev) const; // flag: (T2.OR.T3).AND.S0
	bool I_T2_OR_T3_I_AND_L4(const LEvRec1 ev) const; // flag: (T2.OR.T3).AND.L4
	bool I_T2_OR_T3_I_AND_S15(const LEvRec1 ev) const; // flag: (T2.OR.T3).AND.S15
	bool I_T2_OR_T3_I_AND_VB(const LEvRec1 ev) const; // flag: (T2.OR.T3).AND.VB
	bool central_muon_tight(const LEvRec1 ev) const; // flag: central_muon_tight
	bool central_muon(const LEvRec1 ev) const; // flag: central_muon
	bool right_muon(const LEvRec1 ev) const; // flag: right_muon
	bool left_muon(const LEvRec1 ev) const; // flag: left_muon
	bool right_muon_lyso0(const LEvRec1 ev) const;// flag:right_muon_lyso0
	bool left_muon_lyso2(const LEvRec1 ev) const;// flag:right_muon_lyso2
	bool central_muon_lyso4(const LEvRec1 ev) const;// flag:central_muon_lyso4
	bool VL(const LEvRec1 ev) const; // flag: VL
};




#endif