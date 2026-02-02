#include "SharedEnums.hpp"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Transformable.hpp>
constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;
constexpr float BOX_SIDE = 400.0f;
constexpr float BOX_X_OFFSET = (WINDOW_WIDTH - BOX_SIDE) / 2.0f;
constexpr float BOX_Y_OFFSET = (WINDOW_HEIGHT - BOX_SIDE) / 2.0f;
constexpr float BAR_THICKNESS = 10.0f;
// menu items will be horizantal bars
constexpr float menuItemWidth = 400.0f;
constexpr float menuItemHeight = 50.0f;
constexpr float menuItemSpacing = 50.0f;
constexpr float menuItemXOffset = 200.0f;
constexpr float menuFirstItemYOffset = 100.0f;

constexpr std::array<sf::FloatRect, 9> TTT_BOXES_BOUNDS = [] {
  std::array<sf::FloatRect, 9> bounds = {};
  for (int i = 0; i < 9; i++) {
    int row = (i / 3), column = i % 3; // 4 -> row 1 column 1 we start from 0
    bounds[i] = {{BOX_X_OFFSET + column * (BOX_SIDE / 3.0f + BAR_THICKNESS),
                  BOX_Y_OFFSET + row * (BOX_SIDE / 3.0f + BAR_THICKNESS)},
                 {BOX_SIDE / 3.0f, BOX_SIDE / 3.0f}};
  }
  return bounds;
}();
constexpr std::array<sf::FloatRect, 3> DIFFICULTY_MENU_ITEMS_BOUNDS = [] {
  std::array<sf::FloatRect, 3> bounds = {};
  for (int i = 0; i < 3; i++) {
    bounds[i] = {
        {menuItemXOffset,
         menuFirstItemYOffset + i * (menuItemSpacing * 2 + menuItemHeight)},
        {menuItemWidth, menuItemHeight}};
  }
  return bounds;
}();
constexpr std::array<sf::FloatRect, 4> MENU_ITEMS_BOUNDS = [] {
  std::array<sf::FloatRect, 4> bounds = {};
  for (int i = 0; i < 4; i++) {
    bounds[i] = {{menuItemXOffset, menuFirstItemYOffset +
                                       i * (menuItemSpacing + menuItemHeight)},
                 {menuItemWidth, menuItemHeight}};
  }
  return bounds;
}();
constexpr std::array<const char *, 4> MENU_ITEMS = {
    "Normal Mode", "Infinite Mode", "Vs Computer", "Online"};
constexpr std::array<const char *, 3> DIFFICULTY_MENU_ITEMS = {
    "Beginner", "Intermediate", "Expert"};
constexpr std::array<sf::FloatRect, 3> UPPER_TEXT_ZONES = {
    sf::FloatRect({0, 0}, {BOX_X_OFFSET, BOX_Y_OFFSET}),
    sf::FloatRect({BOX_X_OFFSET, 0}, {BOX_SIDE, BOX_Y_OFFSET}),
    sf::FloatRect({BOX_SIDE + BOX_X_OFFSET, 0}, {BOX_X_OFFSET, BOX_Y_OFFSET})};
constexpr std::array<sf::FloatRect, 9> THIRDS_OF_UPPER_TEXT_ZONES = [] {
  std::array<sf::FloatRect, 9> bounds = {};
  for (int partitionNum = 0; partitionNum < 9; partitionNum++) {
    int column = partitionNum / 3;
    int row = partitionNum % 3;
    auto concernedColumn = UPPER_TEXT_ZONES[column];
    bounds[partitionNum] = {
        {concernedColumn.position.x,
         concernedColumn.position.y + row * (concernedColumn.size.y / 3)},
        {concernedColumn.size.x, concernedColumn.size.y / 3}};
  }
  return bounds;
}();

class CurrentView : public sf::Drawable, public sf::Transformable {
  virtual void draw(sf::RenderTarget &target,
                    sf::RenderStates states) const override = 0;

public:
  sf::Font textFont = sf::Font("../assets/HackNerdFontMono-Bold.ttf");
  std::vector<sf::FloatRect> hoverableItems;
  mutable sf::Vector2i mousePosition;
};
class MenuView : public CurrentView {
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

public:
  std::array<sf::RectangleShape, 4> mainMenuItemsContainers;
  std::array<std::unique_ptr<sf::Text>, 4> menuItemsTexts;
  MenuView();
};
class DifficultyView : public CurrentView {
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

public:
  std::array<sf::RectangleShape, 3> dificultyMenuItemsContainers;
  std::array<std::unique_ptr<sf::Text>, 3> dificultyMenuItemsTexts;
  DifficultyView();
};
class TTTView : public CurrentView {
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
  bool gameOver = false;
  char winner = emptyCellMark;
  std::array<int, 2> score = {0, 0};
  std::array<char, 9> cells;

public:
  TTTView(std::array<char, 9> &cells);
  std::array<sf::RectangleShape, 9> cellBoxes;
  void setCells(std::array<char, 9> newCells);
  void resetGame();
  void setScore(std::array<int, 2> newScore);
  void determinWinner(char mark);
};

void centerText(sf::FloatRect bounds, sf::Text &text);
bool checkIfHovered(sf::Vector2i mousePosition, sf::FloatRect bounds);
sf::RectangleShape highlighter(sf::FloatRect bounds);
sf::Text XOMark(char mark, sf::Font Font, sf::FloatRect &bounds);

class GameView {
  std::unique_ptr<CurrentView> currentView;
  Mode gameMode = Mode::MENU;

public:
  GameView();
  void setMode(Mode newMode);
  void renderView();
  void setView(std::unique_ptr<CurrentView> view);
  CurrentView *getCurrentView();
  sf::RenderWindow window;
};
