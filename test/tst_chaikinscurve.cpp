#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <gmock/gmock.h>

#include <QSGNode>
#include <QSignalSpy>

#include "chaikins.h"

using ::testing::_;
using ::testing::Return;

class ChaikinsCurveMock : public ChaikinsCurve {
public:
    MOCK_METHOD2(updatePaintNode, QSGNode*(QSGNode*, UpdatePaintNodeData*));
};

TEST(ChaikinsCurve, basicConstructor) {
    ChaikinsCurve curve;
    ASSERT_TRUE(true);
}

TEST(ChaikinsCurve, updateRefinement) {
    ChaikinsCurve curve;
    QSignalSpy spy(&curve, SIGNAL(refinementChanged(int)));
    ASSERT_TRUE(spy.isValid());

    curve.setRefinement(3);
    EXPECT_EQ(3, curve.refinement());
    EXPECT_EQ(1, spy.count());
    ASSERT_EQ(3, spy.takeFirst().at(0).toInt());
}

TEST(ChaikinsCurve, updateRefinementWontDoAnythingIfSameValue) {
    ChaikinsCurveMock curve;
    QSignalSpy spy(&curve, SIGNAL(refinementChanged(int)));
    ASSERT_TRUE(spy.isValid());

    curve.setRefinement(3);
    curve.setRefinement(3);
    EXPECT_EQ(1, spy.count());
}
