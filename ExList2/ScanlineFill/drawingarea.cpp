#include "drawingarea.h"
#include "ui_drawingarea.h"
#include <QPalette>
#include <QMouseEvent>

DrawingArea::DrawingArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DrawingArea)
{
    qPointsT qPoints;

    ui->setupUi(this);
    QPalette pal = palette();

    this->qPoints = qPoints;
    this->mode = 'p';

    // set white background
    pal.setColor(QPalette::Background, Qt::white);
    this->setAutoFillBackground(true);
    this->setPalette(pal);
    this->show();



}

DrawingArea::~DrawingArea(){
    delete ui;
}



/*

Paint events are sent to widgets that need to UPDATE or REPAINT themselves,
*/
void DrawingArea::paintEvent(QPaintEvent *){
    QPainter painter(this);
    char number[4];
    int index = 0;

    //do not draw 0 sides polygons
    if(qPoints.empty())
        return;

    if(mode == 'p'){
        painter.setPen(Qt::blue);
        for(qPointsT::iterator it = qPoints.begin(); it != qPoints.end(); ++it){
            index++;
            sprintf(number, "%d", index);
            painter.drawText(*it, number);
        }
    }
    else{
        painter.setPen(Qt::red);
        for(qPointsT::iterator it = qPoints.begin(); it != qPoints.end()-1; ++it){
            painter.drawLine(*it, *(it+1));

            //draw last side: from begin to last
            painter.drawLine(*qPoints.begin(), *qPoints.rbegin());
        }
        if(mode == 'f' && qPoints.size() > 2)
            scanlineFill(&painter);
    }
}

std::priority_queue<edge*, std::vector<edge*>,compare> DrawingArea::createEdges(){
    std::priority_queue<edge*, std::vector<edge*>,compare> edges;
    edge *e;
    int y0, y1, x0, x1;

    // Building edge structures
    for(qPointsT::iterator it = qPoints.begin(); it != qPoints.end()-1; ++it){
        y0 = it->y();
        y1 = (it+1)->y();
        x0 = it->x();
        x1 = (it+1)->x();

        e = new edge;
        e->maxY = std::max(y0, y1);

        // Slope
        if(x0 == x1)
            e->xIncr = 0;
        else
            e->xIncr = (x1-x0)/(y1-y0);

        // Other values.
        if(e->maxY == y0){
            e->currentX = x0;
            e->minY = y1;
        }
        else{
            e->currentX = x1;
            e->minY = y0;
        }

        edges.push(e);
    }

    // Last edge
    y0 = qPoints.rbegin()->y();
    y1 = qPoints.begin()->y();
    x0 = qPoints.rbegin()->x();
    x1 = qPoints.begin()->x();

    e = new edge;
    e->maxY = std::max(y0, y1);

    // Slope.
    if(x0 == x1)
        e->xIncr = 0;
    else
        e->xIncr = (x1-x0)/(y1-y0);

    // Other values.
    if(e->maxY == y0){
        e->currentX = x0;
        e->minY = y1;
    }
    else{
        e->currentX = x1;
        e->minY = y0;
    }

    edges.push(e);
    return edges;
}

bool compare_AET_X (edge* i, edge* j){
    return i->currentX < j->currentX;
}


// Filling the polygon using the Scanline Fill Algorithm
void DrawingArea::scanlineFill(QPainter *paint){
    std::priority_queue<edge*, std::vector<edge*>,compare> ET;
    std::forward_list<edge*> AET;

    edge* e;
    int scanline, maxY, currX;
    int size;

    // Building edges
    ET = createEdges();

    // With the edges, it's possible to fill an edge table.
    // But we can use the edges priority queue as the edge table.
    // We start the algorithm at the minY scanline, instead of 0, as an optimization.
    e = ET.top();
    ET.pop();

    AET.push_front(e);
    scanline = e->minY;
    maxY = e->maxY;

    // While the max Y is not reached (to be updated if needed)
    do{

        // Delete finished edges.
        for(auto it = AET.before_begin(); it != AET.end(); ++it){
            if(std::next(it) != AET.end()){
                if((*std::next(it))->maxY == scanline){
                    AET.erase_after(it);
                }
            }
        }

        // Insert edges with same minY.
        while (!ET.empty() && ET.top()->minY == scanline){
            e = ET.top();
            ET.pop();

            // Update maxY and add edge.
            maxY = std::max(e->maxY, maxY);
            AET.push_front(e);
        }

        // Sort AET by X
        AET.sort(compare_AET_X);

        // Fills line between pairs of X.
        for(auto it = AET.begin(); it != AET.end(); ++it){

            // First point.
            e = *it;
            currX = e->currentX;

            // Second point
            it = std::next(it);
            e = *it;

            paint->drawLine((int)currX, scanline, (int)e->currentX, scanline);
        }

        // Increments all current X
        for(auto it = AET.begin(); it != AET.end(); ++it){
            (*it)->currentX += (*it)->xIncr;
        }

        // Next scan line.
        scanline++;
    } while(scanline != maxY);
}

void DrawingArea::mousePressEvent(QMouseEvent *event){
    qPoints.push_back(event->pos());
    this->mode='p';
    this->update();
}

void DrawingArea::setMode(char mode){
    this->mode=mode;
}

void DrawingArea::clear(){
    qPoints.clear();
}
