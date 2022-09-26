add_test([=[MastermindFeedbackTest.FeedbackTest]=]  /home/bogso/repos/Game/build/mastermindTest [==[--gtest_filter=MastermindFeedbackTest.FeedbackTest]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[MastermindFeedbackTest.FeedbackTest]=]  PROPERTIES WORKING_DIRECTORY /home/bogso/repos/Game/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set(  mastermindTest_TESTS MastermindFeedbackTest.FeedbackTest)
