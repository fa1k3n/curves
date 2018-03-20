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
    MOCK_METHOD0(update, void());
};

TEST(ChaikinsCurve, basicConstructor) {
    ChaikinsCurve curve;
    ASSERT_TRUE(true);
}

TEST(ChaikinsCurve, updateRefinement) {
    ChaikinsCurveMock curve;
    EXPECT_CALL(curve, update());
    QSignalSpy spy(&curve, SIGNAL(refinementChanged(int)));
    ASSERT_TRUE(spy.isValid());

    curve.setRefinement(3);
    EXPECT_EQ(3, curve.refinement());
    EXPECT_EQ(1, spy.count());
    ASSERT_EQ(3, spy.takeFirst().at(0).toInt());
}

TEST(ChaikinsCurve, updateRefinementWontDoAnythingIfSameValue) {
    ChaikinsCurve curve;
    QSignalSpy spy(&curve, SIGNAL(refinementChanged(int)));
    ASSERT_TRUE(spy.isValid());

    curve.setRefinement(3);
    curve.setRefinement(3);
    EXPECT_EQ(1, spy.count());

}

TEST(ChaikinsModel, addModelBasic) {
    ChaikinsModel model;
    ChaikinsCurveMock curve;

    EXPECT_CALL(curve, update()).Times(2);
    curve.setModel(&model);
    model.append(QPoint(1,2));
    EXPECT_TRUE(true);
}
