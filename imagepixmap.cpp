#include "imagepixmap.hpp"
#include <cmath>
#include <QDebug>

ImagePixmap::ImagePixmap(const QPixmap &pixmap, WorkArea *wa, Cropper *crp) :
    QGraphicsPixmapItem(pixmap), WA(wa), CRP(crp), CurrentScale(1)
{
}

void ImagePixmap::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    double scaleFactor = pow((double)2, event->delta() / 240.0);
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.2 || factor > 25)
        return;

    CurrentScale *= scaleFactor;
    scale(scaleFactor, scaleFactor);

    QTransform trans = QTransform().scale(CurrentScale, CurrentScale);
    WA->CurPxmp = WA->photoPxmp.transformed(trans, Qt::SmoothTransformation);

    CRP->getSamples();
}


