#include "SharedEnums.hpp"
#include <array>
#include <memory>
#include <vector>
class GameStateObserver {
public:
  virtual void update() = 0;
  bool operator==(const GameStateObserver &other) const {
    return this == &other;
  };
};
class GameStateObservable {
public:
  virtual void addObserver(GameStateObserver *obs) = 0;
  virtual void removeObserver(GameStateObserver *obs) = 0;
  virtual void notifyObservers() = 0;
};

struct Line {
  int a, b, c;
};
const std::array<char, 9> initCells = {
    emptyCellMark, emptyCellMark, emptyCellMark, emptyCellMark, emptyCellMark,
    emptyCellMark, emptyCellMark, emptyCellMark, emptyCellMark};
const std::vector<Line> winConditions{{0, 1, 2}, {3, 4, 5}, {6, 7, 8},
                                      {0, 3, 6}, {1, 4, 7}, {2, 5, 8},
                                      {0, 4, 8}, {2, 4, 6}};

enum class AiDifficulty { NONE_SELECTED, BEGINNER, INTERMIDIATE, EXPERT };
constexpr std::array<Mode, 4> MENU_MODES = {
    Mode::NORMAL_TTT, Mode::INFINITE_TTT, Mode::VS_COMPUTER, Mode::ONLINE};
constexpr std::array<AiDifficulty, 3> AI_DIFFICULTIES_MODE = {
    AiDifficulty::BEGINNER, AiDifficulty::INTERMIDIATE, AiDifficulty::EXPERT};

char checkForWinner(std::array<char, 9> cells);
int findWinningMove(std::array<char, 9> cells, char mark);
int randomMove(std::array<char, 9> &cells, std::vector<int> &emptyCells);
std::vector<int> findEmptyCells(std::array<char, 9> cells);

int evaluateBoard(std::array<char, 9> cells);
bool gameOver(std::array<char, 9> cells);
int minmaxingTTT(std::array<char, 9> cells, int depth, int alpha, int beta,
                 bool maximizingPlayer);
class AiPlayerStrategy {
public:
  virtual void play(std::array<char, 9> &cells) = 0;
};
class ExpertAiStrategy : public AiPlayerStrategy {
public:
  void play(std::array<char, 9> &cells) override;
};
class IntermediateAiStrategy : public AiPlayerStrategy {
public:
  void play(std::array<char, 9> &cells) override;
};
class BeginnerAiStrategy : public AiPlayerStrategy {
public:
  void play(std::array<char, 9> &cells) override;
};
class NoPlayStrategy : public AiPlayerStrategy {
public:
  void play(std::array<char, 9> &cells) override {}
};

class AiPlayer {
  std::shared_ptr<std::array<char, 9>> cells;
  std::unique_ptr<AiPlayerStrategy> strategy;

public:
  AiPlayer(std::shared_ptr<std::array<char, 9>> arr);
  void setStrategy(std::unique_ptr<AiPlayerStrategy> strategy);
  void play();
};
class GameModel : public GameStateObservable {
  std::shared_ptr<std::array<char, 9>> cells;
  char currentPlayer = Xmark;
  char computerPlayer = Omark; // stay fixed during rounds of VS_COMPUTER game
  int rounds = 0;
  AiPlayer aiPlayer;
  Mode appMode;
  std::array<int, 2> score;
  std::vector<int> orderOfMoves;

public:
  std::array<char, 9> getCellsState();
  std::vector<GameStateObserver *> observers;
  void resetBoard();
  void resetScore();
  void modifyCells(int cellNumber);
  void setGameMode(Mode newMode);
  char checkWinner();
  void setPlayStrategy(std::unique_ptr<AiPlayerStrategy> strategy);
  void setComputerPlayer(char newComputerPlayer);
  std::array<int, 2> getScore();
  GameModel();
  // want to avoid having gm.appMode == Mode::... in GameController
  bool is2player();
  bool isGame();
  bool computerTurn();
  // observables methods
  void removeObserver(GameStateObserver *obs) override;
  void addObserver(GameStateObserver *obs) override;
  void notifyObservers() override;
};
