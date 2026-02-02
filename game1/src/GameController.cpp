// will handle user inputs and will be entry point
#include "GameController.hpp"
#include <iostream>
void handleMouseClick(GameView &vm, GameModel &gm) {
  if (auto *currentView = vm.getCurrentView()) {
    if (auto *menuView = dynamic_cast<MenuView *>(vm.getCurrentView())) {
      int idx = 0;
      for (auto bounds : menuView->hoverableItems) {
        if (checkIfHovered(menuView->mousePosition, bounds)) {
          std::array<char, 9> cells;
          cells.fill(emptyCellMark);
          if (idx != 2) {
            vm.setView(std::make_unique<TTTView>(cells));
          } else {
            vm.setView(std::make_unique<DifficultyView>());
          }
          gm.setGameMode(MENU_MODES[idx]);
          gm.resetScore();
        }
        idx++;
      }
    } else if (auto *tttView = dynamic_cast<TTTView *>(vm.getCurrentView())) {
      auto hoverableItems = tttView->hoverableItems;
      for (int i = 0; i < hoverableItems.size(); i++) {
        if (checkIfHovered(tttView->mousePosition, hoverableItems[i]) &&
            gm.checkWinner() == emptyCellMark) {
          if (gm.getCellsState()[i] == emptyCellMark && !gm.computerTurn())
            gm.modifyCells(i);
        }
      }
      tttView->setCells(gm.getCellsState());
      tttView->determinWinner(gm.checkWinner());
      tttView->setScore(gm.getScore());
    } else if (auto *difficultyView =
                   dynamic_cast<DifficultyView *>(vm.getCurrentView())) {
      int idx = 0;
      for (auto bounds : difficultyView->hoverableItems) {

        if (checkIfHovered(difficultyView->mousePosition, bounds)) {
          std::array<char, 9> cells;
          cells.fill(emptyCellMark);

          vm.setView(std::make_unique<TTTView>(cells));
          gm.resetScore();
          if (idx == 0)
            gm.setPlayStrategy(new BeginnerAiStrategy());
          else if (idx == 1)
            gm.setPlayStrategy(new IntermediateAiStrategy());
          else if (idx == 2)
            gm.setPlayStrategy(new ExpertAiStrategy());
        }
        idx++;
      }
    }
  }
}
void GameController::run() {
  vm.setView(std::make_unique<MenuView>());
  gm.setPlayStrategy(new ExpertAiStrategy());
  while (vm.window.isOpen()) {
    while (const std::optional event = vm.window.pollEvent()) {
      // "close requested" event: we close the window
      if (event->is<sf::Event::Closed>())
        vm.window.close();
      else if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
        if (auto *tempview = dynamic_cast<TTTView *>(vm.getCurrentView()))
          if (keyPressed->scancode == sf::Keyboard::Scancode::R) {
            gm.resetBoard();
            tempview->resetGame();
          }
        if (keyPressed->scancode == sf::Keyboard::Scancode::M) {
          vm.setView(std::make_unique<MenuView>());
        }
      } else if (const auto *mouseClicked =
                     event->getIf<sf::Event::MouseButtonPressed>()) {
        handleMouseClick(vm, gm);
      }
    }
    vm.window.clear(sf::Color::White);
    vm.renderView();
    if (gm.computerTurn()) {
      gm.modifyCells(-1);
      if (auto *tttView = dynamic_cast<TTTView *>(vm.getCurrentView())) {
        tttView->setCells(gm.getCellsState());
        tttView->determinWinner(gm.checkWinner());
        tttView->setScore(gm.getScore());
      }
    }
    vm.window.display();
  }
}
GameController::GameController(GameModel &gm, GameView &vm)
    : gm(std::move(gm)), vm(std::move(vm)) {}
