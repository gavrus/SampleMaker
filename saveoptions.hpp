#ifndef SAVEOPTIONS_HPP
#define SAVEOPTIONS_HPP

#include <QDialog>
#include "ui_saveoptions.h"

class SaveOptions : public QDialog
{
    Q_OBJECT
    
public:
    explicit SaveOptions(QWidget *parent = 0);
    ~SaveOptions();

    Ui::SaveOptions *ui;
    
signals:
    void newSaveOptions(bool,bool,bool,bool,bool,bool,bool);

private slots:
    void on_OkBtn_clicked();
};

#endif // SAVEOPTIONS_HPP
