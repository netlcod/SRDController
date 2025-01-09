#include "gridscene.h"
#include <QLine>
#include <QPainter>
#include <QVarLengthArray>

int GridScene::getCols() const {
    return cols;
}

void GridScene::setColumnCount(int newCols) {
    cols = newCols;
    update();
}

int GridScene::getRows() const {
    return rows;
}

void GridScene::setRowCount(int newRows) {
    rows = newRows;
    update();
}

int GridScene::getStep() const {
    return step;
}

void GridScene::setGridStep(int newStep) {
    step = newStep;
    update();
}

int GridScene::getBStep() const {
    return bstep;
}

void GridScene::setGridBoldStep(int newStep) {
    bstep = newStep;
    update();
}

void GridScene::drawBackground(QPainter* painter, const QRectF& rect) {
    QVarLengthArray<QLine, 1000> lines;
    for (int x = -0.5 * cols * step; x <= 0.5 * cols * step; x += step) {
        lines.append(QLine(x, -0.5 * rows * step, x, 0.5 * rows * step));
    }
    for (int y = -0.5 * rows * step; y <= 0.5 * rows * step; y += step) {
        lines.append(QLine(-0.5 * cols * step, y, 0.5 * cols * step, y));
    }
    painter->setPen(QPen(Qt::lightGray));
    painter->drawLines(lines.data(), lines.size());

    QVarLengthArray<QLine, 1000> blines;
    for (int x = -0.5 * cols * step; x <= 0.5 * cols * step; x += bstep) {
        blines.append(QLine(x, -0.5 * rows * step, x, 0.5 * rows * step));
    }
    for (int y = -0.5 * rows * step; y <= 0.5 * rows * step; y += bstep) {
        blines.append(QLine(-0.5 * cols * step, y, 0.5 * cols * step, y));
    }
    painter->setPen(QPen(Qt::darkGray));
    painter->drawLines(blines.data(), blines.size());

    painter->setPen(QPen(Qt::black));
    painter->drawLine(0, rect.top(), 0, rect.bottom());
    painter->drawLine(rect.left(), 0, rect.right(), 0);
}
