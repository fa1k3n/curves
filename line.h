#ifndef LINE_H
#define LINE_H

#include <QQuickItem>

class Line : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(QQuickItem* start READ start WRITE setStart NOTIFY startChanged)
    Q_PROPERTY(QQuickItem* end READ end WRITE setEnd NOTIFY endChanged)
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    Line(QQuickItem *parent = 0);
    ~Line();

    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *);

    QQuickItem* start() const { return m_start; }
    QQuickItem* end() const { return m_end; }
    int width() const { return m_width; }
    QColor color() const { return m_color; }

    void setStart(QQuickItem *p);
    void setEnd(QQuickItem *p);
    void setWidth(int w);
    void setColor(const QColor &c);

signals:
    void startChanged(QQuickItem *p);
    void endChanged(QQuickItem *p);
    void widthChanged(int w);
    void colorChanged(const QColor &c);

public slots:

private:
    QQuickItem* m_start;
    QQuickItem* m_end;
    int     m_width;
    QColor  m_color;
};

#endif // LINE_H
