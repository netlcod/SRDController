#include "movableitemwrapper.h"

MovableItemWrapper::MovableItemWrapper(QGraphicsItem* parent) : item(parent) {
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsScenePositionChanges);
}

QGraphicsItem* MovableItemWrapper::graphicsItem() const {
    return item;
}

QRectF MovableItemWrapper::boundingRect() const {
    return item->boundingRect();
}

void MovableItemWrapper::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    return item->paint(painter, option, widget);
}

QVariant MovableItemWrapper::itemChange(GraphicsItemChange change, const QVariant& value) {
    if (change == ItemPositionChange) {
        emit itemMoved(value.toPointF());
    }
    return QGraphicsItem::itemChange(change, value);
}

void MovableItemWrapper::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    this->setPos(mapToScene(event->pos()));
}

void MovableItemWrapper::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    this->setCursor(QCursor(Qt::ClosedHandCursor));
}

void MovableItemWrapper::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    this->setCursor(QCursor(Qt::ArrowCursor));
}
