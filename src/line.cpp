#include "line.h"

#include <QSGGeometry>
#include <QSGGeometryNode>
#include <QSGFlatColorMaterial>

Line::Line(QQuickItem *parent) : QQuickItem(parent)
    , m_start(QPoint(0, 0))
    , m_end(QPoint(0, 0))
    , m_color(QColor("red")) {
    setFlag(ItemHasContents, true);
}

Line::Line(QPoint &start, QPoint& end, QQuickItem *parent) : QQuickItem(parent)
    , m_start(start)
    , m_end(end)
    , m_color(QColor("red")) {
    setFlag(ItemHasContents, true);
}

Line::~Line() {

}

QSGNode* Line::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) {
    QSGGeometryNode *node = 0;
    QSGGeometry* geometry = 0;

    if(!oldNode) {
        node = new QSGGeometryNode;

        geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 2);
        geometry->setLineWidth(width());
        geometry->setDrawingMode(QSGGeometry::DrawLineStrip);
        node->setGeometry(geometry);
        node->setFlag(QSGNode::OwnsGeometry);

        QSGFlatColorMaterial *material = new QSGFlatColorMaterial;
        material->setColor(m_color);
        node->setMaterial(material);
        node->setFlag(QSGNode::OwnsMaterial);
    } else {
        node = static_cast<QSGGeometryNode *>(oldNode);
        geometry = node->geometry();
        geometry->allocate(2);
    }

    QSGGeometry::Point2D *vertices = geometry->vertexDataAsPoint2D();
    vertices[0].set(m_start.x(), m_start.y());
    vertices[1].set(m_end.x(), m_end.y());

    node->markDirty(QSGNode::DirtyGeometry);
    return node;
}

void Line::setStart(QPoint &p) {
    if(p == m_start)
        return;

    m_start = p;
    emit lineChanged();

    update();
}


void Line::setEnd(QPoint &p) {
    if(p == m_end)
        return;

    m_end = p;
    emit lineChanged();

    update();
}

void Line::setColor(QColor const &c) {
    if(c == m_color)
        return;

    m_color = c;
    emit colorChanged(c);
    update();
}
