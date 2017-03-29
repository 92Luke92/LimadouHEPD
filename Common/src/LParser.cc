#include "LParser.hh"

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>

LParser::LParser(void)
{}

LParser::~LParser(void)
{}

void LParser::read_file(const std::string inputFile)
{
  std::ifstream inp(inputFile.c_str(),std::ifstream::in);
  
  while(1) {
    if(!inp.good()) break;
    std::string tmpstring;
    std::getline(inp,tmpstring);
    mylines.push_back(tmpstring);
  }
  nlines=mylines.size();
  
  return;
}

void LParser::remove_comments(const std::string comment)
{
  std::vector< std::string >::iterator it;
  size_t comm_pos;
  for(it=mylines.begin(); it!=mylines.end(); it++){
    while(1) {
      comm_pos=it->find_first_of(comment.c_str());
      if(comm_pos==std::string::npos) break;
      it->erase(it->begin()+comm_pos,it->end());
    }
  }

  return;
}

void LParser::remove_multi_blanks(void)
{
  std::vector< std::string >::iterator it;
  for(it=mylines.begin(); it!=mylines.end(); it++){
    size_t blankpos;
    size_t startsearch=0;
    while(1) {
      blankpos=it->find_first_of(" ", startsearch);
      if(blankpos==std::string::npos) break;
      unsigned char blankLength=0;
      while(it->compare(blankpos+blankLength,1," ")==0) blankLength++;
      if(blankLength>1) it->erase(blankpos+1,blankLength-1);
      startsearch=blankpos+1;
    }
  }

  return;
}

void LParser::remove_initial_blanks(void)
{
  std::vector< std::string >::iterator it;
  for(it=mylines.begin(); it!=mylines.end(); it++){
    size_t blankpos=it->find_first_of(" ", 0);
    if(blankpos==0) it->erase(blankpos,1);
  }

  return;
}

void LParser::remove_final_blanks(void)
{
  std::vector< std::string >::iterator it;
  for(it=mylines.begin(); it!=mylines.end(); it++){
    size_t blankpos=it->find_last_of(" ", std::string::npos);
    if(blankpos==it->size()-1 && it->size()!=0) it->erase(blankpos,1);
  }

  return;
}

void LParser::remove_blank_lines(void)
{
  std::vector< std::string >::const_iterator it;
  std::vector< std::string > mylines_cleaned;
  for(it=mylines.begin(); it!=mylines.end(); it++)
    if(it->compare("")!=0) mylines_cleaned.push_back(*it);

  mylines.resize(0);
  mylines.assign(mylines_cleaned.begin(),mylines_cleaned.end());
  
  return;
}

void LParser::tokenize(const std::string separator=" ")
{
  std::vector< std::string >::const_iterator it;
  for(it=mylines.begin(); it!=mylines.end(); it++){
    size_t startWord=0, stopWord;
    std::vector< std::string > tmpvec;
    std::string word;
    while(1) {
      startWord=it->find_first_not_of(separator.c_str(), startWord);
      if(startWord==std::string::npos) break;
      stopWord=it->find_first_of(separator.c_str(), startWord);
      if(stopWord==std::string::npos){
	stopWord=it->find_last_not_of(separator.c_str());
	if(stopWord==std::string::npos) {
	  std::cerr << "Error in parsing" << std::endl;
	}
	else stopWord++;
      }
      word=it->substr(startWord,stopWord-startWord);
      tmpvec.push_back(word);
      startWord=stopWord+1;
    }
    mylines_tokenized.push_back(tmpvec);
  }

return;
}

void LParser::reset(void)
{
  mylines.resize(0);
  nlines=0;
  mylines_tokenized.resize(0);

  return;
}

void LParser::dump(void)
{
  std::vector< std::string >::const_iterator it;
  for(it=mylines.begin(); it!=mylines.end(); ++it)
    std::cout << *it << std::endl;

  return;
}

std::vector< std::vector < std::string > > LParser::get_lines(void)
{
    return mylines_tokenized;
}

