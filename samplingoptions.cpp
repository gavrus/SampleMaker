#include "samplingoptions.hpp"

SamplingOptions::SamplingOptions(QWidget *parent) : QDialog(parent),
    ui(new Ui::SamplingOptions)
{
    ui->setupUi(this);
}

SamplingOptions::~SamplingOptions()
{
    delete ui;
}


void SamplingOptions::on_OkBtn_clicked()
{
    close();
}
