#ifndef RESIZER_HPP
#define RESIZER_HPP

#include <QObject>
#include <QGraphicsItem>
#include <QtGui>
#include <defines.hpp>

class Resizer : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit Resizer(QGraphicsItem *crop, Position pos);
    

    qreal width;
    qreal height;
    Position position;
    QColor ColorBase;
    QColor ColorAnchored;
    QColor ColorMousePressed;
    bool anchored;
    bool pressed;


    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget);

    virtual QRectF boundingRect() const;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    void updatePosition();

signals:
    void sizeChanged(qreal, qreal, Position);
};

#endif // RESIZER_HPP
