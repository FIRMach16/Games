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
  if (mousePosition.x > bounds.position.x + BAR_THICKNESS &&
      mousePosition.x < bounds.position.x + bounds.size.x - BAR_THICKNESS &&
      mousePosition.y > bounds.position.y + BAR_THICKNESS &&
      mousePosition.y < bounds.position.y + bounds.size.y - BAR_THICKNESS)
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
sf::Text XOMark(char mark, sf::Font Font, sf::FloatRect bounds) {
  sf::Text XOMark = sf::Text(Font, mark, 24);

  centerText(bounds, XOMark);
  if (mark == Xmark) {
    XOMark.setFillColor(sf::Color::Blue);
  } else if (mark == Omark) {
    XOMark.setFillColor(sf::Color::Red);
  }
  return XOMark;
}
sf::Text WinText(char mark, sf::Font Font) {

  sf::Text winMsg = sf::Text(Font);
  if (mark == Xmark) {
    winMsg.setString("X wins! press (r) to replay.");
    winMsg.setFillColor(sf::Color::Blue);
  } else if (mark == Omark) {
    winMsg.setString("O wins! press (r) to replay.");
    winMsg.setFillColor(sf::Color::Red);
  }
  winMsg.setCharacterSize(24);
  centerText(UPPER_TEXT_ZONES[1], winMsg);
  return winMsg;
}

GameView::GameView()
    : window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "TicTacToe") {}
void GameView::renderView() { window.draw(*currentView); }
void GameView::setMode(Mode newMode) { gameMode = newMode; }
void GameView::setView(std::unique_ptr<CurrentView> view) {
  currentView = std::move(view);
}
CurrentView *GameView::getCurrentView() { return currentView.get(); }
void TTTView::resetGame() {
  gameOver = false;
  winner = emptyCellMark;
  cells.fill(emptyCellMark);
}
void TTTView::setCells(std::array<char, 9> newCells) { cells = newCells; }
void TTTView::determinWinner(char mark) {
  if (mark != emptyCellMark) {
    gameOver = true;
    winner = mark;
  }
}
TTTView::TTTView(std::array<char, 9> &cells) : cells(cells) {
  hoverableItems.clear();
  cells.fill(emptyCellMark);
  for (int i = 0; i < cellBoxes.size(); i++) {
    cellBoxes[i].setSize(
        {TTT_BOXES_BOUNDS[i].size.x, TTT_BOXES_BOUNDS[i].size.y});
    cellBoxes[i].setPosition(
        {TTT_BOXES_BOUNDS[i].position.x, TTT_BOXES_BOUNDS[i].position.y});
    cellBoxes[i].setOutlineColor(sf::Color::Black);
    cellBoxes[i].setOutlineThickness(BAR_THICKNESS);
    hoverableItems.push_back(cellBoxes[i].getGlobalBounds());
  }
}
void TTTView::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  sf::RenderWindow &window = static_cast<sf::RenderWindow &>(target);
  mousePosition = sf::Mouse::getPosition(window);
  for (sf::RectangleShape rect : cellBoxes)
    target.draw(rect);
  int idx = 0;
  for (auto hoverableItem : hoverableItems) {
    if (checkIfHovered(mousePosition, hoverableItem) &&
        this->cells[idx] == emptyCellMark && !gameOver) {
      target.draw(highlighter(hoverableItem));
    }
    idx++;
  }
  if (gameOver && winner != emptyCellMark) {
    target.draw(WinText(winner, textFont));
  }
  idx = 0;
  for (auto mark : cells) {
    if (mark != emptyCellMark) {
      target.draw(XOMark(mark, textFont, hoverableItems[idx]));
    }
    idx++;
  }
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
