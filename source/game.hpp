#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <map>
#include <string>
#include <thread>
#include <vector>
using namespace std;

enum class GamePhaseType {
  IDLE,
  GENERATING_PROBLEM,
  PLAYERS_MOVE,
  GAME_OVER,
  GAME_WON,
};

enum class PegColor {
  RED,
  BLUE,
  GREEN,
  WHITE,
  BLACK,
  YELLOW,
  NONE,
};

const std::map<PegColor, string> PegColors = {
    {PegColor::RED, "RED"},     {PegColor::BLUE, "BLUE"},
    {PegColor::WHITE, "WHITE"}, {PegColor::GREEN, "GREEN"},
    {PegColor::BLACK, "BLACK"}, {PegColor::YELLOW, "YELLOW"},
    {PegColor::NONE, "NONE"}};

enum class TipColor {
  WHITE,
  BLACK,
};

const std::map<TipColor, string> TipColors = {{TipColor::BLACK, "BLACK"},
                                              {TipColor::WHITE, "WHITE"}};

class GamePhaseInterface {
public:
  virtual void run() = 0;
  virtual GamePhaseType getPhaseType() = 0;
};

class GameInterface {
public:
  virtual void setNextPhase(unique_ptr<GamePhaseInterface>) = 0;
  virtual void runGame() = 0;
};

class GeneratingPhase : public GamePhaseInterface {
public:
  GeneratingPhase(GameInterface &game, size_t pegsNumber)
      : game(game), pegsNumber(pegsNumber) {}
  void run() {
    cout << "Generating secret peg sequence to be guessed by player" << endl;
    std::this_thread::sleep_for(1500ms);
    std::srand(std::time(0));

    for (size_t i = 0; i < pegsNumber; i++) {
      const auto peg = std::rand() % (PegColors.size() - 1);
      secretPegs.push_back(static_cast<PegColor>(peg));
    }

    // TODO delete
    for (auto p : secretPegs)
      cout << static_cast<int>(p) << " ";
    cout << endl;
    cout << "Secret sequence generated" << endl;
    game.setNextPhase(
        make_unique<GamePlayPhase>(GamePlayPhase(game, 3, secretPegs)));
  }
  GamePhaseType getPhaseType() { return GamePhaseType::GENERATING_PROBLEM; }

  class GameWonPhase : public GamePhaseInterface {
  public:
    GameWonPhase(GameInterface &game) : game(game) {}
    void run() {
      cout << "Game won!" << endl;
      std::this_thread::sleep_for(1500ms);
      cout << "Let's start again!" << endl;
      game.setNextPhase(make_unique<GeneratingPhase>(GeneratingPhase(game, 4)));
    }
    GameInterface &game;
    GamePhaseType getPhaseType() { return GamePhaseType::GAME_WON; }
  };

  class GameOverPhase : public GamePhaseInterface {
  public:
    GameOverPhase(GameInterface &game) : game(game) {}
    void run() {
      cout << "Game over!" << endl;
      std::this_thread::sleep_for(1500ms);
      cout << "Let's try again!" << endl;
      game.setNextPhase(make_unique<GeneratingPhase>(GeneratingPhase(game, 4)));
    }
    GameInterface &game;
    GamePhaseType getPhaseType() { return GamePhaseType::GAME_OVER; }
  };

  class GamePlayPhase : public GamePhaseInterface {
  public:
    GamePlayPhase(GameInterface &game, size_t movesNumber,
                  std::vector<PegColor> pegs)
        : game(game), moves(movesNumber), secretPegs(pegs) {}
    void run() {
      auto movesToGo = moves;
      while (movesToGo > 0) {
        vector<PegColor> playersGuess;
        cout << "Moves to go: " << movesToGo << endl;
        movesToGo--;
        cout << "Please provide 4 numbers for your guess: ";
        for (const auto color : PegColors)
          cout << static_cast<int>(color.first) << ":" << color.second << " ";
        cout << endl;
        int guess;
        for (size_t i = 0; i < 4; i++) {
          cin >> guess;
          playersGuess.push_back(static_cast<PegColor>(guess));
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
    GamePhaseType getPhaseType() { return GamePhaseType::PLAYERS_MOVE; }

  private:
    vector<TipColor> giveFeedback(vector<PegColor> playersGuess,
                                  vector<PegColor> secretPegs) {
      vector<TipColor> result;
      for (size_t i = 0; i < playersGuess.size(); i++) {
        if (secretPegs[i] == playersGuess[i]) {
          result.push_back(TipColor::BLACK);
          playersGuess[i] = PegColor::NONE;
          secretPegs[i] = PegColor::NONE;
        }
      }
      for (auto it = playersGuess.begin(); it != playersGuess.end();) {
        if (PegColor::NONE == *it) {
          it++;
          continue;
        }
        auto positionInSecretPegs =
            find(secretPegs.begin(), secretPegs.end(), *it);
        if (positionInSecretPegs != secretPegs.end()) {
          result.push_back(TipColor::WHITE);
          playersGuess.erase(it);
          secretPegs.erase(positionInSecretPegs);
        } else {
          it++;
        }
      }
      return result;
    }
    bool won(const vector<TipColor> &feedback) {
      if (feedback.size() != 4)
        return false;
      for (const auto &f : feedback) {
        if (TipColor::BLACK != f)
          return false;
      }
      return true;
    }
    GameInterface &game;
    const size_t moves;
    std::vector<PegColor> secretPegs;
  };

private:
  GameInterface &game;
  std::vector<PegColor> secretPegs;
  size_t pegsNumber;
};

class GameMastermind : public GameInterface {
public:
  void runGame() override {
    setNextPhase(make_unique<GeneratingPhase>(GeneratingPhase(*this, 4)));
    while (true)
      currentPhase->run();
  }

private:
  void setNextPhase(unique_ptr<GamePhaseInterface> phase) override {
    currentPhase = move(phase);
  }
  GamePhaseType getCurrentPhase() { return currentPhase->getPhaseType(); }
  unique_ptr<GamePhaseInterface> currentPhase;
};
