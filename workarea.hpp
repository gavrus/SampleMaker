#ifndef WORKAREA_HPP
#define WORKAREA_HPP

#include <QMainWindow>
#include <QDir>
#include <QGraphicsScene>
#include <QIntValidator>
#include <imagepixmap.hpp>
#include <cropper.hpp>
#include <directorydialog.hpp>
#include <samplingoptions.hpp>
#include <defines.hpp>
#include <saveoptions.hpp>
#include "ui_workarea.h"

class ImagePixmap;

class WorkArea : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit WorkArea(QWidget *parent = 0);
    ~WorkArea();

    Ui::WorkArea *ui;

    QDir        SourceDir;
    QDir        TargetDir;
    QStringList SourceFilesList;
    QStringList TargetFilesList;
    QPixmap photoPxmp;
    QPixmap CurPxmp;
    QGraphicsScene *scene;

    //Sample pixmaps
    QPixmap Normalized;
    QPixmap Flipped;
    QPixmap LowLuminance;
    QPixmap HighLuminance;
    QPixmap Blurred;
    QPixmap Noise;
    QPixmap LeftDark;
    QPixmap RightDark;

    int SampleWidth;
    int SampleHeight;

    qreal ContrastDecreaseRate;
    int   BrightDecreaseValue;

    qreal ContrastIncreaseRate;
    int   BrightIncreaseValue;

    int    KernelSize;
    double MuNoise;
    double LeftDarkCoef;
    double RightDarkCoef;

    //Save options
    bool flipped;
    bool lowlum;
    bool highlum;
    bool blurred;
    bool noised;
    bool leftdark;
    bool rightdark;

    void createScene();

signals:
    void sampleWidthChanged();
    void sampleHeightChanged();
    void luminanceChanged(Luminance);
    void kernelSzChanged(int);
    void muNoizeChanged(double);
    void leftDarkCoefChanged();
    void rightDarkCoefChanged();

private slots:
    void setSrcDir(QString &SrcFolder);
    void setTargDir(QString &TargFolder);
    void showFirstImg();
    void setSampleWidth(int w);
    void setSampleHeight(int h);
    void setContrastDecreaseRate(double Rate);
    void setBrighDecreaseValue(int Value);
    void setContrastIncreaseRate(double Rate);
    void setBrighIncreaseValue(int Value);
    void setKernelSize(int Size);
    void setMuNoize(double Mu);
    void setLeftDarkCoef(double Coef);
    void setRightDarkCoef(double Coef);
    void setSaveOptions(bool flipped_,bool lowlum_,bool highlum_,
                        bool blurred_,bool noised_,bool leftdark_, bool rightdark_);
    void on_DirBtn_clicked();
    void on_SampOptBtn_clicked();
    void on_ThumbRightBtn_clicked();
    void on_ThumbLefttBtn_clicked();
    void on_CurImgNumEdit_editingFinished();
    void on_SaveBtn_clicked();
    void on_ExitBtn_clicked();
    void on_SaveOptBtn_clicked();

private:
    int CurNumImg;
};

#endif // WORKAREA_HPP




