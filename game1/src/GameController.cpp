// will handle user inputs and will be entry point
#include "GameController.hpp"
// #include <iostream>
void handleMouseClick(GameView &vm, GameModel &gm) {
  if (auto *currentView = vm.getCurrentView()) {
    if (auto *currentView = dynamic_cast<MenuView *>(vm.getCurrentView())) {
      for (auto bounds : currentView->hoverableItems) {
        if (checkIfHovered(currentView->mousePosition, bounds)) {
          std::array<char, 9> cells;
          cells.fill(emptyCellMark);
          vm.setView(std::make_unique<TTTView>(cells));
        }
      }
    } else if (auto *currentView =
                   dynamic_cast<TTTView *>(vm.getCurrentView())) {
      auto hoverableItems = currentView->hoverableItems;
      for (int i = 0; i < hoverableItems.size(); i++) {
        if (checkIfHovered(currentView->mousePosition, hoverableItems[i])) {
          if (gm.getCellsState()[i] == emptyCellMark)
            gm.modifyCells(i);
        }
      }
      currentView->setCells(gm.getCellsState());
    }
  }
}
void GameController::run() {
  vm.setView(std::make_unique<MenuView>());
  while (vm.window.isOpen()) {
    while (const std::optional event = vm.window.pollEvent()) {
      // "close requested" event: we close the window
      if (event->is<sf::Event::Closed>())
        vm.window.close();
      else if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
        if (auto *tempview = dynamic_cast<TTTView *>(vm.getCurrentView()))
          if (keyPressed->scancode == sf::Keyboard::Scancode::R) {
            gm.resetBoard();
            tempview->setCells(gm.getCellsState());
          }
      } else if (const auto *mouseClicked =
                     event->getIf<sf::Event::MouseButtonPressed>()) {
        handleMouseClick(vm, gm);
      }
    }
    vm.window.clear(sf::Color::White);
    vm.renderView();
    vm.window.display();
  }
}
GameController::GameController(GameModel &gm, GameView &vm)
    : gm(std::move(gm)), vm(std::move(vm)) {}
