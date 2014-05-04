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
  void fire();
  void step();

 public:
  ClientHero();
  ClientHero(std::string serialized);
  ~ClientHero();

  // call move (or do nothing) according to key pressed
  int keyboard(int key);

  //event handler for step event
  int eventHandler(Event *p_e);
};

