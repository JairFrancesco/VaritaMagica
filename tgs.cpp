#include <QtGui>
#include "tgs.h"
#define _alto  300
#define _ancho 700
#include <QGraphicsSceneMouseEvent>
#include <iostream>
#include <utility.h>

TGs::TGs(QObject *parent)
    :QGraphicsScene(parent)
{ // Constructor of Scene
    this->over = false;
}

/*
void TGs::drawBackground(QPainter *painter, const QRectF &rect)
{

#define adjy 30
#define adjx 30

    int j = 0;
    int alto = 0;

    QPen pen;
    pen.setWidth(1);
    pen.setBrush(Qt::lightGray);
    painter->setPen(pen);

    painter->drawText(-225, 10, this->str);
    alto = _alto;  // 50 + 2

    for(int i = 0; i < alto; ++i)
    {
        j = i * adjy - 17;

        painter->drawLine(QPoint(-210, j), QPoint(_ancho, j));
    }

    for(int i = 0; i < 300; ++i)
    {
        j = i * adjx - 210;

        painter->drawLine(QPoint(j, 0), QPoint(j, _ancho * 2));
    }
}
*/

/*
void TGs::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QString string = QString("%1, %2")
               .arg(mouseEvent->scenePos().x())
              .arg(mouseEvent->scenePos().y()); // Update the cursor position text
    this->str = string;
    this->update();
}
*/


void TGs::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    std::cout<<"X:"<<(event->scenePos().x())<<", Y:"<<(event->scenePos().y())<<std::endl;
    this->X = event->scenePos().x();
    this->Y = event->scenePos().y();
    this->segmentarImagen();
    this->update();
}

void TGs::segmentarImagen()
{
    float sigma = 0.5;
    float k = 200;
    int min_size = 50;

    Utility* ut = new Utility();
    image <rgb> *input = ut->qImageToImage(this->imagen);
    int num_ccs;
    image<rgb> *seg = ut->segmentImage(input, sigma, k, min_size, &num_ccs);

    res = ut->imageToQImage(seg);

    //res = Utility::imageToQImage(input);

    delete input;
    delete seg;

    QPixmap pixmapTo = QPixmap::fromImage(res);
    this->clear();
    this->addPixmap(pixmapTo);
}

/*
void TGs::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    this->update();
}
*/
