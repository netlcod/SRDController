#include "itemwrapper.h"

ItemWrapper::ItemWrapper(QGraphicsItem* parent) : item(parent) {}

QGraphicsItem* ItemWrapper::graphicsItem() const {
    return item;
}

QRectF ItemWrapper::boundingRect() const {
    return item->boundingRect();
}

void ItemWrapper::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    return item->paint(painter, option, widget);
}

QVariant ItemWrapper::itemChange(GraphicsItemChange change, const QVariant& value) {
    return QGraphicsItem::itemChange(change, value);
}
