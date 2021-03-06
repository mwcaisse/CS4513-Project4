//
// Saucer.h
//

#include "Object.h"
#include "EventCollision.h"

class Saucer : public Object {

 private:
  void moveToStart();
  void out();
  void hit(EventCollision *p_c);

 public:
  Saucer();
  //creates a saucer from the serialized string
  Saucer(std::string serialized);
  ~Saucer();
  int eventHandler(Event *p_e);
};

