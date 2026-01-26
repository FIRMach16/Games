#include "GameController.hpp"
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
#include <array>
#include <cmath>
#include <experimental/random>
#include <string>
using namespace std;
constexpr std::array<const char *, 3> aiDifficulties = {
    "Beginner", "Intermediate", "Expert"};
constexpr std::array<const char *, 4> menuItems = {
    "Normal TicTacToe", "Infite TicTacToe", "VsComputer", "Online"};

// void drawTTTHighlights(sf::RenderWindow &window, sf::Color &color,
//                        sf::Vector2i mousePos, GameState &gameState) {
//   for (int i = 0; i < 9; i++) {
//     if (mousePos.x > TTT_BOXES_BOUNDS[i].position.x &&
//         mousePos.x <
//             TTT_BOXES_BOUNDS[i].position.x + TTT_BOXES_BOUNDS[i].size.x &&
//         mousePos.y > TTT_BOXES_BOUNDS[i].position.y &&
//         mousePos.y <
//             TTT_BOXES_BOUNDS[i].position.y + TTT_BOXES_BOUNDS[i].size.y &&
//         gameState.Cells[i] == CellState::EMPTY) {
//
//       sf::RectangleShape highlighter(TTT_BOXES_BOUNDS[i].size);
//       highlighter.setPosition(TTT_BOXES_BOUNDS[i].position);
//       highlighter.setFillColor(color);
//       window.draw(highlighter);
//     }
//   }
// }
// void drawMenuHighlights(sf::RenderWindow &window, sf::Color &color,
//                         sf::Vector2i &mousePos) {
//   for (int i = 0; i < 4; i++) {
//     if (mousePos.x > MENU_ITEMS_BOUNDS[i].position.x &&
//         mousePos.x <
//             MENU_ITEMS_BOUNDS[i].position.x + MENU_ITEMS_BOUNDS[i].size.x &&
//         mousePos.y > MENU_ITEMS_BOUNDS[i].position.y &&
//         mousePos.y <
//             MENU_ITEMS_BOUNDS[i].position.y + MENU_ITEMS_BOUNDS[i].size.y) {
//
//       sf::RectangleShape highlighter(MENU_ITEMS_BOUNDS[i].size);
//       highlighter.setPosition(MENU_ITEMS_BOUNDS[i].position);
//       highlighter.setFillColor(color);
//       window.draw(highlighter);
//     }
//   }
// }
// void renderDifficultyMenu(sf::RenderWindow &renderWindow, sf::Font textFont,
//                           sf::Color &highlighterColor,
//                           sf::Vector2i &mousePosition) {
//   for (int i = 0; i < 3; i++) {
//     sf::RectangleShape difficultyMenuItem(
//         {DIFFICULTY_MENU_ITEMS_BOUNDS[i].size.x,
//          DIFFICULTY_MENU_ITEMS_BOUNDS[i].size.y});
//     difficultyMenuItem.setPosition(
//         {DIFFICULTY_MENU_ITEMS_BOUNDS[i].position.x,
//          DIFFICULTY_MENU_ITEMS_BOUNDS[i].position.y});
//     difficultyMenuItem.setOutlineColor(sf::Color::Black);
//     difficultyMenuItem.setOutlineThickness(10.0f);
//     renderWindow.draw(difficultyMenuItem);
//     sf::Text menuItemTexts(textFont);
//     menuItemTexts.setCharacterSize(24);
//     menuItemTexts.setFillColor(sf::Color::Black);
//     menuItemTexts.setString(aiDifficulties[i]);
//     drawCenteredText(renderWindow, DIFFICULTY_MENU_ITEMS_BOUNDS[i],
//                      menuItemTexts);
//   }
// }
// void renderMenu(sf::RenderWindow &window, sf::Font &textFont, sf::Color
// &color,
//                 sf::Vector2i &mousePos) {
//   for (int i = 0; i < 4; i++) {
//     sf::RectangleShape MenuItem(
//         {MENU_ITEMS_BOUNDS[i].size.x, MENU_ITEMS_BOUNDS[i].size.y});
//     MenuItem.setPosition(
//         {MENU_ITEMS_BOUNDS[i].position.x, MENU_ITEMS_BOUNDS[i].position.y});
//     MenuItem.setOutlineColor(sf::Color::Black);
//     MenuItem.setOutlineThickness(10.0f);
//     window.draw(MenuItem);
//     drawMenuHighlights(window, color, mousePos);
//     sf::Text menuItemTexts(textFont);
//     menuItemTexts.setCharacterSize(24);
//     menuItemTexts.setFillColor(sf::Color::Black);
//     menuItemTexts.setString(menuItems[i]);
//     drawCenteredText(window, MENU_ITEMS_BOUNDS[i], menuItemTexts);
//   }
// }
//
// void drawBar(sf::RenderWindow &window, sf::Vector2f size, sf::Vector2f pos) {
//   sf::RectangleShape bar(size);
//   bar.setFillColor(sf::Color::Black);
//   bar.setPosition(pos);
//   window.draw(bar);
// }
// void drawXO(sf::RenderWindow &window, sf::Font &textFont,
//             GameState &gameState) {
//   sf::Text Xmark(textFont);
//   sf::Text Omark(textFont);
//   Xmark.setString("X");
//   Xmark.setFillColor(sf::Color::Blue);
//   Omark.setString("O");
//   Omark.setFillColor(sf::Color::Red);
//   Xmark.setCharacterSize(24);
//   Omark.setCharacterSize(24);
//   for (int i = 0; i < 9; i++) {
//     if (gameState.Cells[i] == CellState::X) {
//       drawCenteredText(window, TTT_BOXES_BOUNDS[i], Xmark);
//     } else if (gameState.Cells[i] == CellState::O) {
//       drawCenteredText(window, TTT_BOXES_BOUNDS[i], Omark);
//     }
//   }
//   // this code helps the players know whose turn it is
//   if (gameState.currentPlayer == Player::X) {
//     window.draw(Xmark);
//   } else if (gameState.currentPlayer == Player::O) {
//     window.draw(Omark);
//   }
// }
//
// void drawGrid(sf::RenderWindow &window, sf::Font &textFont) {
//   // tic  toe contour
//   sf::RectangleShape countour({BOX_SIDE, BOX_SIDE});
//   countour.setPosition({BOX_X_OFFSET, BOX_Y_OFFSET});
//   countour.setOutlineColor(sf::Color::Black);
//   countour.setOutlineThickness(BAR_THICKNESS);
//   // window.draw(...);
//   window.draw(countour);
//   drawBar(
//       window, {BAR_THICKNESS, BOX_SIDE},
//       {-BAR_THICKNESS / 2.0f + BOX_X_OFFSET + BOX_SIDE / 3.0f,
//       BOX_Y_OFFSET});
//   drawBar(window, {BAR_THICKNESS, BOX_SIDE},
//           {-BAR_THICKNESS / 2.0f + BOX_X_OFFSET + 2 * BOX_SIDE / 3.0f,
//            BOX_Y_OFFSET});
//   drawBar(
//       window, {BOX_SIDE, BAR_THICKNESS},
//       {BOX_X_OFFSET, -BAR_THICKNESS / 2.0f + BOX_Y_OFFSET + BOX_SIDE
//       / 3.0f});
//   drawBar(window, {BOX_SIDE, BAR_THICKNESS},
//           {BOX_X_OFFSET,
//            -BAR_THICKNESS / 2.0f + BOX_Y_OFFSET + 2 * BOX_SIDE / 3.0f});
// }
// void checkWinnerAndIncrementScore(GameState &gameState) {
//
//   if (gameState.numberOfX >= 3 | gameState.numberOfO >= 3) {
//     if (checkForWinner(gameState.Cells) == Player::X) {
//       gameState.roundWinner = Player::X;
//       gameState.score[0]++;
//     } else if (checkForWinner(gameState.Cells) == Player::O) {
//       gameState.score[1]++;
//       gameState.roundWinner = Player::O;
//     }
//   }
// }
// void drawWinnerAndScore(sf::RenderWindow &window, sf::Font &textFont,
//                         GameState &gameState) {
//   sf::Text winMsg(textFont);
//
//   if (gameState.roundWinner == Player::NONE) {
//     checkWinnerAndIncrementScore(gameState);
//   } else {
//     if (gameState.roundWinner == Player::X) {
//       winMsg.setString("X wins press (r) to replay");
//       winMsg.setFillColor(sf::Color::Blue);
//       winMsg.setCharacterSize(24);
//     } else {
//       winMsg.setString("O wins press (r) to replay");
//       winMsg.setFillColor(sf::Color::Red);
//       winMsg.setCharacterSize(24);
//     }
//     sf::FloatRect bounds =
//         sf::FloatRect({0, 0}, {WINDOW_WIDTH, 3 * BAR_THICKNESS});
//     drawCenteredText(window, bounds, winMsg);
//   }
//
//   sf::FloatRect bounds = sf::FloatRect(
//       {0, WINDOW_HEIGHT - 8 * BAR_THICKNESS},
//       {WINDOW_WIDTH, 3 * BAR_THICKNESS}); // Bottom square for drawing text
//                                           // (score in this case)
//   sf::Text score(textFont);
//   score.setString("score (X vs O) -> " + std::to_string(gameState.score[0]) +
//                   " - " + std::to_string(gameState.score[1]));
//   score.setCharacterSize(24);
//   score.setFillColor(sf::Color::Black);
//   drawCenteredText(window, bounds, score);
// }
//
// void renderTTTwithAi(sf::RenderWindow &renderWindow, AppState &appState,
//                      GameState &gameState, sf::Font &textFont,
//                      sf::Vector2i mousePosition, sf::Color &highlighterColor,
//                      AiPlayer aiPlayer) {
//   drawGrid(renderWindow, textFont);
//   if (gameState.roundWinner == Player::NONE) {
//     drawTTTHighlights(renderWindow, highlighterColor, mousePosition,
//     gameState);
//   }
//   drawWinnerAndScore(renderWindow, textFont, gameState);
//   if (appState.mode == Mode::VS_COMPUTER &&
//       gameState.roundWinner == Player::NONE) {
//     aiPlayer.play();
//   }
//   drawXO(renderWindow, textFont, gameState);
// }
// void renderTTT(sf::RenderWindow &renderWindow, AppState &appState,
//                GameState &gameState, sf::Font &textFont,
//                sf::Vector2i mousePosition, sf::Color &highlighterColor) {
//
//   drawGrid(renderWindow, textFont);
//
//   if (gameState.roundWinner == Player::NONE) {
//
//     drawTTTHighlights(renderWindow, highlighterColor, mousePosition,
//     gameState);
//   }
//
//   drawWinnerAndScore(renderWindow, textFont, gameState);
//   drawXO(renderWindow, textFont, gameState);
// }
//
// bool checkTTTClick(sf::Vector2i &localPosition, sf::FloatRect &bounds,
//                    GameState &gameState, int box) {
//   if (localPosition.x > bounds.position.x &&
//       localPosition.x < bounds.position.x + bounds.size.x &&
//       localPosition.y > bounds.position.y &&
//       localPosition.y < bounds.position.y + bounds.size.y &&
//       gameState.Cells[box] == CellState::EMPTY &&
//       gameState.roundWinner == Player::NONE)
//     return true;
//   else
//     return false;
// }
// void handleClickInInfiniteTTT(GameState &gameState,
//                               sf::Vector2i &localPosition) {
//   int firstBoxInQueue;
//   for (int i = 0; i < 9; i++) {
//     auto bounds = TTT_BOXES_BOUNDS[i];
//
//     if (checkTTTClick(localPosition, bounds, gameState, i)) {
//       if (gameState.currentPlayer == Player::X) {
//         gameState.order.push(i);
//         gameState.Cells[i] = CellState::X;
//         gameState.numberOfX++;
//         gameState.currentPlayer = Player::O;
//         if (gameState.numberOfO == 3) {
//           firstBoxInQueue = gameState.order.front();
//           gameState.Cells[firstBoxInQueue] = CellState::EMPTY;
//           gameState.order.pop();
//           gameState.numberOfO--;
//         }
//       } else {
//         gameState.order.push(i);
//         gameState.Cells[i] = CellState::O;
//         gameState.numberOfO++;
//         gameState.currentPlayer = Player::X;
//         if (gameState.numberOfX == 3) {
//           firstBoxInQueue = gameState.order.front();
//           gameState.Cells[firstBoxInQueue] = CellState::EMPTY;
//           gameState.order.pop();
//           gameState.numberOfX--;
//         }
//       }
//     }
//   }
// }
// void handleClickInNormalTTT(GameState &gameState, sf::Vector2i
// &localPosition) {
//   for (int i = 0; i < 9; i++) {
//     auto bounds = TTT_BOXES_BOUNDS[i];
//     if (checkTTTClick(localPosition, bounds, gameState, i)) {
//       if (gameState.currentPlayer == Player::X) {
//         gameState.Cells[i] = CellState::X;
//         gameState.numberOfX++;
//         gameState.currentPlayer = Player::O;
//       } else {
//         gameState.Cells[i] = CellState::O;
//         gameState.numberOfO++;
//         gameState.currentPlayer = Player::X;
//       }
//     }
//   }
// }
// void handleClickInMainMenu(AppState &appState, GameState &gameState,
//                            bool &transition, sf::Vector2i &localPosition) {
//   for (int i = 0; i < MENU_MODES.size(); i++) {
//     auto bounds = MENU_ITEMS_BOUNDS[i];
//     if (localPosition.x > bounds.position.x &&
//         localPosition.x < bounds.position.x + bounds.size.x &&
//         localPosition.y > bounds.position.y &&
//         localPosition.y < bounds.position.y + bounds.size.y) {
//       appState.mode = MENU_MODES[i];
//       gameState.reset_score();
//       transition = true;
//     }
//   }
// }
// void handleClickInVsComputerTTT(GameState &gameState,
//                                 sf::Vector2i &localPosition, bool
//                                 &transition) {
//   if (gameState.aiDifficulty == AiDifficulty::NONE_SELECTED) {
//     for (int i = 0; i < AI_DIFFICULTIES_MODE.size(); i++) {
//       auto bounds = DIFFICULTY_MENU_ITEMS_BOUNDS[i];
//       if (localPosition.x > bounds.position.x &&
//           localPosition.x < bounds.position.x + bounds.size.x &&
//           localPosition.y > bounds.position.y &&
//           localPosition.y < bounds.position.y + bounds.size.y) {
//         gameState.aiDifficulty = AI_DIFFICULTIES_MODE[i];
//         transition = true;
//       }
//     }
//   } else {
//     for (int i = 0; i < 9; i++) {
//       auto bounds = TTT_BOXES_BOUNDS[i];
//       if (checkTTTClick(localPosition, bounds, gameState, i)) {
//         // only X for now
//         if (gameState.currentPlayer == Player::X) {
//           gameState.Cells[i] = CellState::X;
//           gameState.numberOfX++;
//           gameState.currentPlayer = Player::O;
//         }
//       }
//     }
//   }
// }
int main() {
  std::array<char, 9> arr;
  arr.fill('-');
  BeginnerAiStrategy beginner;
  AiPlayer aiPlayer = AiPlayer(arr);
  aiPlayer.setStrategy(&beginner);
  GameModel gm = GameModel(aiPlayer);
  GameView vm = GameView(arr);
  GameController gc = GameController(gm, vm);
  gc.run();
  // sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
  //                         "TicTacToe");
  // sf::Font xoFont("../assets/HackNerdFontMono-Bold.ttf");
  // bool transition = false; // to reset event loop
  // AppState appState;
  // GameState gameState;
  //
  // BeginnerDifficultyStrategy beginner;
  // IntermediateDifficultyStrategy intermediate;
  // ExpertDifficultyStrategy expert;
  // AiPlayer aiPlayer(gameState);
  // auto OnMouseClicked = [&](const sf::Event::MouseButtonPressed) {
  //   // draw "X" or "O" and  add the boxNumber to boxesWith X or O
  //   sf::Vector2i localPosition = sf::Mouse::getPosition(window);
  //   if (appState.mode == Mode::MENU) {
  //     handleClickInMainMenu(appState, gameState, transition, localPosition);
  //   } else if (appState.mode == Mode::NORMAL_TTT && !transition) {
  //     handleClickInNormalTTT(gameState, localPosition);
  //   } else if (appState.mode == Mode::INFINITE_TTT) {
  //     handleClickInInfiniteTTT(gameState, localPosition);
  //   } else if (appState.mode == Mode::VS_COMPUTER) {
  //     handleClickInVsComputerTTT(gameState, localPosition, transition);
  //   }
  // };
  // sf::Color highlighterColor = sf::Color::Cyan;
  // highlighterColor.a = 50;

  // while (window.isOpen()) {
  // if (transition) {
  //   while (window.pollEvent()) {
  //   }
  //   transition = false;
  // }
  // if (!transition) {
  //
  //   window.handleEvents([&](const sf::Event::Closed &) { window.close(); },
  //                       [&](const sf::Event::KeyPressed &key) {
  //                         if (key.scancode ==
  //                         sf::Keyboard::Scancode::Escape)
  //                           window.close();
  //                         if (key.scancode == sf::Keyboard::Scancode::R) {
  //                           gameState.roundsCounter++;
  //                           gameState.reset_round();
  //                         }
  //                         if (key.scancode == sf::Keyboard::Scancode::M) {
  //                           appState.mode = Mode::MENU; // return to menu
  //                         }
  //                       },
  //                       OnMouseClicked);
  // }
  // // clear the window
  // window.clear(sf::Color::White);
  // sf::Vector2i mousePos = sf::Mouse::getPosition(window);
  // // draw everything here...
  // if (appState.mode == Mode::MENU) {
  //   gameState.reset_score();
  //   renderMenu(window, xoFont, highlighterColor, mousePos);
  //
  // } else if (appState.mode == Mode::VS_COMPUTER) {
  //
  //   if (gameState.aiDifficulty == AiDifficulty::BEGINNER) {
  //     aiPlayer.setStrategy(&beginner);
  //     renderTTTwithAi(window, appState, gameState, xoFont, mousePos,
  //                     highlighterColor, aiPlayer);
  //
  //   } else if (gameState.aiDifficulty == AiDifficulty::INTERMIDIATE) {
  //     aiPlayer.setStrategy(&intermediate);
  //     renderTTTwithAi(window, appState, gameState, xoFont, mousePos,
  //                     highlighterColor, aiPlayer);
  //
  //   } else if (gameState.aiDifficulty == AiDifficulty::EXPERT) {
  //     aiPlayer.setStrategy(&expert);
  //     renderTTTwithAi(window, appState, gameState, xoFont, mousePos,
  //                     highlighterColor, aiPlayer);
  //   } else {
  //     renderDifficultyMenu(window, xoFont, highlighterColor, mousePos);
  //   }
  // } else {
  //   renderTTT(window, appState, gameState, xoFont, mousePos,
  //             highlighterColor);
  // }
  // // end the current frame
  // window.display();
  // }
}
