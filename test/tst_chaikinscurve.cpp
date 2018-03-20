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

class ChaikinsModelMock : public ChaikinsModel {
public:
    void rowsInserted(const QModelIndex &parent, int first, int last) {
        beginInsertRows(parent, first, last);
        endInsertRows();
    }

    void rowsRemoved (const QModelIndex &parent, int first, int last) {
        beginRemoveRows(parent, first, last);
        endRemoveRows();
    }
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

TEST(ChaikinsCurve, addModelBasic) {
    ChaikinsModelMock model;
    ChaikinsCurveMock curve;

    EXPECT_CALL(curve, update()).Times(4);
    curve.setModel(&model);
    emit model.rowsInserted(model.index(0), 0, 0);
    emit model.rowsRemoved(model.index(0), 0, 0);
    emit model.dataChanged(model.index(0), model.index(0));
}


TEST(ChaikinsCurve, setSameModelWontUpdateAnything) {
    ChaikinsModelMock model;
    ChaikinsCurveMock curve;

    EXPECT_CALL(curve, update()).Times(1);
    curve.setModel(&model);
    curve.setModel(&model);
}
