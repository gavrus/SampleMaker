#include "saveoptions.hpp"

SaveOptions::SaveOptions(QWidget *parent) : QDialog(parent),
    ui(new Ui::SaveOptions)
{
    ui->setupUi(this);
}

SaveOptions::~SaveOptions()
{
    delete ui;
}

void SaveOptions::on_OkBtn_clicked()
{
    emit newSaveOptions(ui->flipped->isChecked(),ui->lowlum->isChecked(),ui->highlum->isChecked(),
                        ui->blurred->isChecked(),ui->noised->isChecked(),
                        ui->leftdark->isChecked(),ui->rightdark->isChecked());
    close();
}
