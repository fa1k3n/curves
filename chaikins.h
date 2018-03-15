#ifndef CHAIKINS_H
#define CHAIKINS_H

#include <QQuickItem>
#include <QAbstractItemModel>

class Chaikins : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(int refinement READ refinement WRITE setRefinement NOTIFY refinementChanged)
    Q_PROPERTY(QAbstractItemModel* controlPolygon READ controlPolygon WRITE setControlPolygon NOTIFY controlPolygonChanged)
public:
    Chaikins(QQuickItem *parent = 0);
    ~Chaikins();

    int refinement() { return m_refinment; }
    void setRefinement(int newRefinement);

    QAbstractItemModel* controlPolygon() { return m_controlPolygon; }
    void setControlPolygon(QAbstractItemModel* newPolygon);

    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *);

signals:
    void refinementChanged(int newRefinement);
    void controlPolygonChanged();

private:
    std::vector<QPoint>* refine(std::vector<QPoint>&);
    std::vector<QPoint> m_controlPoints;
    QAbstractItemModel* m_controlPolygon;
    int m_refinment;
};

#endif // CHAIKINS_H
