#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <chaikins.h>

using namespace testing;

TEST(chaikins_model, curves)
{
    ChaikinsModel model;
    EXPECT_EQ(0, model.rowCount());
}
