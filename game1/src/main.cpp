#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Window.hpp>
#include <algorithm>
#include <array>
#include <cmath>
#include <experimental/random>
#include <iostream>
#include <queue>
#include <string>
#include <vector>
using namespace std;
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

struct Line {
  int a, b, c;
};
vector<Line> winConditions{{0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {0, 3, 6},
                           {1, 4, 7}, {2, 5, 8}, {0, 4, 8}, {2, 4, 6}};

constexpr std::array<sf::FloatRect, 9> TTT_BOXES_BOUNDS = [] {
  std::array<sf::FloatRect, 9> bounds = {};
  for (int i = 0; i < 9; i++) {
    int row = (i / 3), column = i % 3; // 4 -> row 1 column 1 we start from 0
    bounds[i] = {{BOX_X_OFFSET + column * (BOX_SIDE / 3.0f),
                  BOX_Y_OFFSET + row * (BOX_SIDE / 3.0f)},
                 {BOX_SIDE / 3.0f, BOX_SIDE / 3.0f}};
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

enum class BoxState { EMPTY, X, O };
enum class Player { X, O, NONE };
enum class Mode {
  MENU,
  NORMAL_TTT,
  INFINITE_TTT,
  VS_COMPUTER, // upcoming
  ONLINE       // upcoming
}; // TTT means TicTacToe;
constexpr std::array<Mode, 4> MENU_MODES = {
    Mode::NORMAL_TTT, Mode::INFINITE_TTT, Mode::VS_COMPUTER, Mode::ONLINE};
struct AppState {
  Mode mode = Mode::MENU;
};

struct GameState {
  int roundsCounter = 0;
  int numberOfX = 0;
  int numberOfO = 0;
  std::array<int, 2> score = {}; // zero-initialized
  std::array<BoxState, 9> Boxes = {};
  std::queue<int> order;

  Player roundWinner = Player::NONE;
  Player currentPlayer = Player::X;

  void reset_round() {
    if (roundsCounter % 2 == 0)
      currentPlayer = Player::X;
    else
      currentPlayer = Player::O;
    Boxes.fill(BoxState::EMPTY);
    numberOfX = 0;
    numberOfO = 0;

    roundWinner = Player::NONE;
    while (!order.empty()) {
      order.pop();
    }
  }
  void
  reset_score() { // will call this when i change the app state or with when the
                  // players want to reset the score via (keybord or mouse)
    score.fill(0);
    currentPlayer = Player::X;
    Boxes.fill(BoxState::EMPTY);
    roundsCounter = 0;
    numberOfO = 0;
    numberOfX = 0;
    roundWinner = Player::NONE;

    while (!order.empty()) {

      order.pop();
    }
  }
};
void drawTTTHighlights(sf::RenderWindow &window, sf::Color &color,
                       sf::Vector2i mousePos, GameState &gameState) {
  for (int i = 0; i < 9; i++) {
    if (mousePos.x > TTT_BOXES_BOUNDS[i].position.x &&
        mousePos.x <
            TTT_BOXES_BOUNDS[i].position.x + TTT_BOXES_BOUNDS[i].size.x &&
        mousePos.y > TTT_BOXES_BOUNDS[i].position.y &&
        mousePos.y <
            TTT_BOXES_BOUNDS[i].position.y + TTT_BOXES_BOUNDS[i].size.y &&
        gameState.Boxes[i] == BoxState::EMPTY &&
        gameState.roundWinner == Player::NONE) {

      sf::RectangleShape highlighter(TTT_BOXES_BOUNDS[i].size);
      highlighter.setPosition(TTT_BOXES_BOUNDS[i].position);
      highlighter.setFillColor(color);
      window.draw(highlighter);
    }
  }
}
void drawMenuHighlights(sf::RenderWindow &window, sf::Color &color,
                        sf::Vector2i &mousePos) {
  for (int i = 0; i < 4; i++) {
    if (mousePos.x > MENU_ITEMS_BOUNDS[i].position.x &&
        mousePos.x <
            MENU_ITEMS_BOUNDS[i].position.x + MENU_ITEMS_BOUNDS[i].size.x &&
        mousePos.y > MENU_ITEMS_BOUNDS[i].position.y &&
        mousePos.y <
            MENU_ITEMS_BOUNDS[i].position.y + MENU_ITEMS_BOUNDS[i].size.y) {

      sf::RectangleShape highlighter(MENU_ITEMS_BOUNDS[i].size);
      highlighter.setPosition(MENU_ITEMS_BOUNDS[i].position);
      highlighter.setFillColor(color);
      window.draw(highlighter);
    }
  }
}
void drawCenteredText(sf::RenderWindow &window, sf::FloatRect &bounds,
                      sf::Text text) {
  sf::FloatRect textBounds = text.getGlobalBounds();
  text.setOrigin({textBounds.size.x / 2.0f, textBounds.size.y / 2.0f});
  text.setPosition({bounds.position.x + bounds.size.x / 2.0f,
                    bounds.position.y + bounds.size.y / 2.0f});
  window.draw(text);
}
void renderMenu(sf::RenderWindow &window, sf::Font &textFont, sf::Color &color,
                sf::Vector2i &mousePos) {
  for (int i = 0; i < 4; i++) {
    sf::RectangleShape MenuItem(
        {MENU_ITEMS_BOUNDS[i].size.x, MENU_ITEMS_BOUNDS[i].size.y});
    MenuItem.setPosition(
        {MENU_ITEMS_BOUNDS[i].position.x, MENU_ITEMS_BOUNDS[i].position.y});
    MenuItem.setOutlineColor(sf::Color::Black);
    MenuItem.setOutlineThickness(10.0f);
    window.draw(MenuItem);
    drawMenuHighlights(window, color, mousePos);
    sf::Text menuItemTexts(textFont);
    menuItemTexts.setCharacterSize(24);
    menuItemTexts.setFillColor(sf::Color::Black);
    sf::FloatRect bounds = MENU_ITEMS_BOUNDS[i];
    switch (i) {
    case 0: {
      menuItemTexts.setString("Normal Tic Tac Toe");
      drawCenteredText(window, bounds, menuItemTexts);
      break;
    }
    case 1: {
      menuItemTexts.setString("Infinite Tic Tac Toe");
      drawCenteredText(window, bounds, menuItemTexts);
      break;
    }
    case 2: {
      menuItemTexts.setString("VsComputer");
      drawCenteredText(window, bounds, menuItemTexts);
      break;
    }
    case 3: {
      menuItemTexts.setString("Online");
      drawCenteredText(window, bounds, menuItemTexts);
      break;
    }
    }
  }
}

void drawBar(sf::RenderWindow &window, sf::Vector2f size, sf::Vector2f pos) {
  sf::RectangleShape bar(size);
  bar.setFillColor(sf::Color::Black);
  bar.setPosition(pos);
  window.draw(bar);
}
void drawXO(sf::RenderWindow &window, sf::Font &textFont,
            GameState &gameState) {
  sf::Text Xmark(textFont);
  sf::Text Omark(textFont);
  Xmark.setString("X");
  Xmark.setFillColor(sf::Color::Blue);
  Omark.setString("O");
  Omark.setFillColor(sf::Color::Red);
  Xmark.setCharacterSize(24);
  Omark.setCharacterSize(24);
  for (int i = 0; i < 9; i++) {
    if (gameState.Boxes[i] == BoxState::X) {
      sf::FloatRect bounds = TTT_BOXES_BOUNDS[i];
      drawCenteredText(window, bounds, Xmark);
    } else if (gameState.Boxes[i] == BoxState::O) {
      sf::FloatRect bounds = TTT_BOXES_BOUNDS[i];
      drawCenteredText(window, bounds, Omark);
    }
  }
  // this code helps the players know whose turn it is
  if (gameState.currentPlayer == Player::X) {
    window.draw(Xmark);
  } else if (gameState.currentPlayer == Player::O) {
    window.draw(Omark);
  }
}
Player checkForWinner(GameState &gameState) {
  for (int i = 0; i < winConditions.size(); i++) {
    if (gameState.Boxes[winConditions[i].a] == BoxState::X &&
        gameState.Boxes[winConditions[i].b] == BoxState::X &&
        gameState.Boxes[winConditions[i].c] == BoxState::X) {
      if (gameState.score[0] + gameState.score[1] <
          (gameState.roundsCounter + 1))
        gameState.score[0]++;
      return Player::X;
    } else if (gameState.Boxes[winConditions[i].a] == BoxState::O &&
               gameState.Boxes[winConditions[i].b] == BoxState::O &&
               gameState.Boxes[winConditions[i].c] == BoxState::O) {
      if (gameState.score[0] + gameState.score[1] <
          (gameState.roundsCounter + 1))
        gameState.score[1]++;
      return Player::O;
    }
  }
  return Player::NONE;
}
void drawGrid(sf::RenderWindow &window, sf::Font &textFont) {
  // tic  toe contour
  sf::RectangleShape countour({BOX_SIDE, BOX_SIDE});
  countour.setPosition({BOX_X_OFFSET, BOX_Y_OFFSET});
  countour.setOutlineColor(sf::Color::Black);
  countour.setOutlineThickness(BAR_THICKNESS);
  // window.draw(...);
  window.draw(countour);
  drawBar(
      window, {BAR_THICKNESS, BOX_SIDE},
      {-BAR_THICKNESS / 2.0f + BOX_X_OFFSET + BOX_SIDE / 3.0f, BOX_Y_OFFSET});
  drawBar(window, {BAR_THICKNESS, BOX_SIDE},
          {-BAR_THICKNESS / 2.0f + BOX_X_OFFSET + 2 * BOX_SIDE / 3.0f,
           BOX_Y_OFFSET});
  drawBar(
      window, {BOX_SIDE, BAR_THICKNESS},
      {BOX_X_OFFSET, -BAR_THICKNESS / 2.0f + BOX_Y_OFFSET + BOX_SIDE / 3.0f});
  drawBar(window, {BOX_SIDE, BAR_THICKNESS},
          {BOX_X_OFFSET,
           -BAR_THICKNESS / 2.0f + BOX_Y_OFFSET + 2 * BOX_SIDE / 3.0f});
}
void checkAndDrawWinnerAndScore(sf::RenderWindow &window, sf::Font &textFont,
                                GameState &gameState) {

  sf::Text winMsg(textFont);
  if (gameState.numberOfX >= 3 | gameState.numberOfO >= 3) {
    // check for win
    if (checkForWinner(gameState) == Player::X) {
      gameState.roundWinner = Player::X;
      gameState.currentPlayer = Player::NONE;
      winMsg.setString("X wins press (r) to replay");
      winMsg.setFillColor(sf::Color::Blue);
      winMsg.setCharacterSize(24);
      sf::FloatRect bounds =
          sf::FloatRect({0, 0}, {WINDOW_WIDTH, 3 * BAR_THICKNESS});
      drawCenteredText(window, bounds, winMsg);

    } else if (checkForWinner(gameState) == Player::O) {
      gameState.roundWinner = Player::O;

      gameState.currentPlayer = Player::NONE;
      winMsg.setString("O wins press (r) to replay");
      winMsg.setFillColor(sf::Color::Red);
      winMsg.setCharacterSize(24);
      sf::FloatRect bounds =
          sf::FloatRect({0, 0}, {WINDOW_WIDTH, 3 * BAR_THICKNESS});
      drawCenteredText(window, bounds, winMsg);
    }
  }
  sf::FloatRect bounds = sf::FloatRect(
      {0, WINDOW_HEIGHT - 8 * BAR_THICKNESS},
      {WINDOW_WIDTH, 3 * BAR_THICKNESS}); // Bottom square for drawing text
                                          // (score in this case)
  sf::Text score(textFont);
  score.setString("score (X vs O) -> " + std::to_string(gameState.score[0]) +
                  " - " + std::to_string(gameState.score[1]));
  score.setCharacterSize(24);
  score.setFillColor(sf::Color::Black);
  drawCenteredText(window, bounds, score);
}

void renderNormalTTT(sf::RenderWindow &window, sf::Font &textFont,
                     GameState &gameState, sf::Vector2i &mousePos,
                     sf::Color &color) {
  drawGrid(window, textFont);
  drawXO(window, textFont, gameState); // On the boxes
  drawTTTHighlights(window, color, mousePos, gameState);
  checkAndDrawWinnerAndScore(window, textFont, gameState);
}
void renderInfiniteTTT(sf::RenderWindow &window, sf::Font &textFont,
                       GameState &gameState, sf::Vector2i &mousePos,
                       sf::Color &color) {
  drawGrid(window, textFont);
  drawXO(window, textFont, gameState); // On the boxes
  drawTTTHighlights(window, color, mousePos, gameState);
  checkAndDrawWinnerAndScore(window, textFont, gameState);
}
int checkIfOneMoveWinsForO(sf::RenderWindow &window, GameState &gameState) {

  if (gameState.numberOfO >= 2) {
    for (int i = 0; i < winConditions.size(); i++) {
      if (gameState.Boxes[winConditions[i].a] == BoxState::O &&
          gameState.Boxes[winConditions[i].b] == BoxState::O &&
          gameState.Boxes[winConditions[i].c] == BoxState::EMPTY)
        return winConditions[i].c;
      else if (gameState.Boxes[winConditions[i].a] == BoxState::O &&
               gameState.Boxes[winConditions[i].b] == BoxState::EMPTY &&
               gameState.Boxes[winConditions[i].c] == BoxState::O)
        return winConditions[i].b;
      else if (gameState.Boxes[winConditions[i].a] == BoxState::EMPTY &&
               gameState.Boxes[winConditions[i].b] == BoxState::O &&
               gameState.Boxes[winConditions[i].c] == BoxState::O)
        return winConditions[i].a;
    }
  }
  return -1;
}
void computerPlay(sf::RenderWindow &window, GameState &gameState) {
  if (gameState.currentPlayer == Player::O) {
    std::mt19937 rng(std::random_device{}());
    std::vector<int> emptyBoxes;
    for (int i = 0; i < 9; i++) {
      if (gameState.Boxes[i] == BoxState::EMPTY) {
        emptyBoxes.push_back(i);
      }
    }

    if (emptyBoxes.empty()) {
      gameState.currentPlayer = Player::NONE;
      return; // end game
    }
    // check if it's possible to place an O so the computer wins
    int resultOfDumbAlgo = checkIfOneMoveWinsForO(window, gameState);
    if (resultOfDumbAlgo != -1) {
      // we got our winner
      gameState.Boxes[resultOfDumbAlgo] = BoxState::O;
    } else {
      // random O
      std::shuffle(emptyBoxes.begin(), emptyBoxes.end(), rng);
      gameState.Boxes[emptyBoxes[0]] = BoxState::O;
    }
    gameState.numberOfO++;
    gameState.currentPlayer = Player::X;
  }
}

void renderVsComputerTTT(sf::RenderWindow &window, sf::Font &textFont,
                         GameState &gameState, sf::Vector2i &mousePos,
                         sf::Color &color) {
  drawGrid(window, textFont);
  // for now the player will be X the computer will be O
  drawXO(window, textFont, gameState);
  drawTTTHighlights(window, color, mousePos, gameState);
  checkAndDrawWinnerAndScore(window, textFont, gameState);
  computerPlay(window, gameState);
}
bool checkTTTClick(sf::Vector2i &localPosition, sf::FloatRect &bounds,
                   GameState &gameState, int box) {
  if (localPosition.x > bounds.position.x &&
      localPosition.x < bounds.position.x + bounds.size.x &&
      localPosition.y > bounds.position.y &&
      localPosition.y < bounds.position.y + bounds.size.y &&
      gameState.Boxes[box] == BoxState::EMPTY &&
      gameState.roundWinner == Player::NONE)
    return true;
  else
    return false;
}
int main() {
  sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
                          "TicTacToe");

  sf::Font xoFont("../assets/HackNerdFontMono-Bold.ttf");
  bool transition = false; // to reset event loop
  AppState appState;
  GameState gameState;
  auto OnMouseClicked = [&](const sf::Event::MouseButtonPressed) {
    // draw "X" or "O" and  add the boxNumber to boxesWith X or O
    sf::Vector2i localPosition = sf::Mouse::getPosition(window);
    if (appState.mode == Mode::MENU) {
      for (int i = 0; i < MENU_MODES.size(); i++) {
        auto bounds = MENU_ITEMS_BOUNDS[i];
        if (localPosition.x > bounds.position.x &&
            localPosition.x < bounds.position.x + bounds.size.x &&
            localPosition.y > bounds.position.y &&
            localPosition.y < bounds.position.y + bounds.size.y) {
          appState.mode = MENU_MODES[i];
          gameState.reset_score();
          transition = true;
        }
      }
    } else if (appState.mode == Mode::NORMAL_TTT && !transition) {
      for (int i = 0; i < 9; i++) {
        auto bounds = TTT_BOXES_BOUNDS[i];
        if (checkTTTClick(localPosition, bounds, gameState, i)) {
          if (gameState.currentPlayer == Player::X) {
            gameState.Boxes[i] = BoxState::X;
            gameState.numberOfX++;
            gameState.currentPlayer = Player::O;
          } else {
            gameState.Boxes[i] = BoxState::O;
            gameState.numberOfO++;
            gameState.currentPlayer = Player::X;
          }
        }
      }
    } else if (appState.mode == Mode::INFINITE_TTT) {
      int firstBoxInQueue;
      for (int i = 0; i < 9; i++) {
        auto bounds = TTT_BOXES_BOUNDS[i];

        if (checkTTTClick(localPosition, bounds, gameState, i)) {
          if (gameState.currentPlayer == Player::X) {
            gameState.order.push(i);
            gameState.Boxes[i] = BoxState::X;
            gameState.numberOfX++;
            gameState.currentPlayer = Player::O;
            if (gameState.numberOfO == 3) {
              firstBoxInQueue = gameState.order.front();
              gameState.Boxes[firstBoxInQueue] = BoxState::EMPTY;
              gameState.order.pop();
              gameState.numberOfO--;
            }
          } else {
            gameState.order.push(i);
            gameState.Boxes[i] = BoxState::O;
            gameState.numberOfO++;
            gameState.currentPlayer = Player::X;
            if (gameState.numberOfX == 3) {
              firstBoxInQueue = gameState.order.front();
              gameState.Boxes[firstBoxInQueue] = BoxState::EMPTY;
              gameState.order.pop();
              gameState.numberOfX--;
            }
          }
        }
      }
    } else if (appState.mode == Mode::VS_COMPUTER) {
      for (int i = 0; i < 9; i++) {
        auto bounds = TTT_BOXES_BOUNDS[i];
        if (checkTTTClick(localPosition, bounds, gameState, i)) {
          // only X for now
          if (gameState.currentPlayer == Player::X) {
            gameState.Boxes[i] = BoxState::X;
            gameState.numberOfX++;
            gameState.currentPlayer = Player::O;
          }
        }
      }
    }
  };
  sf::Color highlighterColor = sf::Color::Cyan;
  highlighterColor.a = 50;
  while (window.isOpen()) {
    if (transition) {
      while (window.pollEvent()) {
      }
      transition = false;
    }
    if (!transition) {

      window.handleEvents([&](const sf::Event::Closed &) { window.close(); },
                          [&](const sf::Event::KeyPressed &key) {
                            if (key.scancode == sf::Keyboard::Scancode::Escape)
                              window.close();
                            if (key.scancode == sf::Keyboard::Scancode::R) {
                              gameState.roundsCounter++;
                              gameState.reset_round();
                            }
                            if (key.scancode == sf::Keyboard::Scancode::M) {
                              appState.mode = Mode::MENU; // return to menu
                            }
                          },
                          OnMouseClicked);
    }
    // clear the window
    window.clear(sf::Color::White);
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    // draw everything here...
    if (appState.mode == Mode::MENU) {
      renderMenu(window, xoFont, highlighterColor, mousePos);

    } else if (appState.mode == Mode::NORMAL_TTT) {
      renderNormalTTT(window, xoFont, gameState, mousePos, highlighterColor);
    } else if (appState.mode == Mode::INFINITE_TTT) {
      renderInfiniteTTT(window, xoFont, gameState, mousePos, highlighterColor);
    } else if (appState.mode == Mode::VS_COMPUTER) {
      renderVsComputerTTT(window, xoFont, gameState, mousePos,
                          highlighterColor);
    } else {
      gameState.reset_score();
    }
    // end the current frame
    window.display();
  }
}
