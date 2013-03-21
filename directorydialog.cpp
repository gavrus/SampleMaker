#include "directorydialog.hpp"

DirectoryDialog::DirectoryDialog(QWidget *parent) : QDialog(parent),
    ui(new Ui::Dialog), isSrcFoldSet(false), isTargFoldSet(false)
{
    ui->setupUi(this);
}

DirectoryDialog::~DirectoryDialog()
{
    delete ui;
}

void DirectoryDialog::on_SrcBtn_clicked()
{
    QString SrcDir = QFileDialog::getExistingDirectory(this);
    ui->SrcLine->setText(SrcDir);

    emit informSrcDir(SrcDir);

    isSrcFoldSet = true;

    if(isTargFoldSet)
        ui->OkBtn->setEnabled(true);
}

void DirectoryDialog::on_TargBtn_clicked()
{
    QString TargDir = QFileDialog::getExistingDirectory(this);
    ui->TargLine->setText(TargDir);

    emit informTargDir(TargDir);

    isTargFoldSet = true;

    if(isSrcFoldSet)
        ui->OkBtn->setEnabled(true);
}

void DirectoryDialog::on_OkBtn_clicked()
{
    emit accepted();
    close();
}

void DirectoryDialog::on_CancelBtn_clicked()
{
    close();
}
