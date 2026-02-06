#include "GameModel.hpp"
#include "GameView.hpp"
class GameController : public GameStateObserver {
  GameModel &gm;
  GameView &vm;

public:
  void run();
  void update() override;
  GameController(GameModel &gm, GameView &vm);
};
