#include "GameController.hpp"

int main() {
  GameModel gm = GameModel();
  GameView vm;
  GameController gc = GameController(gm, vm);
  gc.run();
}
