#pragma once

#include <QGraphicsScene>

class GridScene : public QGraphicsScene {
    Q_OBJECT

public:
    explicit GridScene(QObject* parent = 0) {}

    int getCols() const;
    void setColumnCount(int newCols);

    int getRows() const;
    void setRowCount(int newRows);

    int getStep() const;
    void setGridStep(int newStep);

    int getBStep() const;
    void setGridBoldStep(int newStep);

protected:
    void drawBackground(QPainter* painter, const QRectF& rect);

private:
    int cols = 0;  // count
    int rows = 0;  // count
    int step = 0;  // px
    int bstep = 0; // px
};
