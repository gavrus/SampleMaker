#ifndef IMAGEPIXMAP_HPP
#define IMAGEPIXMAP_HPP

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneWheelEvent>
#include <workarea.hpp>
#include <cropper.hpp>

class WorkArea;
class Cropper;

class ImagePixmap : public QGraphicsPixmapItem
{
public:
    ImagePixmap(const QPixmap &pixmap, WorkArea *wa, Cropper *crp);
    WorkArea *WA;
    Cropper *CRP;
    qreal CurrentScale;

protected:
    virtual void wheelEvent (QGraphicsSceneWheelEvent *event);
};

#endif // IMAGEPIXMAP_HPP
