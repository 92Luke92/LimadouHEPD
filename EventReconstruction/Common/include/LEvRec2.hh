#ifndef __LEVREC2__
#define __LEVREC2__ 1

#include <cstddef>

struct LEvRec2Stream;

class LEvRec2 {

public:
  LEvRec2();
  ~LEvRec2();

  void CopyFromLEvRec2Stream(const LEvRec2Stream evstr);


private:
  void Reset();

};

struct LEvRec2Stream {

  LEvRec2Stream();
  LEvRec2Stream(const LEvRec2 event);
  void Reset();
  void CopyFromLEvRec2(const LEvRec2 event);
  ~LEvRec2Stream();
};

#endif