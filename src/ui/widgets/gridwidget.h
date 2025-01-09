#pragma once

#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QGraphicsView>
#include <QPointer>
#include <QSharedPointer>
#include <QShowEvent>
#include <QWheelEvent>

#include "gridscene.h"

class GridWidget : public QGraphicsView {
    Q_OBJECT

public:
    GridWidget(QWidget* parent = nullptr);

    enum class ItemType { Ellipse, Rect, MovableEllipse, MovableRect };

public slots:
    void setColumnCount(int newCols);
    void setRowCount(int newRows);
    void setGridStep(int newStep);
    void setGridBoldStep(int newStep);

    void addItem(ItemType type, QString name, QBrush brush, QRectF size);
    QPointF setItemPosition(QString name, QPointF pos);
    QPointF moveItem(QString name, QPointF pos);
    QPointF getItemPosition(QString name);

signals:
    void itemPositionChanged(QPointF pos);

protected:
    void wheelEvent(QWheelEvent* event) override;
    void showEvent(QShowEvent* event) override;

private:
    int innerSize = 10;
    int cols, rows;
    int step, bstep;

    QPointer<GridScene> scene;
    QMap<QString, QSharedPointer<QGraphicsItem>> items; // QAbstractGraphicsShapeItem
};
