#ifndef LINE_H
#define LINE_H

#include <QQuickItem>
#include <chaikins.h>

class Line : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(QPoint start READ start WRITE setStart NOTIFY startChanged)
    Q_PROPERTY(QPoint end READ end WRITE setEnd NOTIFY endChanged)
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    Line(QQuickItem *parent = 0);
    ~Line();

    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *);

    QPoint& start() { return m_start; }
    QPoint& end() { return m_end; }
    int width() const { return m_width; }
    QColor color() const { return m_color; }

    void setStart(QPoint& p);
    void setEnd(QPoint& p);
    void setWidth(int w);
    void setColor(const QColor &c);

signals:
    void startChanged(QPoint p);
    void endChanged(QPoint p);
    void widthChanged(int w);
    void colorChanged(const QColor &c);

public slots:

private:
    QPoint m_start;
    QPoint m_end;
    int     m_width;
    QColor  m_color;
};

#endif // LINE_H
