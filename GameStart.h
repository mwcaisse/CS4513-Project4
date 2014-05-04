//
// GameStart.h
//

#define MENU_LEVEL 1
#define PLAY_LEVEL 2

#include "ViewObject.h"

class GameStart : public ViewObject {

	/** The ready status of the game */
	enum ReadyStatus { NONE, ME, OTHER, BOTH };

 private:

	/** The game start's status */
	ReadyStatus status;

  void startGame();
  void start();

  void step();

 public:
  GameStart();
  int eventHandler(Event *e);
  void draw();
  void startOther();
};

