//
// Bullet.h
//

#include "Object.h"
#include "EventCollision.h"

class Bullet : public Object {

 private:
  void out();
  void hit(EventCollision *p_c);

 public:
  //creates a bullet the specified pos with the specified sprite
  Bullet(Position hero_pos, std::string spriteName = "bullet");
  //Creates a new bullet from the serialized string
  Bullet(std::string serialized);
  int eventHandler(Event *p_e);
};

