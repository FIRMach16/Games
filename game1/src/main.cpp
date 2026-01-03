#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <iostream>
#include <unordered_set>
#include <vector>
using namespace std;
#define width 800
#define height 600
#define boxSide 400.0f
#define boxXoffset (width - boxSide) / 2.0f
#define boxYoffset (height - boxSide) / 2.0f
#define barThickness 10.0f
// menu items will be horizantal bars
#define menuItemWidth 400.0f
#define menuItemHeight 50.0f
#define menuItemSpacing 50.0f
#define menuItemXOffset 200.0f
#define menuFirstItemYOffset 100.0f
// area definitions
// area1
#define area1X1 boxXoffset + barThickness
#define area1X2 boxXoffset + boxSide / 3.0f
#define area1Y1 boxYoffset + barThickness
#define area1Y2 boxYoffset + boxSide / 3.0f
// area2
#define area2X1 boxXoffset + boxSide / 3.0f + barThickness
#define area2X2 boxXoffset + 2 * boxSide / 3.0f
#define area2Y1 boxYoffset + barThickness
#define area2Y2 boxYoffset + boxSide / 3.0f
// area3
#define area3X1 boxXoffset + 2 * boxSide / 3.0f + barThickness
#define area3X2 boxXoffset + boxSide - barThickness
#define area3Y1 boxYoffset + barThickness
#define area3Y2 boxYoffset + boxSide / 3.0f
// area4
#define area4X1 boxXoffset + barThickness
#define area4X2 boxXoffset + boxSide / 3.0f
#define area4Y1 boxYoffset + boxSide / 3.0f + barThickness
#define area4Y2 boxYoffset + 2 * boxSide / 3.0f
// area5
#define area5X1 boxXoffset + boxSide / 3.0f + barThickness
#define area5X2 boxXoffset + 2 * boxSide / 3.0f
#define area5Y1 boxYoffset + boxSide / 3.0f + barThickness
#define area5Y2 boxYoffset + 2 * boxSide / 3.0f

// area6
#define area6X1 boxXoffset + 2 * boxSide / 3.0f + barThickness
#define area6X2 boxXoffset + boxSide - barThickness
#define area6Y1 boxYoffset + boxSide / 3.0f + barThickness
#define area6Y2 boxYoffset + 2 * boxSide / 3.0f

// area7
#define area7X1 boxXoffset + barThickness
#define area7X2 boxXoffset + boxSide / 3.0f
#define area7Y1 boxYoffset + 2 * boxSide / 3.0f + barThickness
#define area7Y2 boxYoffset + boxSide - barThickness
// area8
#define area8X1 boxXoffset + boxSide / 3.0f + barThickness
#define area8X2 boxXoffset + 2 * boxSide / 3.0f
#define area8Y1 boxYoffset + 2 * boxSide / 3.0f + barThickness
#define area8Y2 boxYoffset + boxSide - barThickness
// area9
#define area9X1 boxXoffset + 2 * boxSide / 3.0f + barThickness
#define area9X2 boxXoffset + boxSide - barThickness
#define area9Y1 boxYoffset + 2 * boxSide / 3.0f + barThickness
#define area9Y2 boxYoffset + boxSide - barThickness
struct Line {
  int a, b, c;
};
vector<Line> winConditions{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {1, 4, 7},
                           {2, 5, 8}, {3, 6, 9}, {1, 5, 9}, {3, 5, 7}};
struct Area {
  float x1, x2, y1, y2;
  int boxIdx;
};
vector<Area> Areas = {
    {area1X1, area1X2, area1Y1, area1X1, 1},
    {area2X1, area2X2, area2Y1, area2Y2, 2},
    {area3X1, area3X2, area3Y1, area3Y2, 3},
    {area4X1, area4X2, area4Y1, area4Y2, 4},
    {area5X1, area5X2, area5Y1, area5Y2, 5},
    {area6X1, area6X2, area6Y1, area6Y2, 6},
    {area7X1, area7X2, area7Y1, area7Y2, 7},
    {area8X1, area8X2, area8Y1, area8Y2, 8},
    {area9X1, area9X2, area9Y1, area9Y2, 9},
};

int main() {
  // seeing areas limits:
  // for (const auto &area : Areas) {
  //   cout << "Area N°" << area.boxIdx << ": ( x1=" << area.x1
  //        << ",x2=" << area.x2 << ",y1=" << area.y1 << ",y2=" << area.y2 <<
  //        endl;
  // }
  // create the window
  sf::RenderWindow window(sf::VideoMode({width, height}), "My window");
  std::unordered_set<int> permittedBoxes = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::unordered_set<int> boxesWithX = {};
  std::unordered_set<int> boxesWithO = {};
  vector<sf::FloatRect> menuItemsBounds = {};
  bool menuItemsBoundDefined =
      false; // I need to add the bounds to menuItemsBounds only once
  sf::Font xoFont("../assets/HackNerdFontMono-Bold.ttf");
  int turn = 0;
  int boxNumber = 0;
  int mode = 0; // 0 : menu 1 : NormalTicTacToe  2 : InfiniteTicTacToe 3:
                // VsComputer 4:Online // should've went with enum class
  bool transition = false; // to reset event loop

  auto drawBar = [&](sf::Vector2f size, sf::Vector2f pos) {
    sf::RectangleShape bar(size);
    bar.setFillColor(sf::Color::Black);
    bar.setPosition(pos);
    window.draw(bar);
  };

  auto drawBoxHighlight = [&](int boxNumber) {
    if (boxNumber == 0)
      return;

    sf::RectangleShape highlighter({boxSide / 3 - 5.0f, boxSide / 3 - 5.0f});
    highlighter.setFillColor(sf::Color::Cyan);
    highlighter.setPosition({boxXoffset + ((boxNumber - 1) % 3) * boxSide / 3 +
                                 ((boxNumber - 1) % 3) * 5.0f / 2,
                             boxYoffset +
                                 ((boxNumber - 1) / 3) * boxSide / 3.0f +
                                 ((boxNumber - 1) / 3) * 5.0f / 2});

    window.draw(highlighter);
  };
  auto onMouseMoved = [&](const sf::Event::MouseMoved &) {
    sf::Vector2i localPosition = sf::Mouse::getPosition(window);
    float x = localPosition.x;
    float y = localPosition.y;
    // cout << "(" << x << "," << y << ")" << endl;
    boxNumber = 0;
    for (const auto &area : Areas) {

      if (x > area.x1 && x < area.x2 && y > area.y1 && y < area.y2 &&
          permittedBoxes.count(area.boxIdx) == 1) {
        boxNumber = area.boxIdx;
        break;
      }
    }

    // std::cout << boxNumber << endl;
  };
  auto drawXO = [&]() {
    sf::Text Xmark(xoFont);
    sf::Text Omark(xoFont);
    // X
    Xmark.setString("X");
    Xmark.setFillColor(sf::Color::Blue);
    Omark.setString("O");
    Omark.setFillColor(sf::Color::Red);
    Xmark.setCharacterSize(24);
    Omark.setCharacterSize(24);

    for (int i = 1; i < 10; i++) {
      if (boxesWithX.count(i) == 1) {
        Xmark.setPosition({Areas[i - 1].x1 + 5 * barThickness,
                           Areas[i - 1].y1 + 5 * barThickness});
        window.draw(Xmark);
      } else if (boxesWithO.count(i) == 1) {
        Omark.setPosition({Areas[i - 1].x1 + 5 * barThickness,
                           Areas[i - 1].y1 + 5 * barThickness});
        window.draw(Omark);
      }
    }
  };
  auto checkForWinner = [&]() {
    // 0 : keep playing , 1 : X wins  ,  2 : O wins
    for (const auto &winCondition : winConditions) {
      if (boxesWithX.count(winCondition.a) + boxesWithX.count(winCondition.b) +
              boxesWithX.count(winCondition.c) ==
          3) {
        return 1;
      } else if (boxesWithO.count(winCondition.a) +
                     boxesWithO.count(winCondition.b) +
                     boxesWithO.count(winCondition.c) ==
                 3) {
        return 2;
      }
    }
    return 0;
  };
  auto drawCenteredText = [&](sf::Text Text,
                              sf::FloatRect encapsulatingBodyBounds) {
    sf::FloatRect textBounds = Text.getGlobalBounds();
    Text.setOrigin({textBounds.size.x / 2.0f, textBounds.size.y / 2.0f});
    Text.setPosition({encapsulatingBodyBounds.position.x +
                          encapsulatingBodyBounds.size.x / 2.0f,
                      encapsulatingBodyBounds.position.y +
                          encapsulatingBodyBounds.size.y / 2.0f});
    window.draw(Text);
  };
  auto OnMouseClicked = [&](const sf::Event::MouseButtonPressed) {
    // draw "X" or "O" and  add the boxNumber to boxesWith X or O
    if (mode == 0) {
      sf::Vector2i localPosition = sf::Mouse::getPosition(window);
      for (int i = 0; i < menuItemsBounds.size(); i++) {
        auto bounds = menuItemsBounds[i];
        if (localPosition.x > bounds.position.x &&
            localPosition.x < bounds.position.x + bounds.size.x &&
            localPosition.y > bounds.position.y &&
            localPosition.y < bounds.position.y + bounds.size.y) {
          mode = i + 1;
          permittedBoxes = {1, 2, 3, 4, 5, 6, 7, 8, 9};
          boxesWithO = {};
          boxesWithX = {};
          turn = 0;
          transition = true;
        }
      }
    }
    if (mode == 1 && !transition) {
      if (boxNumber == 0 | permittedBoxes.count(boxNumber) == 0)
        return;

      if (turn % 2 == 0) {
        // X
        boxesWithX.insert(boxNumber);
      } else {
        boxesWithO.insert(boxNumber);
      }
      turn++;

      permittedBoxes.erase(boxNumber);
    }
  };
  auto drawNormalTicTacToe = [&]() {
    sf::Text Xmark(xoFont);
    sf::Text Omark(xoFont);
    sf::Text winMsg(xoFont);
    // X
    Xmark.setString("X");
    Xmark.setFillColor(sf::Color::Blue);
    Omark.setString("O");
    Omark.setFillColor(sf::Color::Red);
    Xmark.setCharacterSize(24);
    Omark.setCharacterSize(24);
    // tic  toe contour
    sf::RectangleShape countour({boxSide, boxSide});
    countour.setPosition({boxXoffset, boxYoffset});
    countour.setOutlineColor(sf::Color::Black);
    countour.setOutlineThickness(barThickness);
    // tic tac toe bars
    // window.draw(...);
    window.draw(countour);
    drawBar({barThickness, boxSide},
            {-barThickness / 2.0f + boxXoffset + boxSide / 3.0f, boxYoffset});
    drawBar(
        {barThickness, boxSide},
        {-barThickness / 2.0f + boxXoffset + 2 * boxSide / 3.0f, boxYoffset});
    drawBar({boxSide, barThickness},
            {boxXoffset, -barThickness / 2.0f + boxYoffset + boxSide / 3.0f});
    drawBar(
        {boxSide, barThickness},
        {boxXoffset, -barThickness / 2.0f + boxYoffset + 2 * boxSide / 3.0f});
    // highlighter
    drawBoxHighlight(boxNumber);
    drawXO(); // On the boxes

    // this code helps the players know whose turn it is
    if (turn % 2 == 0) {
      window.draw(Xmark);
    } else {
      window.draw(Omark);
    }
    if (boxesWithX.size() >= 3 | boxesWithO.size() >= 3) {
      // check for win
      if (checkForWinner() == 1) {
        permittedBoxes = {}; // this should end the game (a better way to do it
                             // is with a flag that controll the set)
        winMsg.setString("X wins press (r) to replay");
        winMsg.setFillColor(sf::Color::Blue);
        winMsg.setCharacterSize(24);
        sf::FloatRect textBounds = winMsg.getGlobalBounds();
        winMsg.setOrigin({textBounds.size.x / 2.0f, textBounds.size.y / 2.0f});
        winMsg.setPosition({width / 2.0f, 2 * barThickness});
        window.draw(winMsg);
      } else if (checkForWinner() == 2) {
        permittedBoxes = {}; // this should end the game (a better way to do it
                             // is with a flag that controll the set)
        winMsg.setString("O wins press (r) to replay");
        winMsg.setFillColor(sf::Color::Red);
        winMsg.setCharacterSize(24);
        sf::FloatRect textBounds = winMsg.getGlobalBounds();
        winMsg.setOrigin({textBounds.size.x / 2.0f, textBounds.size.y / 2.0f});
        winMsg.setPosition({width / 2.0f, 2 * barThickness});
        window.draw(winMsg);
      }
    }
  };
  auto drawMenu = [&]() {
    for (int i = 0; i < 4; i++) {
      sf::RectangleShape MenuItem({menuItemWidth, menuItemHeight});
      MenuItem.setPosition(
          {menuItemXOffset,
           menuFirstItemYOffset + i * (menuItemHeight + menuItemSpacing)});
      MenuItem.setOutlineColor(sf::Color::Black);
      MenuItem.setOutlineThickness(10.0f);
      window.draw(MenuItem);
      sf::Text menuItemTexts(xoFont);
      sf::FloatRect itemBounds = MenuItem.getGlobalBounds();
      if (menuItemsBoundDefined == false) {
        menuItemsBounds.push_back(itemBounds);
      }
      menuItemTexts.setCharacterSize(24);
      menuItemTexts.setFillColor(sf::Color::Black);

      switch (i) {
      case 0: {
        menuItemTexts.setString("Normal Tic Tac Toe");
        drawCenteredText(menuItemTexts, itemBounds);
        break;
      }
      case 1: {
        menuItemTexts.setString("Infinite Tic Tac Toe");
        drawCenteredText(menuItemTexts, itemBounds);
        break;
      }
      case 2: {
        menuItemTexts.setString("VsComputer");
        drawCenteredText(menuItemTexts, itemBounds);
        break;
      }
      case 3: {
        menuItemTexts.setString("Online");
        drawCenteredText(menuItemTexts, itemBounds);
        break;
      }
      }
    }
    menuItemsBoundDefined = true;
  };
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
                              permittedBoxes = {1, 2, 3, 4, 5, 6, 7, 8, 9};
                              boxesWithO = {};
                              boxesWithX = {};
                              turn = 0;
                            }
                            if (key.scancode == sf::Keyboard::Scancode::M) {
                              mode = 0; // return to menu
                            }
                          },
                          onMouseMoved, OnMouseClicked);
    }
    // clear the window
    window.clear(sf::Color::White);

    if (mode == 0) {
      drawMenu();
    } else if (mode == 1) {
      drawNormalTicTacToe();
    } else {
      // for now other modes are a white screen
    }
    // draw everything here...

    // end the current frame
    window.display();
  }
}
