#include "LSteeringManager.hh"
#include <typeinfo>   // operator typeid
#include <vector>
#include <iostream>
#include "LParser.hh"

void LSteeringManager::Reset(void) {
  parameters.resize(0);
  return;
}

LSteeringManager::LSteeringManager() {
  Reset();
}

bool LSteeringManager::IsThereParameter(const char* parName) const {
  for(auto pit : parameters) {
    if(pit.name.compare(parName)==0) return true;
  }
  std::cout << __LSTEERINGMANAGER__ << "Error: no parameter named \""
	    << parName << "\" loaded." << std::endl;
  return false;
}


void LSteeringManager::Load(const std::string inputFile) {
  Load(inputFile.c_str());
  return;
}

void LSteeringManager::Load(const char* inputFile) {
  std::cout << __LSTEERINGMANAGER__ << "Reading input file " << inputFile << std::endl;
  
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


LSteeringManager::~LSteeringManager() {
  Reset();
}
