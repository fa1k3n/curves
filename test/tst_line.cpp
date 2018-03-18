#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <QSignalSpy>
#include <line.h>

using namespace testing;

TEST(line, basicConstructor) {
    Line line;

    EXPECT_EQ(QColor("red"), line.color());
    EXPECT_EQ(QPoint(0, 0), line.start());
    EXPECT_EQ(QPoint(0, 0), line.end());
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


