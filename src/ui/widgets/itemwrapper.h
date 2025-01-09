#pragma once

#include <QCursor>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>

class ItemWrapper : public QGraphicsItem {
public:
    explicit ItemWrapper(QGraphicsItem* parent);

    QGraphicsItem* graphicsItem() const;
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

private:
    QGraphicsItem* item;
};
