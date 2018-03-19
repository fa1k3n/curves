#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <gmock/gmock.h>

#include <QSignalSpy>
#include <QModelIndex>

#include "chaikins.h"

TEST(ChaikinsModel, basicContructor) {
    ChaikinsModel model;
    ASSERT_EQ(0, model.rowCount());
}

TEST(ChaikinsModel, possibleToAppendToModel) {
    ChaikinsModel model;
    QSignalSpy spy1(&model, SIGNAL(rowsAboutToBeInserted(const QModelIndex, int, int)));
    ASSERT_TRUE(spy1.isValid());
    QSignalSpy spy2(&model, SIGNAL(rowsInserted(const QModelIndex, int, int)));
    ASSERT_TRUE(spy2.isValid());

    int index = model.append(QPoint(1, 2));
    ASSERT_EQ(1, model.rowCount());
    ASSERT_EQ(1, spy1.count());   // Make sure that we are inserting the data correctly
    ASSERT_EQ(1, spy2.count());
    ASSERT_EQ(0, index);
}

TEST(ChaikinsModel, possibeToRemoveFromModel) {
    ChaikinsModel model;
    QSignalSpy spy1(&model, SIGNAL(rowsAboutToBeRemoved(const QModelIndex, int, int)));
    ASSERT_TRUE(spy1.isValid());
    QSignalSpy spy2(&model, SIGNAL(rowsRemoved(const QModelIndex, int, int)));
    ASSERT_TRUE(spy2.isValid());
    model.append(QPoint(1, 2));
    model.remove(QPoint(1, 2));
    ASSERT_EQ(0, model.rowCount());
    ASSERT_EQ(1, spy1.count());   // Make sure that we are inserting the data correctly
    ASSERT_EQ(1, spy2.count());
    ASSERT_EQ(0, spy2.takeFirst().at(1).toInt()); // verify the index
}

TEST(ChaikinsModel, notPossibleToRemoveNonExistingPoints) {
    ChaikinsModel model;
    QSignalSpy spy(&model, SIGNAL(rowsRemoved(const QModelIndex, int, int)));

    model.remove(QPoint(1, 2));
    ASSERT_EQ(0, spy.count());
}

TEST(ChaikinsModel, possibleToGetIndexedModelElement) {
    ChaikinsModel model;
    QPoint p = QPoint(0, 1);
    model.append(p);
    ASSERT_EQ(p, model.get(0));
}

TEST(ChaikinsModel, getElementChecksBounds) {
    ChaikinsModel model;
    auto p = model.get(-1);
    ASSERT_TRUE(p.isNull());

    p = model.get(10);
    ASSERT_TRUE(p.isNull());
}

TEST(ChaikinsModel, itIsPossibleToUpdatePosition) {
    ChaikinsModel model;
    QPoint p = QPoint(0, 1);
    model.append(p);

    QSignalSpy spy(&model, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex &, const QVector<int> &)));
    ASSERT_TRUE(spy.isValid());

    model.setData(model.index(0), QVariant(QPoint(2, 3)), ChaikinsModel::PositionRole);
    ASSERT_EQ(QPoint(2, 3), model.get(0).toPoint());
    ASSERT_EQ(1, spy.count());
    ASSERT_EQ(0, spy.takeFirst().at(0).toModelIndex().row());
}

TEST(ChaikinsModel, updatePositionWithSameDataWillDoNothing) {
    ChaikinsModel model;
    QPoint p = QPoint(0, 1);
    model.append(p);

    QSignalSpy spy(&model, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex &, const QVector<int> &)));
    ASSERT_TRUE(spy.isValid());

    model.setData(model.index(0), QVariant(QPoint(0, 1)), ChaikinsModel::PositionRole);
    ASSERT_EQ(0, spy.count());
}

TEST(ChaikinsModel, refineModelBasic) {
    QList<QPoint> out;
    ChaikinsModel model;
    model.append(QPoint(2, 2));
    model.append(QPoint(6, 6));
    ASSERT_TRUE(model.refine(out, 1));
    ASSERT_EQ(2, out.size());
    ASSERT_EQ(QPoint(3, 3), out[0]);
    ASSERT_EQ(QPoint(5, 5), out[1]);
}

TEST(ChaikinsModel, refineNegativeLevel) {
    QList<QPoint> out;
    ChaikinsModel model;
    model.append(QPoint(2, 2));
    model.append(QPoint(6, 6));
    ASSERT_FALSE(model.refine(out, -1));
}

TEST(ChaikinsModel, refineEmptyModel) {
    QList<QPoint> out;
    ChaikinsModel model;
    ASSERT_FALSE(model.refine(out, 1));
}

TEST(ChaikinsModel, itIsPossibleToSetPreviousPoint) {
    ChaikinsModel model;
    model.append(QPoint(0, 1));
    model.append(QPoint(1, 2));
    model.append(QPoint(5, 6));
    model.setData(model.index(2), QPoint(0, 1), ChaikinsModel::PrevRole);

    // Prev point should be updated
    ASSERT_EQ(QPoint(0, 1), model.data(model.index(2), ChaikinsModel::PrevRole));

    // Original prev point should point to null
    //ASSERT_TRUE(model.data(model.index(1), ChaikinsModel::PrevRole).isNull());
}

TEST(ChaikinsModel, itIsPossibleToSetNextPoint) {
    ChaikinsModel model;
    model.append(QPoint(0, 1));
    model.append(QPoint(1, 2));
    model.append(QPoint(5, 6));

    ASSERT_EQ(QPoint(1, 2), model.data(model.index(0), ChaikinsModel::NextRole));
    model.setData(model.index(0), QPoint(5, 6), ChaikinsModel::NextRole);

    ASSERT_EQ(QPoint(5, 6), model.data(model.index(0), ChaikinsModel::NextRole));

}

TEST(ChaikinsModel, pointsWillAutomaticallyBeConnected) {
    ChaikinsModel model;
    model.append(QPoint(2, 2));
    model.append(QPoint(6, 6));
    ASSERT_EQ(QPoint(2, 2), model.data(model.index(1), ChaikinsModel::PrevRole));
    ASSERT_EQ(QPoint(6, 6), model.data(model.index(0), ChaikinsModel::NextRole));
}

TEST(ChaikinsModel, removeItemWillConnectNeighbours) {
    ChaikinsModel model;
    model.append(QPoint(0, 1));
    model.append(QPoint(1, 2));
    model.append(QPoint(5, 6));
    model.remove(QPoint(1, 2));
    ASSERT_EQ(QPoint(5, 6), model.data(model.index(0), ChaikinsModel::NextRole));
    ASSERT_EQ(QPoint(0, 1), model.data(model.index(1), ChaikinsModel::PrevRole));
}

TEST(ChaikinsModel, removeWithConnection) {
    ChaikinsModel model;
    model.append(QPoint(0, 1));
    model.append(QPoint(1, 2));
    model.remove(QPoint(1, 2));
    ASSERT_TRUE(model.data(model.index(0), ChaikinsModel::NextRole).isNull());
    ASSERT_TRUE(model.data(model.index(0), ChaikinsModel::PrevRole).isNull());
}


