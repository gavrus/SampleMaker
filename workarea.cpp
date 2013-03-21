#include "workarea.hpp"

void save(QPixmap &pxm, QDir &TargetDir,
          QStringList &TargetFilesList, bool flip=false)
{
    QImage img;
        img = (flip)? pxm.toImage().mirrored(true,false).convertToFormat(QImage::Format_RGB32).
                    convertToFormat(QImage::Format_Indexed8) :
                      pxm.toImage().convertToFormat(QImage::Format_RGB32).
                    convertToFormat(QImage::Format_Indexed8);

    TargetDir.setPath(TargetDir.path());
    TargetFilesList = TargetDir.entryList();
    QString savepath = TargetDir.path() + QDir::separator();
    QString filename=QString::number(TargetFilesList.size()+1)+".jpg";
    img.save(savepath+filename);
}

WorkArea::WorkArea(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::WorkArea),
    SampleWidth(70), SampleHeight(70),
    ContrastDecreaseRate(0.4), BrightDecreaseValue(50),
    ContrastIncreaseRate(2.2), BrightIncreaseValue(50),
    KernelSize(15), MuNoise(0.8), LeftDarkCoef(0.3), RightDarkCoef(0.3),
    flipped(true), lowlum(true), highlum(true),
    blurred(true), noised(true), leftdark(true), rightdark(true)

{
    ui->setupUi(this);
    ui->scrollArea->setVisible(false);
    SourceDir.setFilter(QDir::Files | QDir::NoSymLinks);
    TargetDir.setFilter(QDir::Files | QDir::NoSymLinks);
}

WorkArea::~WorkArea()
{
    delete ui;
}

void WorkArea::setSrcDir(QString &SrcFolder)
{
    SourceDir.setPath(SrcFolder);
    SourceFilesList = SourceDir.entryList();
}

void WorkArea::setTargDir(QString &TargFolder)
{
    TargetDir.setPath(TargFolder);
    TargetFilesList = TargetDir.entryList();
}

void WorkArea::createScene()
{
    photoPxmp.load(SourceDir.path() + QDir::separator() +
                   SourceFilesList.at(CurNumImg));

    CurPxmp.load(SourceDir.path() + QDir::separator() +
                SourceFilesList.at(CurNumImg));

    Cropper *crp = new Cropper(this);
    QObject::connect(this, SIGNAL(sampleWidthChanged()), crp, SLOT(getSamples()));
    QObject::connect(this, SIGNAL(sampleHeightChanged()), crp, SLOT(getSamples()));
    QObject::connect(this, SIGNAL(luminanceChanged(Luminance)), crp, SLOT(changeLuminance(Luminance)));
    QObject::connect(this, SIGNAL(kernelSzChanged(int)), crp, SLOT(blur(int)));
    QObject::connect(this, SIGNAL(muNoizeChanged(double)), crp, SLOT(addNoize(double)));
    QObject::connect(this, SIGNAL(leftDarkCoefChanged()), crp, SLOT(leftDark()));
    QObject::connect(this, SIGNAL(rightDarkCoefChanged()), crp, SLOT(rightDark()));

    ImagePixmap *impm = new ImagePixmap(photoPxmp, this, crp);
    scene->addItem(impm);
    scene->addItem(crp);
    scene->update();
}

void WorkArea::showFirstImg()
{
    ui->ThumbLefttBtn->setEnabled(true);
    ui->ThumbRightBtn->setEnabled(true);
    ui->CurImgNumEdit->setEnabled(true);
    ui->SampOptBtn->setEnabled(true);
    ui->SaveOptBtn->setEnabled(true);
    ui->SaveBtn->setEnabled(true);
    ui->scrollArea->setVisible(true);

    CurNumImg = 0;
    scene = new QGraphicsScene;
    ui->Viewer->setScene(scene);
    createScene();

    QIntValidator *Validator = new QIntValidator(1, SourceFilesList.size(), this);
    ui->CurImgNumEdit->setValidator(Validator);
    ui->CurImgNumEdit->setText(QString::number(CurNumImg+1)+" / "+
                               QString::number(SourceFilesList.size()));
}

void WorkArea::on_ThumbRightBtn_clicked()
{
    CurNumImg = (CurNumImg==SourceFilesList.size()-1)? 0 : CurNumImg+1;
    scene->clear();
    createScene();
    ui->CurImgNumEdit->setText(QString::number(CurNumImg+1)+" / "+
                               QString::number(SourceFilesList.size()));
}

void WorkArea::on_ThumbLefttBtn_clicked()
{
    CurNumImg = (CurNumImg==0)? SourceFilesList.size()-1 : CurNumImg-1;
    scene->clear();
    createScene();
    ui->CurImgNumEdit->setText(QString::number(CurNumImg+1)+" / "+
                               QString::number(SourceFilesList.size()));
}

void WorkArea::on_CurImgNumEdit_editingFinished()
{
    CurNumImg = ui->CurImgNumEdit->text().toInt()-1;
    scene->clear();
    createScene();
    ui->CurImgNumEdit->clearFocus();
    ui->CurImgNumEdit->setText(QString::number(CurNumImg+1)+" / "+
                               QString::number(SourceFilesList.size()));
}

void WorkArea::setSampleWidth(int w)
{
    SampleWidth = w;
    emit sampleWidthChanged();
}

void WorkArea::setSampleHeight(int h)
{
    SampleHeight = h;
    emit sampleHeightChanged();
}

void WorkArea::setContrastDecreaseRate(double Rate)
{
    ContrastDecreaseRate = Rate;
    emit luminanceChanged(LOW);
}

void WorkArea::setBrighDecreaseValue(int Value)
{
    BrightDecreaseValue = Value;
    emit luminanceChanged(LOW);
}

void WorkArea::setContrastIncreaseRate(double Rate)
{
    ContrastIncreaseRate = Rate;
    emit luminanceChanged(HIGH);
}

void WorkArea::setBrighIncreaseValue(int Value)
{
    BrightIncreaseValue = Value;
    emit luminanceChanged(HIGH);
}

void WorkArea::setKernelSize(int Size)
{
    KernelSize = Size;
    emit kernelSzChanged(KernelSize);
}

void WorkArea::setMuNoize(double Mu)
{
    MuNoise = Mu;
    emit muNoizeChanged(MuNoise);
}

void WorkArea::setLeftDarkCoef(double Coef)
{
    LeftDarkCoef = Coef;
    emit leftDarkCoefChanged();
}

void WorkArea::setRightDarkCoef(double Coef)
{
    RightDarkCoef = Coef;
    emit rightDarkCoefChanged();
}

void WorkArea::setSaveOptions(bool flipped_, bool lowlum_, bool highlum_,
                              bool blurred_, bool noised_, bool leftdark_, bool rightdark_)
{
    flipped   = flipped_;
    lowlum    = lowlum_;
    highlum   = highlum_;
    blurred   = blurred_;
    noised    = noised_;
    leftdark  = leftdark_;
    rightdark = rightdark_;
}

void WorkArea::on_DirBtn_clicked()
{
    DirectoryDialog *DirDial = new DirectoryDialog(this);
    DirDial->setAttribute(Qt::WA_DeleteOnClose, true);

    QObject::connect(DirDial, SIGNAL(informSrcDir(QString&)),
                     SLOT(setSrcDir(QString&)));

    QObject::connect(DirDial, SIGNAL(informTargDir(QString&)),
                     SLOT(setTargDir(QString&)));

    QObject::connect(DirDial, SIGNAL(accepted()), SLOT(showFirstImg()));

    DirDial->show();
}

void WorkArea::on_SampOptBtn_clicked()
{
    SamplingOptions *SampOpt = new SamplingOptions(this);
    SampOpt->setAttribute(Qt::WA_DeleteOnClose, true);

    SampOpt->ui->widthChanger->setValue(SampleWidth);
    SampOpt->ui->heightChanger->setValue(SampleHeight);
    SampOpt->ui->ContrDecrChanger->setValue(ContrastDecreaseRate);
    SampOpt->ui->BrightDecrChanger->setValue(BrightDecreaseValue);
    SampOpt->ui->ContrIncrChanger->setValue(ContrastIncreaseRate);
    SampOpt->ui->BrightIncrChanger->setValue(BrightIncreaseValue);
    SampOpt->ui->KernelSizeChanger->setValue(KernelSize);
    SampOpt->ui->NoizeChanger->setValue(MuNoise);
    SampOpt->ui->LeftDarkChanger->setValue(LeftDarkCoef);
    SampOpt->ui->RightDarkChanger->setValue(RightDarkCoef);

    QObject::connect(SampOpt->ui->widthChanger, SIGNAL(valueChanged(int)), this, SLOT(setSampleWidth(int)));
    QObject::connect(SampOpt->ui->heightChanger, SIGNAL(valueChanged(int)), this, SLOT(setSampleHeight(int)));
    QObject::connect(SampOpt->ui->ContrDecrChanger, SIGNAL(valueChanged(double)), this, SLOT(setContrastDecreaseRate(double)));
    QObject::connect(SampOpt->ui->BrightDecrChanger, SIGNAL(valueChanged(int)), this, SLOT(setBrighDecreaseValue(int)));
    QObject::connect(SampOpt->ui->ContrIncrChanger, SIGNAL(valueChanged(double)), this, SLOT(setContrastIncreaseRate(double)));
    QObject::connect(SampOpt->ui->BrightIncrChanger, SIGNAL(valueChanged(int)), this, SLOT(setBrighIncreaseValue(int)));
    QObject::connect(SampOpt->ui->KernelSizeChanger, SIGNAL(valueChanged(int)), this, SLOT(setKernelSize(int)));
    QObject::connect(SampOpt->ui->NoizeChanger, SIGNAL(valueChanged(double)), this, SLOT(setMuNoize(double)));
    QObject::connect(SampOpt->ui->LeftDarkChanger, SIGNAL(valueChanged(double)), this, SLOT(setLeftDarkCoef(double)));
    QObject::connect(SampOpt->ui->RightDarkChanger, SIGNAL(valueChanged(double)), this, SLOT(setRightDarkCoef(double)));
    SampOpt->show();
}

void WorkArea::on_SaveOptBtn_clicked()
{
    SaveOptions *SaveOpt = new SaveOptions(this);
    SaveOpt->setAttribute(Qt::WA_DeleteOnClose, true);
    SaveOpt->ui->blurred->setChecked(blurred);
    SaveOpt->ui->flipped->setChecked(flipped);
    SaveOpt->ui->highlum->setChecked(highlum);
    SaveOpt->ui->leftdark->setChecked(leftdark);
    SaveOpt->ui->lowlum->setChecked(lowlum);
    SaveOpt->ui->noised->setChecked(noised);
    SaveOpt->ui->rightdark->setChecked(rightdark);

    QObject::connect(SaveOpt, SIGNAL(newSaveOptions(bool,bool,bool,bool,bool,bool,bool)),
                     this, SLOT(setSaveOptions(bool,bool,bool,bool,bool,bool,bool)));
    SaveOpt->show();
}

void WorkArea::on_SaveBtn_clicked()
{
    save(Normalized, TargetDir, TargetFilesList);
    if(flipped)
        save(Flipped, TargetDir, TargetFilesList);

    if(lowlum)
    {
        save(LowLuminance, TargetDir, TargetFilesList);
        if(flipped)
            save(LowLuminance, TargetDir, TargetFilesList, true);
    }
    if(highlum)
    {
        save(HighLuminance, TargetDir, TargetFilesList);
        if(flipped)
            save(HighLuminance, TargetDir, TargetFilesList, true);
    }
    if(blurred)
    {
        save(Blurred, TargetDir, TargetFilesList);
        if(flipped)
            save(Blurred, TargetDir, TargetFilesList, true);
    }
    if(noised)
    {
        save(Noise, TargetDir, TargetFilesList);
        if(flipped)
            save(Noise, TargetDir, TargetFilesList, true);
    }
    if(leftdark)
    {
        save(LeftDark, TargetDir, TargetFilesList);
        if(flipped)
            save(LeftDark, TargetDir, TargetFilesList, true);
    }
    if(rightdark)
    {
        save(RightDark, TargetDir, TargetFilesList);
        if(flipped)
            save(RightDark, TargetDir, TargetFilesList, true);
    }
}

void WorkArea::on_ExitBtn_clicked()
{
    close();
}



