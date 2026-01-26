// will handle user inputs and will be entry point
#include "GameController.hpp"
void switchViewIfApplicable(GameView &vm) {
  if (auto *currentView = vm.getCurrentView()) {

    if (auto *currentView = dynamic_cast<MenuView *>(vm.getCurrentView())) {
      for (auto bounds : vm.getCurrentView()->hoverableItems) {
        if (checkIfHovered(vm.getCurrentView()->mousePosition, bounds)) {
          std::array<char, 9> cells;
          cells.fill('_');
          vm.setView(std::make_unique<TTTView>(cells));
        }
      }
    } else if (auto *currentView =
                   dynamic_cast<TTTView *>(vm.getCurrentView())) {
      vm.setView(std::make_unique<MenuView>());
    }
  }
}
void GameController::run() {
  std::array<char, 9> cells;
  cells.fill('_');
  vm.setView(std::make_unique<MenuView>());
  while (vm.window.isOpen()) {
    // check all the window's events that were triggered since the last
    // iteration of the loop
    while (const std::optional event = vm.window.pollEvent()) {
      // "close requested" event: we close the window
      if (event->is<sf::Event::Closed>())
        vm.window.close();
      else if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scancode::A)
          vm.setView(std::make_unique<MenuView>());
        if (keyPressed->scancode == sf::Keyboard::Scancode::B)
          vm.setView(std::make_unique<TTTView>(cells));
      } else if (const auto *mouseClicked =
                     event->getIf<sf::Event::MouseButtonPressed>()) {
        switchViewIfApplicable(vm);
      }
    }
    vm.window.clear(sf::Color::White);
    vm.renderView();
    vm.window.display();
  }
}
GameController::GameController(GameModel &gm, GameView &vm)
    : gm(std::move(gm)), vm(std::move(vm)) {}
