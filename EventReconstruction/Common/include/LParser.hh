#ifndef __LPARSER__
#define __LPARSER__ 1

#include <string>
#include <vector>

class LParser
{
public:
  LParser(void);
  ~LParser(void);

  void read_file(const std::string inp);
  void remove_comments(const std::string comment);
  void remove_multi_blanks(void);
  void remove_initial_blanks(void);
  void remove_final_blanks(void);
  void remove_blank_lines(void);
  void tokenize(const std::string separator);
  void reset(void);
  void dump(void);
  std::vector< std::vector< std::string > > get_lines(void);
  
private:
  std::vector< std::string > mylines;
  int nlines;
  std::vector< std::vector< std::string > > mylines_tokenized;
};


#endif
