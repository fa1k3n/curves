#include "chaikins.h"

ChaikinsModel::ChaikinsModel(QObject*) {

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
    else if(role == PrevRole)
        if(m_controlPoints.at(index.row()).prev())
            return *m_controlPoints.at(index.row()).prev();
        else
            return m_controlPoints.at(index.row());

    return QVariant();
}

QHash<int, QByteArray> ChaikinsModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[PositionRole] = "position";
    roles[PrevRole] = "prev";

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

int ChaikinsModel::append(QPoint controlPoint) {
    beginInsertRows(QModelIndex(), m_controlPoints.count(), m_controlPoints.count());
    m_controlPoints.append(ControlPoint(controlPoint.x(), controlPoint.y()));
    endInsertRows();
    return m_controlPoints.count() - 1; // Return index just inserted
}

void ChaikinsModel::connect(QPoint c1, QPoint c2) {
    auto cp1 = find(c1);
    auto cp2 = find(c2);
    if(cp1->connect(cp2))
        std::cout << "Connected " << *cp1 << " with " << *cp2 << std::endl;

}

ControlPoint* ChaikinsModel::find(QPoint p) {
    for(int i = 0; i < m_controlPoints.count(); i++) {
        if(m_controlPoints[i] == p) return &m_controlPoints[i];
    }
}

void ChaikinsModel::remove(int index) {
    if(index < 0 || index >= m_controlPoints.count())
        return;

    auto cp = m_controlPoints.at(index);
    // Disconnect from neighbours
    cp.disconnect(cp.prev());
    cp.disconnect(cp.next());

    // Remove from model
    beginRemoveRows(QModelIndex(), index, index);
    m_controlPoints.removeAt(index);
    endRemoveRows();
}

QPoint ChaikinsModel::get(int index) {
    return m_controlPoints.at(index);
}

bool ChaikinsModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    QPoint p = value.toPoint();
    if (p == m_controlPoints[index.row()])
        return false;
    m_controlPoints[index.row()].setX(p.x());
    m_controlPoints[index.row()].setY(p.y());
    emit dataChanged(index , index);
    return true;
}

