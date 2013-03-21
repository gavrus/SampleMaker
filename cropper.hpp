#ifndef CROPPER_HPP
#define CROPPER_HPP

#include <QObject>
#include <qglobal.h>
#include <QGraphicsItem>
#include <QtGui>
#include <resizer.hpp>
#include <cmath>
#include <workarea.hpp>
#include <defines.hpp>

class WorkArea;
class Resizer;

Q_GUI_EXPORT void qt_blurImage( QImage &blurImage, qreal radius,
                                bool quality, int transposed = 0);

class Cropper : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit Cropper(WorkArea *wa);

    bool anchored;
    qreal width;
    qreal height;
    qreal RotateAngle;
    qreal AnchoredAngle;

    Resizer *RSide;
    Resizer *LSide;
    Resizer *USide;
    Resizer *BSide;

    WorkArea *WA;


    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget);

    virtual QRectF boundingRect() const;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    virtual void wheelEvent(QGraphicsSceneWheelEvent *event);
    void updateItemsPos();
    void setHighLumin();
    void setLowLumin();
    QPixmap convertToGray(QPixmap &pxmp);
    void normalize(QPixmap src);
    QPixmap adjustLuminance(QPixmap &pxm, qreal contrast, qreal bright);

public slots:
    void changeSize(qreal dx, qreal dy, Position position);
    void getSamples();
    void changeLuminance(Luminance Lumin);
    void blur(int KernelSize);
    void addNoize(double mu);
    void leftDark();
    void rightDark();
};

#endif // CROPPER_HPP
