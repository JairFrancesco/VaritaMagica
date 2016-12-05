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

public slots:
    //void drawBackground(QPainter *painter, const QRectF &rect);
    //void mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void segmentarImagen();
    //void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    bool over;
    QString str;
    //QGraphicsTextItem cursor;
};

#endif // TGS_H
