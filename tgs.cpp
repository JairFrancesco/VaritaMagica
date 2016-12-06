#include <QtGui>
#include "tgs.h"
#define _alto  300
#define _ancho 700
#include <QGraphicsSceneMouseEvent>
#include <iostream>
#include <utilidades.h>

TGs::TGs(QObject *parent)
    :QGraphicsScene(parent)
{ // Constructor of Scene
    this->over = false;
}


void TGs::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    std::cout<<"X:"<<(event->scenePos().x())<<", Y:"<<(event->scenePos().y())<<std::endl;
    this->X = event->scenePos().x();
    this->Y = event->scenePos().y();
    int altura = this->imagen.size().height();
    int ancho = this->imagen.size().width();
    if (this->X > 0 && this->X < ancho && this->Y > 0 && this->Y < altura)
    {
        this->segmentarImagen();
        this->update();
    }

}

void TGs::mostrarImagenResultante(QImage res)
{
    QPixmap pixmapTo = QPixmap::fromImage(res);
    this->clear();
    this->addPixmap(pixmapTo);
}


void TGs::segmentarImagen()
{
    Utilidades* ut = new Utilidades();
    if (this->imagen.isNull())
    {
        std::cout<<"Seleccione una imagen!!"<<std::endl;
    }
    image <rgb> *imgOriginal = ut->qImageToImage(this->imagen);
    std::cout<<"Llego hasta este punto"<<std::endl;
    image<rgb> *imgResultante = ut->segmentarImagen(imgOriginal, this->valorSigma, this->constanteK, this->minSize, this->X, this->Y);
    res = ut->imageToQImage(imgResultante);
    delete imgOriginal;
    delete imgResultante;
    mostrarImagenResultante(res);
}
