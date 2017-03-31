#ifndef __LSTEERINGMANAGER__
#define __LSTEERINGMANAGER__ "LSteeringManager ########## "

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <typeinfo>

struct steerEntry {
  std::string type;
  std::string name;
  std::string value;
};

class LSteeringManager {
public:
  LSteeringManager();
  ~LSteeringManager();
  void Load(const char* inputFile);
  void Load(const std::string inputFile);
  template<typename RetType> RetType GetParameter(const char* parName) const;
  void Reset(void);
  
private:
  bool IsThereParameter(const char *parName) const;
  template <typename RetType> bool IsRetTypeCorrect(const char *parName) const;
  
  std::vector<steerEntry> parameters;
};

template<typename RetType> RetType LSteeringManager::GetParameter(const char* parName) const {
  if(!IsThereParameter(parName)) return 0;
  if(!IsRetTypeCorrect<RetType>(parName)) return 0;
  RetType result;
  for(auto pit : parameters) {
    if(pit.name.compare(parName)==0) {
      std::istringstream istr(pit.value);
      istr >> std::boolalpha >> result;  // boolalpha necessary to read true/false in case of boolean
    }
  }
  return result;
}

template <typename RetType> bool LSteeringManager::IsRetTypeCorrect(const char* parName) const {
  for(auto pit : parameters) {
    if(pit.name.compare(parName)==0) {
      if(pit.type.compare("std::string")==0 && typeid(RetType)==typeid(std::string)) return true;
      else if(pit.type.compare("int")==0 && typeid(RetType)==typeid(int)) return true;
      else if(pit.type.compare("float")==0 && typeid(RetType)==typeid(float)) return true;
      else if(pit.type.compare("double")==0 && typeid(RetType)==typeid(double)) return true;
      else if(pit.type.compare("bool")==0 && typeid(RetType)==typeid(bool)) return true;
    }
  }
  std::cout << __LSTEERINGMANAGER__ << "Error: parameter named \""
	    << parName << "\" not loaded or wrong type." << std::endl;
  return false;
}

#endif
