#ifndef DIRECTORYDIALOG_HPP
#define DIRECTORYDIALOG_HPP

#include <QDialog>
#include <QFileDialog>
#include "ui_directorydialog.h"


class DirectoryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DirectoryDialog(QWidget *parent = 0);
    ~DirectoryDialog();

    Ui::Dialog *ui;
    
signals:
    void informSrcDir(QString&);
    void informTargDir(QString&);

private slots:
    void on_OkBtn_clicked();
    void on_SrcBtn_clicked();
    void on_TargBtn_clicked();

    void on_CancelBtn_clicked();

private:
    bool isSrcFoldSet;
    bool isTargFoldSet;
};

#endif // DIRECTORYDIALOG_HPP


