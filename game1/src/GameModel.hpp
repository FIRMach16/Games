#include "SharedEnums.hpp"
#include <array>
#include <vector>
// class GameStateObserver {
// public:
//   virtual void update() = 0;
// };
// class GameStateObservable {
//   virtual void add(GameStateObserver obs) = 0;
//   virtual void remove(GameStateObserver obs) = 0;
//   virtual void notify() = 0;
// };
//
struct Line {
  int a, b, c;
};
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
void randomMove(std::array<char, 9> cells, std::vector<int> &emptyCells);
std::vector<int> findEmptyCells(std::array<char, 9> cells);

int evaluateBoard(std::array<char, 9> cells);
bool gameOver(std::array<char, 9> cells);
int minmaxingTTT(std::array<char, 9> cells, int depth, int alpha, int beta,
                 bool maximizingPlayer);
class AiPlayerStrategy {

  virtual void play(std::array<char, 9> &cells) = 0;

protected:
  char currentPlayer;
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
  void updateCurrentPlayer();
};

class AiPlayer {
  std::array<char, 9> cells;
  AiPlayerStrategy *strategy = new NoPlayStrategy();

public:
  AiPlayer(std::array<char, 9> arr);
  void setStrategy(AiPlayerStrategy *strategy);
};
class GameModel {
  std::array<char, 9> cells = {emptyCellMark, emptyCellMark, emptyCellMark,
                               emptyCellMark, emptyCellMark, emptyCellMark,
                               emptyCellMark, emptyCellMark, emptyCellMark};
  char currentPlayer = Xmark;
  AiPlayer aiPlayer;
  Mode appMode;
  std::array<int, 2> score;
  std::vector<int> orderOfMoves;

public:
  std::array<char, 9> getCellsState();
  void resetBoard();
  void resetScore();
  void modifyCells(int cellNumber);
  void setGameMode(Mode newMode);
  char checkWinner();
  std::array<int, 2> getScore();
  GameModel(AiPlayer aiPlayer);
  // want to avoid having gm.appMode == Mode::... in GameController
  bool is2player();
  bool isGame();
};
