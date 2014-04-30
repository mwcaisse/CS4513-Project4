//
// GameStart.h
//

#define MENU_LEVEL 1
#define PLAY_LEVEL 2

#include "ViewObject.h"

class GameStart : public ViewObject {

 private:
  void step();
  void start();

 public:
  GameStart();
  int eventHandler(Event *e);
  void draw();
};

