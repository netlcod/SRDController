#include "gridwidget.h"
#include "itemwrapper.h"
#include "movableitemwrapper.h"

GridWidget::GridWidget(QWidget* parent) : QGraphicsView(parent) {
    setRenderHint(QPainter::Antialiasing);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    scene = new GridScene(this);
    setScene(scene);
}

void GridWidget::setColumnCount(int newCols) {
    cols = newCols;
    scene->clear();
    scene->setSceneRect(-0.5 * cols * step, -0.5 * rows * step, cols * step, rows * step);
    scene->setColumnCount(cols);
    fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void GridWidget::setRowCount(int newRows) {
    rows = newRows;
    scene->clear();
    scene->setSceneRect(-0.5 * cols * step, -0.5 * rows * step, cols * step, rows * step);
    scene->setRowCount(rows);
    fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void GridWidget::setGridStep(int newStep) {
    step = newStep;
    scene->clear();
    scene->setGridStep(step);
    scene->setSceneRect(-0.5 * cols * step, -0.5 * rows * step, cols * step, rows * step);
    fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void GridWidget::setGridBoldStep(int newStep) {
    bstep = newStep;
    scene->clear();
    scene->setGridBoldStep(bstep);
    scene->setSceneRect(-0.5 * cols * step, -0.5 * rows * step, cols * step, rows * step);
    fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void GridWidget::addItem(ItemType type, QString name, QBrush brush, QRectF size) {
    switch (type) {
        case ItemType::Ellipse: {
            QGraphicsEllipseItem* item = new QGraphicsEllipseItem(size);
            item->setBrush(brush);
            items[name] = QSharedPointer<ItemWrapper>::create(item);
            break;
        }
        case ItemType::Rect: {
            QGraphicsRectItem* item = new QGraphicsRectItem(size);
            item->setBrush(brush);
            items[name] = QSharedPointer<ItemWrapper>::create(item);
            break;
        }
        case ItemType::MovableEllipse: {
            QGraphicsEllipseItem* item = new QGraphicsEllipseItem(size);
            item->setBrush(brush);
            items[name] = QSharedPointer<MovableItemWrapper>::create(item);
            connect(qSharedPointerCast<MovableItemWrapper>(items[name]).get(), &MovableItemWrapper::itemMoved, this, [this](QPointF pos) {
                emit itemPositionChanged(pos / innerSize);
            });
            break;
        }
        case ItemType::MovableRect: {
            QGraphicsRectItem* item = new QGraphicsRectItem(size);
            item->setBrush(brush);
            items[name] = QSharedPointer<MovableItemWrapper>::create(item);
            connect(qSharedPointerCast<MovableItemWrapper>(items[name]).get(), &MovableItemWrapper::itemMoved, this, [this](QPointF pos) {
                emit itemPositionChanged(pos / innerSize);
            });
            break;
        }
    }

    scene->addItem(items[name].get());
    items[name]->setPos(0, 0);
}

QPointF GridWidget::setItemPosition(QString name, QPointF pos) {
    qreal newX = pos.x() * innerSize;
    qreal newY = pos.y() * innerSize;

    // Ограничиваем перемещение по сетке
    if (newX >= -0.5 * cols * step && newX <= 0.5 * cols * step) {
        items[name]->setX(newX);
    }
    if (newY >= -0.5 * rows * step && newY <= 0.5 * rows * step) {
        items[name]->setY(newY);
    }
    return getItemPosition(name);
}

QPointF GridWidget::moveItem(QString name, QPointF pos) {
    qreal newX = items[name]->x() + pos.x() * innerSize;
    qreal newY = items[name]->y() + pos.y() * innerSize;

    // Ограничиваем перемещение по сетке
    if (newX >= -0.5 * cols * step && newX <= 0.5 * cols * step) {
        items[name]->setX(newX);
    }
    if (newY >= -0.5 * rows * step && newY <= 0.5 * rows * step) {
        items[name]->setY(newY);
    }
    return getItemPosition(name);
}

QPointF GridWidget::getItemPosition(QString name) {
    qreal col = items[name]->pos().x() / step;
    qreal row = items[name]->pos().y() / step;

    return QPointF(col, row);
}

void GridWidget::wheelEvent(QWheelEvent* event) {
    double scaleFactor = 1.15;
    if (event->angleDelta().y() > 0) {
        scale(scaleFactor, scaleFactor);
    } else {
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }
}

void GridWidget::showEvent(QShowEvent* event) {
    fitInView(QRect(-0.5 * cols * step, -0.5 * rows * step, cols * step, rows * step), Qt::KeepAspectRatio);
}
