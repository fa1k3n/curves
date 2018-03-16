#include "chaikins.h"

ChaikinsModel::ChaikinsModel(QObject*) {
    beginInsertRows(QModelIndex(), m_controlPoints.count(), m_controlPoints.count() - 1);
    m_controlPoints.append(QPoint(100, 350));
    m_controlPoints.append(QPoint(200, 50));
    m_controlPoints.append(QPoint(300, 175));
    endInsertRows();
}

int ChaikinsModel::rowCount(const QModelIndex &/* parent */) const {
    return m_controlPoints.size();
}

QVariant ChaikinsModel::data(const QModelIndex &index, int role) const {
    if(!index.isValid())
        return QVariant();

    if(index.row() >= m_controlPoints.size() || index.row() < 0)
        return QVariant();

    if(role == PositionRole)
        return m_controlPoints.at(index.row());

    return QVariant();
}

QHash<int, QByteArray> ChaikinsModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[PositionRole] = "position";
    return roles;
}

bool ChaikinsModel::refine(QList<QPoint>& out, int level) {
    for(auto cp : m_controlPoints)
        out.append(cp);
    return refineRec(out, level);
}

bool ChaikinsModel::refineRec(QList<QPoint>& out, int level) {
    if (level == 0)
        return true;

    QList<QPoint> refined;
    for(int i = 1; i < out.size(); i++) {
        QPoint p2 = out[i];
        QPoint p1 = out[i-1];
        QPoint diff =  p2 - p1;

        refined.push_back(p1 + diff*1/4);
        refined.push_back(p2 - diff*1/4);
    }

    out = refined;
    return refineRec(out, level - 1);
}

void ChaikinsModel::append(QPoint controlPoint) {
    beginInsertRows(QModelIndex(), m_controlPoints.count(), m_controlPoints.count());
    m_controlPoints.append(controlPoint);
    endInsertRows();
}

void ChaikinsModel::remove(int index) {
    if(index < 0 || index >= m_controlPoints.count())
        return;

    beginRemoveRows(QModelIndex(), index, index);
    m_controlPoints.removeAt(index);
    endRemoveRows();

}

bool ChaikinsModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    QPoint p = value.toPoint();
    if (p == m_controlPoints[index.row()])
        return false;
    m_controlPoints[index.row()] = p;
    emit dataChanged(index , index);
    return true;
}

