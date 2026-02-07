
#include "GameModel.hpp"
#include <algorithm>
#include <array>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

GameModel::GameModel()
    : cells(std::make_shared<std::array<char, 9>>(initCells)),
      aiPlayer(AiPlayer(cells)) {}
void GameModel::addObserver(GameStateObserver *obs) {
  observers.push_back(obs);
}
void GameModel::removeObserver(GameStateObserver *obs) {
  observers.erase(remove(observers.begin(), observers.end(), obs),
                  observers.end());
}

void GameModel::notifyObservers() {
  for (GameStateObserver *observer : observers) {
    observer->update();
  }
}
void GameModel::setPlayStrategy(std::unique_ptr<AiPlayerStrategy> strategy) {
  aiPlayer.setStrategy(std::move(strategy));
}
std::array<char, 9> GameModel::getCellsState() { return (*cells); }
void GameModel::modifyCells(int cellNumber) {
  if (is2player()) {
    (*cells)[cellNumber] = currentPlayer;
    if (currentPlayer == Xmark) {
      currentPlayer = Omark;
    } else {
      currentPlayer = Xmark;
    }
    if (appMode == Mode::INFINITE_TTT) {
      orderOfMoves.push_back(cellNumber);
      if (orderOfMoves.size() == 7) {
        (*cells)[orderOfMoves[0]] = emptyCellMark;
        orderOfMoves.erase(orderOfMoves.begin());
      }
    }
  } else {
    if (currentPlayer == computerPlayer) {
      aiPlayer.play();
      if (computerPlayer == Xmark) {
        currentPlayer = Omark;
      } else {
        currentPlayer = Xmark;
      }
    } else if (currentPlayer != emptyCellMark) {

      if (cellNumber != -1) {

        (*cells)[cellNumber] = currentPlayer;
        currentPlayer = computerPlayer;
      }
    }
  }
  notifyObservers();
};
void GameModel::resetBoard() {
  rounds++;
  if (rounds % 2 == 0) {
    currentPlayer = Xmark;
  } else {
    currentPlayer = Omark;
  }
  orderOfMoves.clear();
  (*cells).fill(emptyCellMark);
  notifyObservers();
}

char GameModel::checkWinner() {
  auto tmp = checkForWinner((*cells));
  if (tmp == emptyCellMark) {
    return emptyCellMark;
  } else if (tmp == Xmark && currentPlayer != emptyCellMark) {
    score[0]++;
    currentPlayer = emptyCellMark;
    return Xmark;
  } else if (tmp == Omark && currentPlayer != emptyCellMark) {
    score[1]++;
    currentPlayer = emptyCellMark;
    return Omark;
  }
  return emptyCellMark;
  notifyObservers();
}
void GameModel::setGameMode(Mode newMode) { appMode = newMode; }
void GameModel::resetScore() {
  rounds = 0;
  cells->fill(emptyCellMark);
  score.fill(0);
  currentPlayer = Xmark;
  notifyObservers();
}
std::array<int, 2> GameModel::getScore() { return score; }
bool GameModel::isGame() { return appMode != Mode::MENU; }
bool GameModel::is2player() { return appMode != Mode::VS_COMPUTER; }
bool GameModel::computerTurn() {
  return (currentPlayer == computerPlayer && !is2player() && isGame());
}
void AiPlayer::setStrategy(std::unique_ptr<AiPlayerStrategy> strategy) {
  this->strategy = std::move(strategy);
}
AiPlayer::AiPlayer(std::shared_ptr<std::array<char, 9>> arr) : cells(arr) {}
void AiPlayer::play() { strategy->play((*cells)); }
void BeginnerAiStrategy::play(std::array<char, 9> &cells) {

  std::vector<int> emptyCells = findEmptyCells(cells);
  int move;
  if (emptyCells.empty())
    return;
  if (findWinningMove(cells, Omark) != -1) {
    move = findWinningMove(cells, Omark);
  } else {
    move = randomMove(cells, emptyCells);
  }
  cells[move] = Omark;
};
void IntermediateAiStrategy::play(std::array<char, 9> &cells) {
  // random or one move win or block adversary from wining
  std::vector<int> emptyCells = findEmptyCells(cells);
  if (emptyCells.empty()) {
    return; // end game
  }
  int move;
  if (findWinningMove(cells, Omark) != -1) {
    move = findWinningMove(cells, Omark);
  } else if (findWinningMove(cells, Xmark) != -1) {
    move = findWinningMove(cells, Xmark);
  } else {
    move = randomMove(cells, emptyCells);
  }
  cells[move] = Omark;
};
void ExpertAiStrategy::play(std::array<char, 9> &cells) {
  std::vector<int> emptyCells = findEmptyCells(cells);
  if (emptyCells.empty()) {
    return; // end game
  }

  std::array<char, 9> cells_cpy = cells;
  int bestEval = INT32_MIN, score, move;
  for (int idx : emptyCells) {
    cells_cpy[idx] = Omark;
    score =
        minmaxingTTT(cells_cpy, emptyCells.size(), INT32_MIN, INT32_MAX, false);
    if (score > bestEval) {
      bestEval = score;
      move = idx;
    }
    cells_cpy[idx] = emptyCellMark;
  }
  cells[move] = Omark;
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
             cells[winConditions[i].b] == emptyCellMark &&
             cells[winConditions[i].c] == mark)
      return winConditions[i].b;
    else if (cells[winConditions[i].a] == emptyCellMark &&
             cells[winConditions[i].b] == mark &&
             cells[winConditions[i].c] == mark)
      return winConditions[i].a;
  }
  return -1;
}
int randomMove(std::array<char, 9> &cells, std::vector<int> &emptyCells) {
  std::mt19937 rng(std::random_device{}());
  std::shuffle(emptyCells.begin(), emptyCells.end(), rng);
  return emptyCells[0];
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
