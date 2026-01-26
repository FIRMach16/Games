// will have diffrent game interace elements
// and will use SFML Graphics
#include "GameView.hpp"
#include <SFML/Graphics/Font.hpp>
#include <iostream>
void centerText(sf::FloatRect bounds, sf::Text &text) {
  sf::FloatRect textBounds = text.getGlobalBounds();
  text.setOrigin({textBounds.size.x / 2.0f, textBounds.size.y / 2.0f});
  text.setPosition({bounds.position.x + bounds.size.x / 2.0f,
                    bounds.position.y + bounds.size.y / 2.0f});
}
bool checkIfHovered(sf::Vector2i mousePosition, sf::FloatRect bounds) {
  if (mousePosition.x > bounds.position.x &&
      mousePosition.x < bounds.position.x + bounds.size.x &&
      mousePosition.y > bounds.position.y &&
      mousePosition.y < bounds.position.y + bounds.size.y)
    return true;
  return false;
}
sf::RectangleShape highlighter(sf::FloatRect bounds) {
  sf::RectangleShape highlighter = sf::RectangleShape(
      {bounds.size.x - BAR_THICKNESS * 2, bounds.size.y - BAR_THICKNESS * 2});
  highlighter.setPosition(
      {bounds.position.x + BAR_THICKNESS, bounds.position.y + BAR_THICKNESS});
  highlighter.setFillColor(sf::Color::Cyan);
  return highlighter;
}

GameView::GameView(std::array<char, 9> cells)
    : window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "TicTacToe") {
  this->cells = cells;
}
void GameView::renderView() { window.draw(*currentView); }
void GameView::setMode(Mode newMode) { gameMode = newMode; }
void GameView::setView(std::unique_ptr<CurrentView> view) {
  currentView = std::move(view);
}

TTTView::TTTView(std::array<char, 9> &cells) : cells(cells) {
  rect.setSize({300, 300});
  rect.setPosition({WINDOW_WIDTH / 2 - 300 / 2, WINDOW_HEIGHT / 2 - 300 / 2});
  rect.setFillColor(sf::Color::Red);
}
void TTTView::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  target.draw(rect);
}
MenuView::MenuView() {
  hoverableItems.clear();
  for (int i = 0; i < menuItemsContainers.size(); i++) {
    menuItemsContainers[i].setSize({menuItemWidth, menuItemHeight});
    menuItemsContainers[i].setPosition(
        {menuItemXOffset,
         menuFirstItemYOffset + i * (menuItemHeight + menuItemSpacing)});
    menuItemsContainers[i].setOutlineColor(sf::Color::Black);
    menuItemsContainers[i].setOutlineThickness(BAR_THICKNESS);

    menuItemsTexts[i] = std::make_unique<sf::Text>(textFont, MENU_ITEMS[i], 24);
    auto &text = *menuItemsTexts[i];
    text.setFillColor(sf::Color::Black);
    centerText(menuItemsContainers[i].getGlobalBounds(), text);
    hoverableItems.push_back({menuItemsContainers[i].getGlobalBounds()});
  }
}
void MenuView::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  sf::RenderWindow &window = static_cast<sf::RenderWindow &>(target);
  mousePosition = sf::Mouse::getPosition(window);
  for (sf::RectangleShape rect : menuItemsContainers)
    target.draw(rect);
  for (auto hoverableItem : hoverableItems) {
    if (checkIfHovered(mousePosition, hoverableItem)) {
      target.draw(highlighter(hoverableItem));
    }
  }
  for (auto &text : menuItemsTexts) {
    target.draw(*text);
  }
}
