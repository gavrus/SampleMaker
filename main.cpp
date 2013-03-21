#include <QApplication>
#include "workarea.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WorkArea w;
    w.show();
    
    return a.exec();
}
