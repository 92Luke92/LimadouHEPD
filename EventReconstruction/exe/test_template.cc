

#include <string>

struct steerEntry {
  std::string type;
  std::string name;
  std::string value;
};

class LSteeringManager {
  
  LSteeringManager();
  ~LSteeringManager();
  void Load(const char* inputFile);
  template<typename RetType> RetType GetParameter(const char* parName) const;
  void Reset(void);
  
private:
  bool IsThereParameter(const char *parName) const;
  bool IsRetTypeCorrect(const char *parName) const;
  
  std::vector<steerEntry> parameters;
};
  

#include <typeinfo>   // operator typeid
#include <vector>
#include "LParser.h"

void LSteeringManager::Reset(void) {
  parameters.resize(0);
  return;
}

template<typename RetType> LSteeringManager::GetParameter(const char* parName) const {
  if(!IsThereParameter(parName)) return 0;
  if(!IsRetTypeCorrect(parName)) return 0;
  RetType result;
  for(auto pit : parameters) {
    if(pit.name.compare(parName)==0) result = static_cast<RetType>(pit.value);
  }
  return result;
}



bool LSteeringManager::IsThereParameter(const char* parName) const {
  for(auto pit : parameters) {
    if(pit.name.compare(parName)==0) return true;
  }
  std::cout << __LSTEEERINGMANAGER__ << "Error: no parameter named \""
	    << parName << "\" loaded." << std::endl;
  return false;
}


bool LSteeringManager::IsRetTypeCorrect(const char* parName) const {
  for(auto pit : parameters) {
    if(pit.name.compare(parName)==0) {
      if(pit.type.compare(typeid(RetType).name())==0) return true;
    }
  }
  std::cout << __LSTEEERINGMANAGER__ << "Error: parameter named \""
	    << parName << "\" not loaded or wrong type." << std::endl;
  return false;
}



void LSteeeringManager::Load(const char* inputFile) {
  std::cout << __LSTEERNMANAGER__ << "Reading input file " << inputFile << std::endl;
  
  LParser myparser;
  myparser.read_file(std::string(inputFile));
  myparser.remove_comments("#");
  myparser.remove_comments("//");
  myparser.remove_multi_blanks();
  myparser.remove_initial_blanks();
  myparser.remove_final_blanks();
  myparser.remove_blank_lines();
  myparser.tokenize(" ");
  //myparser.dump();

  std::vector< std::vector< std::string > > mylines_parsed=myparser.get_lines();
  
  // Interpret the file
  const unsigned char NOfTokensPerLine=3; // type name value
  detlist= new std::vector< std::pair<std::string, double> >();
  
  std::vector< std::vector< std::string > >::const_iterator iit;  
  for(iit=mylines_parsed.begin(); iit!=mylines_parsed.end(); iit++) {
    size_t NOfTokens=iit->size();    
    if(NOfTokens!=NOfTokensPerLine) {
      std::cerr << __LSTEERINGMANAGER__ << "Found " << iit->size()
		<< " tokens in a line of \"" << inputFile << "\"."
		<< " Impossible continuing." << std::endl;
      return;
    }
    
    steerEntry ent;
    ent.type = iit->at(0);
    ent.name = iit->at(1);
    ent.value = iit->at(2);
    parameters.push_back(ent);
  }  

  return;
}
