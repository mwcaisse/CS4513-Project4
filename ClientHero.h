//
// Hero.h
//

#include "Object.h"
#include "EventKeyboard.h"

class ClientHero : public Object {

 private:
  int fire_slowdown;
  int fire_countdown;
  int nuke_count;
  void move(int dy);

 public:
  ClientHero();
  ~ClientHero();
};

