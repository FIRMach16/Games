
#include "GameModel.hpp"
#include <algorithm>
#include <array>
#include <random>
#include <vector>

GameModel::GameModel(AiPlayer aiPlayer) : aiPlayer(aiPlayer) {}
std::array<char, 9> GameModel::getCellsState() { return this->cells; }
void GameModel::modifyCells(int cellNumber) {
  this->cells[cellNumber] = this->currentPlayer;
};
void AiPlayer::setStrategy(AiPlayerStrategy *strategy) {
  this->strategy = strategy;
}
AiPlayer::AiPlayer(std::array<char, 9> arr) { this->cells = arr; }
// void GameModel::notify() {
//   for (GameStateObserver observer : this->observers) {
//     observer.update();
//   }
// };
// void GameModel::add(GameStateObserver observer) {
//   this->observers.push_back(observer);
// }

void BeginnerAiStrategy::play(std::array<char, 9> &cells) {
  if (currentPlayer == Omark) {
    std::vector<int> emptyCells = findEmptyCells(cells);
    if (emptyCells.empty())
      return;
    if (findWinningMove(cells, Omark) != -1) {
      cells[findWinningMove(cells, Omark)] = Omark;
    } else {
      randomMove(cells, emptyCells);
    }
    currentPlayer = Xmark;
  }
};
void IntermediateAiStrategy::play(std::array<char, 9> &cells) {
  // random or one move win or block adversary from wining
  if (currentPlayer == Omark) {
    std::vector<int> emptyCells = findEmptyCells(cells);
    if (emptyCells.empty()) {
      currentPlayer = emptyCellMark;
      return; // end game
    }
    if (findWinningMove(cells, Omark) != -1) {
      cells[findWinningMove(cells, Omark)] = Omark;
    } else if (findWinningMove(cells, Xmark) != -1) {
      cells[findWinningMove(cells, Xmark)] = Omark;
    } else {
      randomMove(cells, emptyCells);
    }
    currentPlayer = Xmark;
  }
};
void ExpertAiStrategy::play(std::array<char, 9> &cells) {
  if (currentPlayer == 'O') {

    std::vector<int> emptyCells = findEmptyCells(cells);
    if (emptyCells.empty()) {
      currentPlayer = emptyCellMark;
      return; // end game
    }

    std::array<char, 9> cells_cpy = cells;
    int bestEval = INT32_MIN, score, move;
    for (int idx : emptyCells) {
      cells_cpy[idx] = Omark;
      score = minmaxingTTT(cells_cpy, emptyCells.size(), INT32_MIN, INT32_MAX,
                           false);
      if (score > bestEval) {
        bestEval = score;
        move = idx;
      }
      cells_cpy[idx] = emptyCellMark;
    }
    currentPlayer = Xmark;
  }
};
int evaluateBoard(std::array<char, 9> cells) {
  if (checkForWinner(cells) == emptyCellMark)
    return 0;
  else if (checkForWinner(cells) == Omark)
    return 1;
  else if (checkForWinner(cells) == Xmark)
    return -1;
  return 0;
}
bool gameOver(std::array<char, 9> cells) {
  if ((checkForWinner(cells) != emptyCellMark) ||
      findEmptyCells(cells).empty()) {
    return true;
  }
  return false;
}
int minmaxingTTT(std::array<char, 9> cells, int depth, int alpha, int beta,
                 bool maximizingPlayer) {
  if ((depth == 0) || gameOver(cells)) {
    return evaluateBoard(cells);
  }
  if (maximizingPlayer) {
    int bestEval = INT32_MIN;
    for (int idx : findEmptyCells(cells)) {
      cells[idx] = Omark;
      int eval = minmaxingTTT(cells, depth - 1, alpha, beta, false);
      if (eval > bestEval) {
        bestEval = eval;
      }
      cells[idx] = emptyCellMark;
    }
    return bestEval;
  } else {
    int worstEval = INT32_MAX;
    for (int idx : findEmptyCells(cells)) {
      cells[idx] = Xmark;
      int eval = minmaxingTTT(cells, depth - 1, alpha, beta, true);
      if (eval < worstEval) {
        worstEval = eval;
      }
      cells[idx] = emptyCellMark;
    }
    return worstEval;
  }
}

char checkForWinner(std::array<char, 9> cells) {
  for (int i = 0; i < winConditions.size(); i++) {
    if (cells[winConditions[i].a] == Xmark &&
        cells[winConditions[i].b] == Xmark &&
        cells[winConditions[i].c] == Xmark) {
      {
        return Xmark;
      }
    } else if (cells[winConditions[i].a] == Omark &&
               cells[winConditions[i].b] == Omark &&
               cells[winConditions[i].c] == Omark) {
      {
        return Omark;
      }
    }
  }
  return emptyCellMark;
}
int findWinningMove(std::array<char, 9> cells, char mark) {
  for (int i = 0; i < winConditions.size(); i++) {
    if (cells[winConditions[i].a] == mark &&
        cells[winConditions[i].b] == mark &&
        cells[winConditions[i].c] == emptyCellMark)
      return winConditions[i].c;
    else if (cells[winConditions[i].a] == mark &&
             cells[winConditions[i].b] == mark &&
             cells[winConditions[i].c] == emptyCellMark)
      return winConditions[i].b;
    else if (cells[winConditions[i].a] == emptyCellMark &&
             cells[winConditions[i].b] == mark &&
             cells[winConditions[i].c] == mark)
      return winConditions[i].a;
  }
  return -1;
}
void randomMove(std::array<char, 9> cells, std::vector<int> &emptyCells) {
  std::mt19937 rng(std::random_device{}());
  std::shuffle(emptyCells.begin(), emptyCells.end(), rng);
  cells[emptyCells[0]] = Omark;
}
std::vector<int> findEmptyCells(std::array<char, 9> cells) {
  std::vector<int> emptyCells;
  for (int i = 0; i < 9; i++) {
    if (cells[i] == '-') {
      emptyCells.push_back(i);
    }
  }
  return emptyCells;
}
