#include "chaikins.h"

#include <QSGGeometry>
#include <QSGGeometryNode>
#include <QSGFlatColorMaterial>

Chaikins::Chaikins(QQuickItem *parent) : QQuickItem(parent)
    ,m_refinment(0) {
    setFlag(ItemHasContents, true);

    m_controlPoints.push_back(QPoint(200, 220));
    m_controlPoints.push_back(QPoint(260, 100));
    m_controlPoints.push_back(QPoint(350, 180));
    update();
}

Chaikins::~Chaikins() {}

void Chaikins::setRefinement(int newRefinement) {
    if (m_refinment == newRefinement)
        return;

    m_refinment = newRefinement;
    emit refinementChanged(newRefinement);
    update();
}

#include <iostream>
void Chaikins::setControlPolygon(QAbstractItemModel* newPolygon) {
    connect(newPolygon, &QAbstractItemModel::rowsInserted, [=](const QModelIndex &parent, int first, int last) {
        auto values = newPolygon->data(newPolygon->index((last, 0), 0));
        std::cout << "CHANGED ROW COUNT " << newPolygon->rowCount() << " " << values.toPoint().x() << std::endl;
    });
    std::cout << "ROW COUNT " << newPolygon->rowCount() << std::endl;
}


std::vector<QPoint>* Chaikins::refine(std::vector<QPoint>& oldControlPoints) {
    std::vector<QPoint>* newControlPoints = new std::vector<QPoint>;
    for(int i = 1; i < oldControlPoints.size(); i++) {
        QPoint diff = oldControlPoints[i] - oldControlPoints[i-1];
        newControlPoints->push_back(oldControlPoints[i-1] + diff*1/4);
        newControlPoints->push_back(oldControlPoints[i] - diff*1/4);
    }
    return newControlPoints;
}

QSGNode* Chaikins::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) {
    QSGGeometryNode *node = 0;
    QSGGeometry* geometry = 0;

    // Refine
    std::vector<QPoint>* newControlPoints = new std::vector<QPoint>(m_controlPoints);
    for(int lvl = 0; lvl < m_refinment; lvl++ ) {
        newControlPoints = refine(*newControlPoints);
    }

    if(!oldNode) {
        node = new QSGGeometryNode;

        geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), newControlPoints->size());
        geometry->setLineWidth(2);
        geometry->setDrawingMode(QSGGeometry::DrawLineStrip);
        node->setGeometry(geometry);
        node->setFlag(QSGNode::OwnsGeometry);

        QSGFlatColorMaterial *material = new QSGFlatColorMaterial;
        material->setColor(QColor("red"));
        node->setMaterial(material);
        node->setFlag(QSGNode::OwnsMaterial);
    } else {
        node = static_cast<QSGGeometryNode*>(oldNode);
        geometry = node->geometry();
        geometry->allocate(newControlPoints->size());
    }

    QSizeF itemSize = size();


    QSGGeometry::Point2D *vertices = geometry->vertexDataAsPoint2D();
    for (int i = 0; i < newControlPoints->size(); i++) {
       vertices[i].set(newControlPoints->at(i).x(), newControlPoints->at(i).y());
    }

    node->markDirty(QSGNode::DirtyGeometry);
    return node;
}
