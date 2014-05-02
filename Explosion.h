//
// Explosion.h
//

#include "Object.h"

class Explosion : public Object {

 private:
  int time_to_live;
  void step();

 public:
  Explosion(Position pos);
  Explosion(std::string serialized);
  int eventHandler(Event *p_e);
};

