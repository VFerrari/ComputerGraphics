/*
 * MC937 - Computer Graphics
 * Exercise List 2 - Exercise 2: Scanline Fill Algorithm.
 *
 * Qt application to draw a polygon and fill using the Scanline Fill Algorithm.
 *
 * DrawingArea: Main area for drawing.
 * Has the implementation of the Scanline Fill Algorithm and other painting functions.
 *
 * Author:
 *      Victor Ferreira Ferrari - RA 187890
 *
 * Last Modified: 29/03/2020
 */

#include "drawingarea.h"
#include "ui_drawingarea.h"

DrawingArea::DrawingArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DrawingArea)
{
    qPointsT qPoints;
    QPalette pal = palette();

    ui->setupUi(this);

    this->qPoints = qPoints;
    this->mode = 'p';
    this->color = Qt::black;

    // set white background
    pal.setColor(QPalette::Background, Qt::white);
    this->setAutoFillBackground(true);
    this->setPalette(pal);
    this->show();
}

DrawingArea::~DrawingArea(){
    delete ui;
}

void DrawingArea::setMode(char mode){
    this->mode=mode;
}

void DrawingArea::setColor(QColor color){
    this->color = color;
}

/**
 * Finds closest point to the reference point.
 * Calculates the distance between the reference and all points.
 * @brief DrawingArea::closestPoint
 * @param ref: point of reference to distance.
 * @return index of the closest point.
 */
int DrawingArea::closestPoint(QPoint ref){
    QPoint diff;
    double dist, minDist = 1000000;
    int index;

    // Calculates distance.
    for (int i = 0; i < (int)qPoints.size(); ++i){
        diff = ref - qPoints[i];
        dist = sqrt(diff.x()*diff.x() + diff.y()*diff.y());

        // Updates minimum distance/closest point.
        if(dist < minDist){
            minDist = dist;
            index = i;
        }
    }

    return index;
}

/**
 * Handles mouse presses in the drawing area.
 * Left button: NEW vertex to polygon.
 * Right button: modify polygon, position of the closest point.
 * @brief DrawingArea::mousePressEvent
 * @param event The mouse press properties (position, etc).
 */
void DrawingArea::mousePressEvent(QMouseEvent *event){
    int index;

    // If right button, we find the closest point and modifies its position.
    if(event->button() == Qt::RightButton){
        if(qPoints.empty())
            return;

        index = closestPoint(event->pos());
        qPoints[index].setX(event->pos().x());
        qPoints[index].setY(event->pos().y());
    }

    // If left button, new vertex.
    else
        qPoints.push_back(event->pos());

    this->mode='p';
    this->update();

    // Disable fill button.
    ((MainWindow*)(parent()->parent()))->changeFill(false);
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
        painter.setPen(color);
        for(qPointsT::iterator it = qPoints.begin(); it != qPoints.end()-1; ++it){
            painter.drawLine(*it, *(it+1));

            //draw last side: from begin to last
            painter.drawLine(*qPoints.begin(), *qPoints.rbegin());
        }
        if(mode == 'f' && qPoints.size() > 2)
            scanlineFill(&painter);
    }
}

/**
 * Creates the edge structures for each edge, and inserts into a priority queue.
 * Priority queue takes the form of a min heap.
 * xIncr: line slope.
 * @brief DrawingArea::createEdges
 */
auto DrawingArea::createEdges(){
    std::priority_queue<edge*, std::vector<edge*>,compareMinY> edges;
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
        if(y0 == y1)
            e->xIncr = 0;
        else
            e->xIncr = (x1-x0)/(double)(y1-y0);

        // Other values.
        if(e->maxY == y0){
            e->currentX = x1;
            e->minY = y1;
        }
        else{
            e->currentX = x0;
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
    if(y0 == y1)
        e->xIncr = 0;
    else
        e->xIncr = (x1-x0)/(double)(y1-y0);

    // Other values.
    if(e->maxY == y0){
        e->currentX = x1;
        e->minY = y1;
    }
    else{
        e->currentX = x0;
        e->minY = y0;
    }

    edges.push(e);
    return edges;
}

bool compare_AET_X (edge* i, edge* j){
    return i->currentX < j->currentX;
}

/**
 * Filling the polygon using the Scanline Fill Algorithm
 * @brief DrawingArea::scanlineFill
 * @param paint: painter for DrawingArea.
 */
void DrawingArea::scanlineFill(QPainter *paint){
    std::priority_queue<edge*, std::vector<edge*>,compareMinY> ET;
    std::forward_list<edge*> AET;

    edge* e;
    int scanline, maxY, currX;

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
            currX = round(e->currentX);

            // Second point
            it = std::next(it);

            // Just in case an odd number of vertices are in the AET (discretion problem).
            if (it == AET.end())
                break;

            e = *it;

            paint->drawLine(currX, scanline, (int)e->currentX, scanline);
        }

        // Increments all current X
        for(auto it = AET.begin(); it != AET.end(); ++it){
            (*it)->currentX += (*it)->xIncr;
        }

        // Next scan line.
        scanline++;
    } while(scanline != maxY);
}

/**
 * Resets drawing area, clearing points and resetting pen color.
 * @brief DrawingArea::clear
 */
void DrawingArea::clear(){
    qPoints.clear();
    this->color = Qt::black;
}
