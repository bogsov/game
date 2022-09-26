#include "interfaces.hpp"
#include <gtest/gtest_prod.h>
#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;

constexpr size_t PROBLEM_SIZE = 4;
constexpr size_t PLAYERS_MOVES = 8;

class GameOverPhase : public GamePhaseInterface {
public:
  GameOverPhase(GameInterface &game);
  void run() override;
  GameInterface &game;
  GamePhaseType getPhaseType() override;
};

class GameWonPhase : public GamePhaseInterface {
public:
  GameWonPhase(GameInterface &game);
  void run() override;
  GameInterface &game;
  GamePhaseType getPhaseType() override;
};

class GamePlayPhase : public GamePhaseInterface {
public:
  GamePlayPhase(GameInterface &game, size_t movesNumber,
                std::vector<PegColor> pegs);
  void run() override;
  GamePhaseType getPhaseType() override;

private:
  FRIEND_TEST(MastermindFeedbackTest, FeedbackTest);
  bool isValidPegColor(int guess);
  vector<FeedbackColor> giveFeedback(vector<PegColor> playersGuess,
                                     vector<PegColor> secretPegs);
  bool won(const vector<FeedbackColor> &feedback);
  GameInterface &game;
  const size_t moves;
  std::vector<PegColor> secretPegs;
};

class GeneratingPhase : public GamePhaseInterface {
public:
  GeneratingPhase(GameInterface &game);
  void run() override;
  GamePhaseType getPhaseType() override;

private:
  GameInterface &game;
  std::vector<PegColor> secretPegs;
};

class GameMastermind : public GameInterface {
public:
  void runGame() override;
  void setNextPhase(unique_ptr<GamePhaseInterface> phase) override;

private:
  GamePhaseType getCurrentPhaseType();
  unique_ptr<GamePhaseInterface> currentPhase;
};
