#ifndef DRAWINGAREA_H
#define DRAWINGAREA_H

// Qt Libraries
#include <QWidget>
#include <QPainter>
#include <QPoint>
#include <QPalette>
#include <QMouseEvent>

// C++ Libraries
#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>
#include <forward_list>

// C Libraries
#include <math.h>

// File headers
#include "mainwindow.h"

struct edge {
             int maxY, minY;
             double currentX;
             double xIncr;
             };

struct compareMinY {
                    bool operator()(const edge* a, const edge* b){
                        return a->minY > b->minY;
                    }
                   };

namespace Ui {
class DrawingArea;
}

class DrawingArea : public QWidget
{
    // Meta-Object Compiler macro
    Q_OBJECT

    //std vector of QPoints type
    using  qPointsT = std::vector<QPoint>;
    using edge = struct edge;

public:

    explicit DrawingArea(QWidget *parent = nullptr);
    ~DrawingArea();

    void setMode(char mode);
    void setColor(QColor color);
    void clear();
private:
    Ui::DrawingArea *ui;
    qPointsT qPoints;
    QColor color;
    char mode;

    auto createEdges();
    void scanlineFill(QPainter *paint);
    void mousePressEvent(QMouseEvent *event);
    int closestPoint(QPoint ref);
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // DRAWINGAREA_H
