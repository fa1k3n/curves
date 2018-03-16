#ifndef CHAIKINS_H
#define CHAIKINS_H

#include <QQuickItem>
#include <QAbstractListModel>

class ChaikinsModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum ChaikinsModelRoles {
        PositionRole = Qt::UserRole + 1
    };

    ChaikinsModel(QObject* parent = 0);

    Q_INVOKABLE void append(QPoint controlPoint);
    Q_INVOKABLE void remove(int index);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    bool refine(QList<QPoint>& out, int level);

private:
    bool refineRec(QList<QPoint>& out, int level);
    QList<QPoint> m_controlPoints;
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
