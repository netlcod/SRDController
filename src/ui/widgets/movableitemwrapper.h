#pragma once

#include <QCursor>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>

class MovableItemWrapper : public QGraphicsObject {
    Q_OBJECT

public:
    explicit MovableItemWrapper(QGraphicsItem* parent);

    QGraphicsItem* graphicsItem() const;
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

private:
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

signals:
    void itemMoved(QPointF pos);

private:
    QGraphicsItem* item;
};
