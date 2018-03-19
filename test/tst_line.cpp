#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <QSGGeometryNode>
#include <QSGGeometry>
#include <QSignalSpy>
#include <QSGFlatColorMaterial>
#include <line.h>

using namespace testing;

TEST(line, basicConstructor) {
    Line line;
    EXPECT_EQ(QColor("red"), line.color());
    EXPECT_EQ(QPoint(0, 0), line.start());
    EXPECT_EQ(QPoint(0, 0), line.end());
}

TEST(line, basicConstructorWithArgs) {
    QPoint p1(1, 2), p2(4, 5);
    Line line(p1, p2);
    EXPECT_EQ(p1, line.start());
    EXPECT_EQ(p2, line.end());
}

TEST(line, setStartPosition) {
    Line line;
    QSignalSpy spy(&line, SIGNAL(lineChanged()));
    QPoint p(1, 2);
    line.setStart(p);
    EXPECT_EQ(1, spy.count());
    EXPECT_EQ(p, line.start());
}

TEST(line, setStartToSameValueDoesNotEmitSignal) {
    Line line;
    QSignalSpy spy(&line, SIGNAL(lineChanged()));
    QPoint p(1, 2);
    line.setStart(p);
    line.setStart(p);
    EXPECT_EQ(1, spy.count());
}

TEST(line, setEndPosition) {
    Line line;
    QSignalSpy spy(&line, SIGNAL(lineChanged()));
    QPoint p(1, 2);
    line.setEnd(p);
    EXPECT_EQ(1, spy.count());
    EXPECT_EQ(p, line.end());
}

TEST(line, setEndToSameValueDoesNotEmitSignal) {
    Line line;
    QSignalSpy spy(&line, SIGNAL(lineChanged()));
    QPoint p(1, 2);
    line.setEnd(p);
    line.setEnd(p);
    EXPECT_EQ(1, spy.count());
}

TEST(line, setColor) {
    Line line;
    QSignalSpy spy(&line, SIGNAL(colorChanged(const QColor)));
    line.setColor("blue");
    EXPECT_EQ(1, spy.count());
    EXPECT_EQ(QColor("blue"), line.color());
}

TEST(line, setColorToSameValueDoesNotEmitSignal) {
    Line line;
    QSignalSpy spy(&line, SIGNAL(colorChanged(const QColor)));
    line.setColor("blue");
    line.setColor("blue");
    EXPECT_EQ(1, spy.count());
}

TEST(line, basicUpdatePaintNode) {
    Line line;
    QSGGeometryNode* node = static_cast<QSGGeometryNode*>(line.updatePaintNode(nullptr, nullptr));
    QSGFlatColorMaterial* mat = dynamic_cast<QSGFlatColorMaterial*>(node->material());
    QSGGeometry* geom = node->geometry();
    EXPECT_TRUE(node->flags() & (QSGNode::OwnsMaterial | QSGNode::OwnsGeometry));
    EXPECT_EQ(line.width(), geom->lineWidth());
    EXPECT_EQ(line.color(), mat->color());

    QSGGeometry::Point2D *vertices = geom->vertexDataAsPoint2D();
    EXPECT_EQ(line.start(), QPoint(vertices[0].x, vertices[0].y));
    EXPECT_EQ(line.end(), QPoint(vertices[1].x, vertices[1].y));

}



