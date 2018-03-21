#include "chaikins.h"

#include <QSGGeometry>
#include <QSGGeometryNode>
#include <QSGFlatColorMaterial>

ChaikinsCurve::ChaikinsCurve(QQuickItem *parent) : QQuickItem(parent)
    ,m_model(nullptr)
    ,m_refinment(0) {
    setFlag(ItemHasContents, true);
    update();
}

ChaikinsCurve::~ChaikinsCurve() {}

void ChaikinsCurve::setRefinement(int newRefinement) {
    if (m_refinment == newRefinement)
        return;

    m_refinment = newRefinement;
    emit refinementChanged(newRefinement);
    update();
}

void ChaikinsCurve::setModel(ChaikinsModel* model) {
    if(m_model == model)
        return;

    m_model = model;
    connect(model, &ChaikinsModel::rowsInserted, [=](const QModelIndex &parent, int first, int last) {
        update();
    });

    connect(model, &ChaikinsModel::dataChanged, [=](const QModelIndex &, const QModelIndex &, const QVector<int>) {
        update();
    });

    connect(model, &ChaikinsModel::rowsRemoved, [=](const QModelIndex &parent, int first, int last) {
       update();
    });

    emit modelChanged(model);
    update();
}

QSGNode* ChaikinsCurve::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) {
    QSGGeometryNode *node = 0;
    QSGGeometry* geometry = 0;

    // Without any model we cant do anything
    if(!m_model)
        return nullptr;

    // Refine
    QList<QPoint> refinedData;
    m_model->refine(refinedData, m_refinment);

    if(!oldNode) {
        node = new QSGGeometryNode;

        geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), refinedData.count());
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
        geometry->allocate(refinedData.count());
    }

    QSGGeometry::Point2D *vertices = geometry->vertexDataAsPoint2D();
    for (int i = 0; i < refinedData.count(); i++) {
       vertices[i].set(refinedData[i].x(), refinedData[i].y());
    }

    node->markDirty(QSGNode::DirtyGeometry);
    return node;
}
