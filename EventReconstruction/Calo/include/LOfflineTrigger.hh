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
	bool I_T2_OR_T3_I_AND_S1(const LEvRec1 ev) const; // flag: (T2.OR.T3).AND.S1
};




#endif