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

    bool connect(ControlPoint* c) {
        m_prev = c;
        c->m_next = this;
        return true;
    }

    bool disconnect(ControlPoint* c) {
        if(c == nullptr)
            return false;

        if(m_prev == c && *c->m_next == *this) {
            c->m_next = nullptr;
            return true;
        }

        if (m_next == c && *c->m_prev == *this) {
            c->m_prev = nullptr;
            return true;
        }
        return false;
    }

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
        PrevRole = Qt::UserRole + 2
    };

    ChaikinsModel(QObject* parent = 0);

    Q_INVOKABLE int append(QPoint controlPoint);
    Q_INVOKABLE void remove(int index);
    Q_INVOKABLE void connect(QPoint c1, QPoint c2);
    Q_INVOKABLE QPoint get(int index);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    bool refine(QList<QPoint>& out, int level);

private:
    bool refineRec(QList<QPoint>& out, int level);
    ControlPoint* find(QPoint p);
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
