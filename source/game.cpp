#include "game.h"
#include "interfaces.hpp"
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <thread>
#ifdef CHEAT_MODE
constexpr bool cheating = true;
#else
constexpr bool cheating = false;
#endif
using namespace std;

GameOverPhase::GameOverPhase(GameInterface &game) : game(game) {}
void GameOverPhase::run() {
  cout << "Game over!" << endl;
  std::this_thread::sleep_for(1500ms);
  cout << "Let's try again!" << endl;
  game.setNextPhase(make_unique<GeneratingPhase>(GeneratingPhase(game)));
}

GamePhaseType GameOverPhase::getPhaseType() { return GamePhaseType::GAME_OVER; }

GameWonPhase::GameWonPhase(GameInterface &game) : game(game) {}
void GameWonPhase::run() {
  cout << "Game won!" << endl;
  std::this_thread::sleep_for(1500ms);
  cout << "Let's start again!" << endl;
  game.setNextPhase(make_unique<GeneratingPhase>(GeneratingPhase(game)));
}

GamePhaseType GameWonPhase::getPhaseType() { return GamePhaseType::GAME_WON; }

GamePlayPhase::GamePlayPhase(GameInterface &game, size_t movesNumber,
                             std::vector<PegColor> pegs)
    : game(game), moves(movesNumber), secretPegs(pegs) {}

void GamePlayPhase::run() {
  auto movesToGo = moves;

  while (movesToGo > 0) {
    vector<PegColor> playersGuess;
    cout << "Moves to go: " << movesToGo << endl;
    movesToGo--;
    cout << "Please provide 4 numbers for your guess: ";

    for (const auto color : PegColors)
      cout << static_cast<int>(color.first) << ":" << color.second << " ";

    cout << endl;
    string guess;
    int guessInteger;
    for (size_t i = 0; i < 4; i++) {
      while (true) {
        cin >> guess;
        try {
          guessInteger = stoi(guess);
          if (isValidPegColor(guessInteger))
            break;
        } catch (...) {
        }
        cout << "Please provide valid number: " << endl;
      }
      playersGuess.push_back(static_cast<PegColor>(guessInteger));
    }
    auto feedback = giveFeedback(playersGuess, secretPegs);

    for (const auto &f : feedback) {
      cout << TipColors.at(f) << " ";
    }

    cout << endl;

    if (won(feedback)) {
      game.setNextPhase(make_unique<GameWonPhase>(GameWonPhase(game)));
      return;
    }
  }
  game.setNextPhase(make_unique<GameOverPhase>(GameOverPhase(game)));
}

bool GamePlayPhase::isValidPegColor(int guess) {
  auto peg = static_cast<PegColor>(guess);
  return (peg >= PegColor::RED) and (peg <= PegColor::YELLOW);
}

GamePhaseType GamePlayPhase::getPhaseType() {
  return GamePhaseType::PLAYERS_MOVE;
}

vector<FeedbackColor> GamePlayPhase::giveFeedback(vector<PegColor> playersGuess,
                                                  vector<PegColor> secretPegs) {

  // First find right color and right place
  vector<FeedbackColor> result;
  for (size_t i = 0; i < playersGuess.size(); i++) {
    if (secretPegs[i] == playersGuess[i]) {
      result.push_back(FeedbackColor::BLACK);
    }
  }

  const auto &rightColorAndPlace = result.size();

  // Find all guessed colors
  sort(playersGuess.begin(), playersGuess.end());
  sort(secretPegs.begin(), secretPegs.end());
  vector<PegColor> intersection;
  set_intersection(playersGuess.begin(), playersGuess.end(), secretPegs.begin(),
                   secretPegs.end(), back_inserter(intersection));
  auto rightColorButWrongPlace = intersection.size() - rightColorAndPlace;

  // Append feedback with all the guessed colors minus the previously
  // counted ones
  for (size_t i = 0; i < rightColorButWrongPlace; i++)
    result.push_back(FeedbackColor::WHITE);

  return result;
}

bool GamePlayPhase::won(const vector<FeedbackColor> &feedback) {
  if (feedback.size() != PROBLEM_SIZE)
    return false;
  for (const auto &fb : feedback) {
    if (FeedbackColor::BLACK != fb)
      return false;
  }
  return true;
}

GeneratingPhase::GeneratingPhase(GameInterface &game) : game(game) {}
void GeneratingPhase::run() {
  cout << "New game starting" << endl;
  std::this_thread::sleep_for(1500ms);
  cout << "Generating secret peg sequence to be guessed by player" << endl;
  std::this_thread::sleep_for(1500ms);
  std::srand(std::time(0));

  for (size_t i = 0; i < PROBLEM_SIZE; i++) {
    const auto peg = std::rand() % (PegColors.size());
    secretPegs.push_back(static_cast<PegColor>(peg));
  }

  if (cheating) {
    for (auto p : secretPegs)
      cout << static_cast<int>(p) << " ";

    cout << endl;
  }

  cout << "Secret sequence generated" << endl;
  game.setNextPhase(make_unique<GamePlayPhase>(
      GamePlayPhase(game, PLAYERS_MOVES, secretPegs)));
}

GamePhaseType GeneratingPhase::getPhaseType() {
  return GamePhaseType::GENERATING_PROBLEM;
}

void GameMastermind::runGame() {
  setNextPhase(make_unique<GeneratingPhase>(GeneratingPhase(*this)));
  auto gamesWonSofar = 0;
  auto allGames = 0;
  
  while (true) {
    if (GamePhaseType::GENERATING_PROBLEM == getCurrentPhaseType())
      allGames++;

    currentPhase->run();

    if (GamePhaseType::GAME_WON == getCurrentPhaseType()) {
      gamesWonSofar++;
      cout << "Already won " << gamesWonSofar << " out of " << allGames
           << " games" << endl;
    }
  }
}

void GameMastermind::setNextPhase(unique_ptr<GamePhaseInterface> phase) {
  currentPhase = move(phase);
}

GamePhaseType GameMastermind::getCurrentPhaseType() {
  return currentPhase->getPhaseType();
}
