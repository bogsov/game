#include "game.h"
#include "gtest/gtest.h"
using namespace testing;

struct GameInterfaceFake : public GameInterface {
  void setNextPhase(unique_ptr<GamePhaseInterface>) override {}
  void runGame() override {}
};

class MastermindFeedbackTest : public ::testing::Test {
protected:
  void SetUp() override { GameInterfaceFake gameFake; }

  void TearDown() override {}

  GameInterfaceFake gameFake;
};

static vector<PegColor> getPegs(initializer_list<int> list) {
  vector<PegColor> result;
  for (const auto &l : list)
    result.push_back(static_cast<PegColor>(l));
  return result;
}

TEST_F(MastermindFeedbackTest, FeedbackTest) {
  vector<PegColor> pegs = getPegs({1, 2, 3, 4});
  GamePlayPhase gp(gameFake, 5, pegs);
  vector<FeedbackColor> feedback;
  {
    feedback = {FeedbackColor::BLACK, FeedbackColor::BLACK,
                FeedbackColor::BLACK, FeedbackColor::BLACK};
    EXPECT_EQ(feedback, gp.giveFeedback(getPegs({1, 2, 3, 4}), pegs));
  }
  {
    feedback = {FeedbackColor::WHITE, FeedbackColor::WHITE,
                FeedbackColor::WHITE, FeedbackColor::WHITE};
    EXPECT_EQ(feedback, gp.giveFeedback(getPegs({4, 3, 2, 1}), pegs));
  }
  {
    feedback = {FeedbackColor::BLACK, FeedbackColor::WHITE,
                FeedbackColor::WHITE, FeedbackColor::WHITE};
    EXPECT_EQ(feedback, gp.giveFeedback(getPegs({1, 3, 4, 2}), pegs));
  }
  {
    feedback = {FeedbackColor::BLACK, FeedbackColor::BLACK};
    EXPECT_EQ(feedback, gp.giveFeedback(getPegs({1, 2, 5, 5}), pegs));
  }
  {
    feedback = {FeedbackColor::WHITE, FeedbackColor::WHITE};
    EXPECT_EQ(feedback, gp.giveFeedback(getPegs({5, 1, 2, 1}), pegs));
  }
  {
    feedback = {FeedbackColor::BLACK};
    EXPECT_EQ(feedback, gp.giveFeedback(getPegs({1, 1, 1, 1}), pegs));
  }
  {
    feedback = {FeedbackColor::WHITE, FeedbackColor::WHITE};
    EXPECT_EQ(feedback, gp.giveFeedback(getPegs({2, 1, 1, 1}), pegs));
  }
  {
    feedback = {FeedbackColor::BLACK, FeedbackColor::BLACK};
    EXPECT_EQ(feedback, gp.giveFeedback(getPegs({1, 2}), pegs));
  }
  {
    feedback = {};
    EXPECT_EQ(feedback, gp.giveFeedback(getPegs({}), pegs));
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}