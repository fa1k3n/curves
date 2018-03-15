#include "line.h"

#include <QSGGeometry>
#include <QSGGeometryNode>
#include <QSGFlatColorMaterial>

Line::Line(QQuickItem *parent) : QQuickItem(parent)
    , m_start(nullptr)
    , m_end(nullptr)
    , m_width(0)
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
        geometry->setLineWidth(m_width);
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

    QSizeF itemSize = size();
    QSGGeometry::Point2D *vertices = geometry->vertexDataAsPoint2D();
    vertices[0].set(m_start->x() + m_start->width()/2, m_start->y() + m_start->height()/2);
    vertices[1].set(m_end->x() + m_end->width()/2, m_end->y() + m_end->height()/2);

    node->markDirty(QSGNode::DirtyGeometry);
    return node;
}

void Line::setStart(QQuickItem *p) {
    if(p == m_start)
        return;

    m_start = p;
    emit startChanged(p);
    connect(p, &QQuickItem::xChanged, [=]() {
        emit startChanged(p);
        update();
    });
    connect(p, &QQuickItem::destroyed, [=]() {
        emit startChanged(p);
    });
    update();
}


void Line::setEnd(QQuickItem *p) {
    if(p == m_end)
        return;

    m_end = p;
    emit endChanged(p);
    connect(p, &QQuickItem::xChanged, [=]() {
        emit endChanged(p);
        update();
    });
    connect(p, &QQuickItem::destroyed, [=]() {
        emit endChanged(p);
    });
    update();
}

void Line::setWidth(int w) {
    if(w == m_width)
        return;

    m_width = w;
    emit widthChanged(w);
    update();
}

void Line::setColor(QColor const &c) {
    if(c == m_color)
        return;

    m_color = c;
    emit colorChanged(c);
    update();
}
