#ifndef SAMPLINGOPTIONS_HPP
#define SAMPLINGOPTIONS_HPP

#include <QDialog>
#include "ui_samplingoptions.h"


class SamplingOptions : public QDialog
{
    Q_OBJECT
    
public:
    explicit SamplingOptions(QWidget *parent = 0);
    ~SamplingOptions();

    Ui::SamplingOptions *ui;
    
private slots:
    void on_OkBtn_clicked();

};

#endif // SAMPLINGOPTIONS_HPP
