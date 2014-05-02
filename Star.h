//
// Star.h
//

#include "Object.h"

#define STAR_CHAR '.'

class Star : public Object {

 private:
  void out();

 public:
  Star();
  Star(std::string serialized);
  void draw(void);
  int eventHandler(Event *p_e);
};

