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
    else if(role == PrevRole) {
        auto prev = m_controlPoints.at(index.row()).prev();
        if (!prev) return QVariant();
        return *prev;
    } else if(role == NextRole) {
        auto next = m_controlPoints.at(index.row()).next();
        if (!next) return QVariant();
        return *next;
    }
    return QVariant();
}

QHash<int, QByteArray> ChaikinsModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[PositionRole] = "position";
    roles[PrevRole] = "prev";
    roles[NextRole] = "next";

    return roles;
}

bool ChaikinsModel::refine(QList<QPoint>& out, int level) {
    if(level < 0 || m_controlPoints.count() == 0)
        return false;
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
    ControlPoint* prev = nullptr;
    if(!m_controlPoints.isEmpty())
        prev = &m_controlPoints.last();
    m_controlPoints.append(ControlPoint(controlPoint.x(), controlPoint.y(), prev));
    // Update previous items next pointer
    endInsertRows();
    if(prev)
        setData(index(m_controlPoints.indexOf(*prev)), m_controlPoints.last(), NextRole);

    return m_controlPoints.count() - 1; // Return index just inserted
}

int ChaikinsModel::find(QPoint p, ControlPoint** cp) {
    for(int i = 0; i < m_controlPoints.count(); i++) {
        if(m_controlPoints[i] == p) {
            if(cp != nullptr) *cp = &m_controlPoints[i];
            return i;
        }
    }
    return -1;
}

void ChaikinsModel::remove(QPoint p) {
    ControlPoint* cp;
    int i = find(p, &cp);
    if(i == -1) return;

    // Connect neighbours
    // Start with prev
    if(cp->prev() && cp->prev()->next()) {
        auto n = cp->next() ? *cp->next() : QVariant();
        setData(index(find(*cp->prev())), n, NextRole);
    }
    // Then next
    if(cp->prev() && cp->next() && cp->next()->prev()) {
        auto n = cp->prev() ? *cp->prev() : QVariant();
        setData(index(find(*cp->next())), n, PrevRole);
    }

    // Remove from model
    beginRemoveRows(QModelIndex(), i, i);
    m_controlPoints.removeAt(i);
    endRemoveRows();
}

QVariant ChaikinsModel::get(int i) {
    return data(index(i), PositionRole);
}

bool ChaikinsModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    QPoint p = value.toPoint();
    ControlPoint* cp;
    find(p, &cp);
    if (p == m_controlPoints[index.row()])
        return false;
    if(role == PositionRole) {
        m_controlPoints[index.row()].setX(p.x());
        m_controlPoints[index.row()].setY(p.y());
    } else if (role == PrevRole) {
        ControlPoint* prev = nullptr;
        if(cp && !cp->isNull()) prev = cp;
        m_controlPoints[index.row()].setPrev(prev);
    } else if (role == NextRole) {
        ControlPoint* n = nullptr;
        if(cp && !cp->isNull()) n = cp;
        m_controlPoints[index.row()].setNext(n);
    }

    emit dataChanged(index , index);
    return true;
}

