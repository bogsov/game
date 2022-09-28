#pragma once
#include <map>
#include <memory>
#include <string>
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
};

const std::map<PegColor, string> PegColors = {
    {PegColor::RED, "RED"},     {PegColor::BLUE, "BLUE"},
    {PegColor::WHITE, "WHITE"}, {PegColor::GREEN, "GREEN"},
    {PegColor::BLACK, "BLACK"}, {PegColor::YELLOW, "YELLOW"}};

enum class FeedbackColor {
  WHITE,
  BLACK,
};

const std::map<FeedbackColor, string> FeedbackColors = {
    {FeedbackColor::BLACK, "BLACK"}, {FeedbackColor::WHITE, "WHITE"}};

class GamePhaseInterface {
public:
  virtual void run() = 0;
  virtual GamePhaseType getPhaseType() = 0;
  virtual ~GamePhaseInterface(){};
};

class GameInterface {
public:
  virtual void setNextPhase(unique_ptr<GamePhaseInterface>) = 0;
  virtual void runGame() = 0;
  virtual ~GameInterface(){};
};