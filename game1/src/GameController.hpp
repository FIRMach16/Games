#include "GameModel.hpp"
#include "GameView.hpp"
class GameController {
  GameModel gm;
  GameView vm;

public:
  void run();
  GameController(GameModel &gm, GameView &vm);
};
