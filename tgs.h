#ifndef TGS_H
#define TGS_H

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QImage>

QT_BEGIN_NAMESPACE

class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;

QT_END_NAMESPACE

class TGs : public QGraphicsScene
{
public:
    TGs(QObject *parent = 0);
    int X = 0;
    int Y = 0;
    QImage imagen;
    QImage res;

private:
    float valorSigma = 0.6;
    float constanteK = 180;
    int minSize = 100;

public slots:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void segmentarImagen();
    void mostrarImagenResultante(QImage res);
    bool over;
    QString str;
    //QGraphicsTextItem cursor;
};

#endif // TGS_H
