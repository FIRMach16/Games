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
using namespace std;
constexpr std::array<const char *, 3> aiDifficulties = {
    "Beginner", "Intermediate", "Expert"};
constexpr std::array<const char *, 4> menuItems = {
    "Normal TicTacToe", "Infite TicTacToe", "VsComputer", "Online"};

int main() {
  std::array<char, 9> arr;
  arr.fill(emptyCellMark);
  BeginnerAiStrategy beginner;
  AiPlayer aiPlayer = AiPlayer(arr);
  aiPlayer.setStrategy(&beginner);
  GameModel gm = GameModel(aiPlayer);
  GameView vm;
  GameController gc = GameController(gm, vm);
  gc.run();
}
