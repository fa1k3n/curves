#ifndef CHAIKINS_H
#define CHAIKINS_H

#include <QQuickItem>
#include <QAbstractListModel>
#include <iostream>

class ControlPoint : public QPoint {
public:
    ControlPoint() : QPoint() {}
    ControlPoint(int xpos, int ypos, ControlPoint* prev = nullptr, ControlPoint* next = nullptr) : QPoint(xpos, ypos), m_prev(prev), m_next(next) {}

    ControlPoint* prev(void) const { return m_prev; }
    ControlPoint* next(void) const { return m_next; }
    void setNext(ControlPoint* p) { m_next = p; }
    void setPrev(ControlPoint* p) { m_prev = p; }

    bool operator==(ControlPoint& rhs) const {
        return x() == rhs.x() && y() == rhs.y();
    }

    friend std::ostream& operator<<(std::ostream &strm, ControlPoint cp) {
        return strm << "(" << cp.x() << ", " << cp.y() << ")";
    }

private:
    ControlPoint *m_prev, *m_next;
};

Q_DECLARE_METATYPE(QPoint*)

class ChaikinsModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum ChaikinsModelRoles {
        PositionRole = Qt::UserRole + 1,
        PrevRole = Qt::UserRole + 2,
        NextRole = Qt::UserRole + 3
    };

    ChaikinsModel(QObject* parent = 0);

    Q_INVOKABLE int append(QPoint controlPoint);
    Q_INVOKABLE void remove(QPoint controlPoint);
    Q_INVOKABLE QVariant get(int index);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    bool refine(QList<QPoint>& out, int level);

private:
    bool refineRec(QList<QPoint>& out, int level);
    int find(QPoint p, ControlPoint** cp = nullptr);
    QList<ControlPoint> m_controlPoints;
};

class Chaikins : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(int refinement READ refinement WRITE setRefinement NOTIFY refinementChanged)
    Q_PROPERTY(ChaikinsModel* model READ model WRITE setModel NOTIFY modelChanged)
public:
    Chaikins(QQuickItem *parent = 0);
    ~Chaikins();

    int refinement() { return m_refinment; }
    void setRefinement(int newRefinement);

    ChaikinsModel* model() { return m_model; };
    void setModel(ChaikinsModel* model);

    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *);

signals:
    void refinementChanged(int newRefinement);
    void modelChanged(ChaikinsModel* model);

private:
    ChaikinsModel* m_model;
    int m_refinment;
};

#endif // CHAIKINS_H
