#ifndef LINE_H
#define LINE_H

#include <QQuickItem>
#include <chaikins.h>

class Line : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(QPoint start READ start WRITE setStart NOTIFY lineChanged)
    Q_PROPERTY(QPoint end READ end WRITE setEnd NOTIFY lineChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    Line(QQuickItem *parent = 0);
    Line(QPoint& start, QPoint &end, QQuickItem *parent = 0);
    ~Line();

    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *);

    QPoint& start() { return m_start; }
    QPoint& end() { return m_end; }
    QColor color() const { return m_color; }

    void setStart(QPoint& p);
    void setEnd(QPoint& p);
    void setColor(const QColor &c);

signals:
    void lineChanged();
    void colorChanged(const QColor &c);

public slots:

private:
    QPoint m_start;
    QPoint m_end;
    QColor  m_color;
};

#endif // LINE_H
