#include "cropper.hpp"
#include <QDebug>

double dRand(double Min, double Max)
{
    return Min + ((double)rand() / RAND_MAX) * (Max - Min);
}

QPixmap equalizeHistogram(QPixmap &pxm, int widthStart,
                          int widthEnd,  int UBoundNorm)
{
    QImage img = pxm.toImage();
    int imgH = img.height();
    int numPix = (widthEnd - widthStart)*imgH;

    //Histogram calculation
     std::vector<int> pdf;//Probability density function
     for(int i=widthStart; i<widthEnd; i++)
         for(int j=0; j<imgH; j++)
             pdf.push_back(qGray(img.pixel(i, j)));
     std::sort(pdf.begin(), pdf.end());
     int pdfLen = pdf.size();

     int accumul = 0;
     std::map<int, int> cdf;//Cumulative distribution function
     for(int i=0; i<pdfLen-1; i++)
     {
         ++accumul;
         if(pdf[i] != pdf[i+1])
             cdf.insert(std::pair<int,int>(pdf[i], accumul));
     }
     cdf.insert(std::pair<int,int>(pdf[pdfLen-1], ++accumul));

     //Equalize histogram
     int cdfMin = cdf.begin()->second;
     int diviser = numPix - cdfMin;
     int Pxl, newPxl;
     for(int i=widthStart; i<widthEnd; i++)
         for(int j=0; j<imgH; j++)
         {
             Pxl = qGray(img.pixel(i, j));
             newPxl = static_cast<double>((cdf[Pxl] - cdfMin))/
                      static_cast<double>(diviser)*UBoundNorm;
             img.setPixel(i,j, qRgb(newPxl,newPxl,newPxl));
         }
    return QPixmap::fromImage(img);
}

void showRotationTooltip(QGraphicsSceneMouseEvent *event, qreal angle, qreal ancangle, bool anchored)
{
    QPointF pos = event->screenPos();
    QPoint p;
    p.setX(static_cast<int>(pos.x()));
    p.setY(static_cast<int>(pos.y()));
    qreal rAngle = anchored? ceil(angle-ancangle) : ceil(angle);
    QString tooltip = "Rotation: " + QString::number(rAngle);
    QToolTip::showText(p, tooltip);
}

void showRotationTooltip(QGraphicsSceneWheelEvent *event, qreal angle, qreal ancangle, bool anchored)
{
    QPointF pos = event->screenPos();
    QPoint p;
    p.setX(static_cast<int>(pos.x()));
    p.setY(static_cast<int>(pos.y()));
    qreal rAngle = anchored? ceil(angle-ancangle) : ceil(angle);
    QString tooltip = "Rotation: " + QString::number(rAngle);
    QToolTip::showText(p, tooltip);
}

Cropper::Cropper(WorkArea *wa) :
    width(55), height(55), RotateAngle(0), WA(wa), anchored(false)
{
    setFlag(ItemSendsGeometryChanges);
    setAcceptDrops(true);
    setCacheMode(DeviceCoordinateCache);
    setFlag(ItemIsSelectable);
    setAcceptHoverEvents(true);


    LSide = new Resizer(this, LEFT);
    QObject::connect(LSide, SIGNAL(sizeChanged(qreal,qreal,Position)),
                     this, SLOT(changeSize(qreal,qreal,Position)));

    RSide = new Resizer(this, RIGHT);
    QObject::connect(RSide, SIGNAL(sizeChanged(qreal,qreal,Position)),
                     this, SLOT(changeSize(qreal,qreal,Position)));

    USide = new Resizer(this, UPPER);
    QObject::connect(USide, SIGNAL(sizeChanged(qreal,qreal,Position)),
                     this, SLOT(changeSize(qreal,qreal,Position)));

    BSide = new Resizer(this, BOTTOM);
    QObject::connect(BSide, SIGNAL(sizeChanged(qreal,qreal,Position)),
                     this, SLOT(changeSize(qreal,qreal,Position)));
    setPos(50,50);
    getSamples();
}

void Cropper::paint(QPainter *painter,
                    const QStyleOptionGraphicsItem *option,
                    QWidget *widget)
{
    QPen pen;
    pen.setWidth(1);

    pen.setColor(QColor(0,0,0,0));
    painter-> setRenderHint (QPainter:: Antialiasing);
    painter->setPen(pen);
    painter->drawRect(boundingRect());
}

QRectF Cropper::boundingRect() const
{
    return QRectF(-width/2, -height/2, width, height);
}

void Cropper::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

}

void Cropper::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)//Left button performs drag and drop
    {
        qreal dx = event->scenePos().x() - event->lastScenePos().x();
        qreal dy = event->scenePos().y() - event->lastScenePos().y();
        moveBy(dx, dy);
    }
    else//Right button performs rotation
    {
        qreal Xcur  = event->pos().x();
        qreal Ycur  = event->pos().y();
        qreal Xprev = event->lastPos().x();
        qreal Yprev = event->lastPos().y();

        qreal d = Xprev*Ycur - Xcur*Yprev;
        qreal a = sqrt(Xprev*Xprev + Yprev*Yprev);
        qreal b = sqrt(Xcur*Xcur + Ycur*Ycur);

        RotateAngle += asin(d / (a * b)) * 180 / PI;
        if(RotateAngle>360) RotateAngle=360;
        else if(RotateAngle<-360) RotateAngle=-360;
        setRotation(RotateAngle);
        showRotationTooltip(event, RotateAngle, AnchoredAngle, anchored);
    }
    getSamples();
}

void Cropper::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if(anchored)
    {
        anchored = false;
        LSide->anchored = false;
        LSide->update();

        RSide->anchored = false;
        RSide->update();

        BSide->anchored = false;
        BSide->update();

        USide->anchored = false;
        USide->update();
    }
    else
    {
        AnchoredAngle = RotateAngle;
        anchored = true;
        LSide->anchored = true;
        LSide->update();

        RSide->anchored = true;
        RSide->update();

        BSide->anchored = true;
        BSide->update();

        USide->anchored = true;
        USide->update();
    }
    showRotationTooltip(event, RotateAngle, AnchoredAngle, anchored);
}

void Cropper::changeSize(qreal dx, qreal dy, Position position)
{
    qreal RadRotateAngle = RotateAngle*PI/180;   
    prepareGeometryChange();
    switch(position)
    {
    case RIGHT:
        if(width>MINWIDTH && width+dx<MINWIDTH)
            dx = MINWIDTH - width - dx;
        else if(width<=MINWIDTH && dx<0)
            dx = 0;

        width += dx;
        moveBy(0.5*dx*cos(RadRotateAngle), 0.5*dx*sin(RadRotateAngle));
        USide->width += dx;
        BSide->width += dx;
        break;

    case LEFT:
        if(width>MINWIDTH && width-dx<MINWIDTH)
            dx =   width - dx - MINWIDTH;
        else if(width<=MINWIDTH && dx>0)
            dx = 0;

        width -= dx;
        moveBy(0.5*dx*cos(RadRotateAngle), 0.5*dx*sin(RadRotateAngle));
        USide->width -= dx;
        BSide->width -= dx;
        break;

    case UPPER:
        if(height>MINHEIGHT && height-dy<MINHEIGHT)
            dy = height - dy - MINHEIGHT;
        else if(height<=MINHEIGHT && dy>0)
            dy = 0;

        height -= dy;
        moveBy(-0.5*dy*sin(RadRotateAngle), 0.5*dy*cos(RadRotateAngle));
        LSide->height -= dy;
        RSide->height -= dy;
        break;

    case BOTTOM:
        if(height>MINHEIGHT && height+dy<MINHEIGHT)
            dy = MINHEIGHT - height - dy;
        else if(height<=MINHEIGHT && dy<0)
            dy = 0;

        height += dy;
        moveBy(-0.5*dy*sin(RadRotateAngle), 0.5*dy*cos(RadRotateAngle));
        LSide->height += dy;
        RSide->height += dy;
        break;
    }
   updateItemsPos();
   getSamples();
}

void Cropper:: wheelEvent(QGraphicsSceneWheelEvent *event)
{
    event->setAccepted( true );
    qint64 numSteps = event->delta()/120;
    RotateAngle += numSteps;
    if(RotateAngle>360) RotateAngle=360;
    else if(RotateAngle<-360) RotateAngle=-360;

    setRotation(RotateAngle);
    showRotationTooltip(event, RotateAngle, AnchoredAngle, anchored);
    getSamples();
}

void Cropper::updateItemsPos()
{
    USide->updatePosition();
    BSide->updatePosition();
    LSide->updatePosition();
    RSide->updatePosition();
    update();
}

void Cropper::getSamples()
{
    ////////////////Extract ROI///////////////////////////
    qreal w = WA->CurPxmp.width();
    qreal h = WA->CurPxmp.height();

    QTransform general = QTransform().rotate(-RotateAngle);
    QPointF p[4];
    p[0] = QPoint(0, 0);
    p[1] = QPointF(w, 0);
    p[2] = QPointF(w, h);
    p[3] = QPointF(0, h);

    QPointF offsGen(0, 0);
    for (int i = 0; i < 4; ++i)
    {
         QPointF tempGen = general.map(p[i]);
         offsGen.setX(qMin(offsGen.x(), tempGen.x()));
         offsGen.setY(qMin(offsGen.y(), tempGen.y()));
    }
    QPointF LTgen = general.map(mapToScene(boundingRect().topLeft())) - offsGen;
    int Xgen = static_cast<int>(LTgen.x());
    int Ygen = static_cast<int>(LTgen.y());


    QPixmap AuxPxmColor = WA->CurPxmp.transformed(general, Qt::SmoothTransformation).
                          copy(Xgen, Ygen, width, height).
                          scaled(WA->SampleWidth, WA->SampleHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ////////////////////////////////////////////////////////////////////////

    //////Create samples preview/////////
    normalize(convertToGray(AuxPxmColor));

    //Flipped
    WA->Flipped = QPixmap::fromImage(WA->Normalized.toImage().mirrored(true,false));
    WA->ui->Flipped->setPixmap(WA->Flipped);

    setLowLumin();
    setHighLumin();
    blur(WA->KernelSize);
    addNoize(WA->MuNoise);
    leftDark();
    rightDark();
}

QPixmap Cropper::convertToGray(QPixmap &pxmp)
{
    QImage image = pxmp.toImage();
    QRgb col;
    int gray;
    int width  = pxmp.width();
    int height = pxmp.height();
    for (int i = 0; i < width; ++i)
    {
       for (int j = 0; j < height; ++j)
       {
           col = image.pixel(i, j);
           gray = qGray(col);
           image.setPixel(i, j, qRgb(gray, gray, gray));
       }
    }
   return QPixmap::fromImage(image);
}

void Cropper::normalize(QPixmap src)
{
    WA->Normalized = equalizeHistogram(src, 0, src.width(), 255);
    WA->ui->Normalized->setPixmap(WA->Normalized);
}

QPixmap Cropper::adjustLuminance(QPixmap &pxm, qreal contrast, qreal bright)
{
    QImage img = pxm.toImage();
    int imgW = img.width();
    int imgH = img.height();
    for(int i=0; i<imgW; i++)
        for(int j=0; j<imgH; j++)
        {
           int pxl = contrast * qGray(img.pixel(i, j)) + bright;
           if(pxl>255)    pxl=255;
           else if(pxl<0) pxl=0;
           img.setPixel(i, j, qRgb(pxl, pxl, pxl));
        }
    return QPixmap::fromImage(img);
}

void Cropper::setHighLumin()
{
    WA->HighLuminance = adjustLuminance(WA->Normalized,
                                        WA->ContrastIncreaseRate,
                                        WA->BrightIncreaseValue);
    WA->ui->HighLum->setPixmap(WA->HighLuminance);
}

void Cropper::setLowLumin()
{
    WA->LowLuminance = adjustLuminance(WA->Normalized,
                                       WA->ContrastDecreaseRate,
                                       -WA->BrightDecreaseValue);
    WA->ui->Lowlum->setPixmap(WA->LowLuminance);
}

void Cropper::changeLuminance(Luminance Lumin)
{
    switch(Lumin)
    {
    case LOW:
        setLowLumin();
        break;

    case HIGH:
        setHighLumin();
        break;
    }
}

void Cropper::blur(int KernelSize)
{
    QImage img = WA->Normalized.toImage();
    qt_blurImage(img, KernelSize, true);
    WA->Blurred = QPixmap::fromImage(img);
    WA->ui->Blurred->setPixmap(WA->Blurred);
}

void Cropper::addNoize(double mu)
{
    QImage img = WA->Normalized.toImage();
    int imgW = img.width();
    int imgH = img.height();

    int pxlNew;
    for(int i=0; i<imgW; i++)
        for(int j=0; j<imgH; j++)
        {
            pxlNew = qGray(img.pixel(i, j))*(1+dRand(-mu, mu));
            pxlNew = (pxlNew>255)? 255 : pxlNew;
            img.setPixel(i, j, qRgb(pxlNew, pxlNew, pxlNew));
        }
    WA->Noise = QPixmap::fromImage(img);
    WA->ui->Noise->setPixmap(WA->Noise);
}

void Cropper::leftDark()
{
    QPixmap pxm = WA->Normalized;
    int dark = static_cast<int>(256*WA->LeftDarkCoef);
    WA->LeftDark = equalizeHistogram(pxm, 0, 0.5*pxm.width(), dark);
    WA->ui->LeftDark->setPixmap(WA->LeftDark);
}

void Cropper::rightDark()
{
    QPixmap pxm = WA->Normalized;
    int w = pxm.width();
    int dark = static_cast<int>(256*WA->RightDarkCoef);
    WA->RightDark = equalizeHistogram(pxm, 0.5*w, w, dark);
    WA->ui->RightDark->setPixmap(WA->RightDark);
}



