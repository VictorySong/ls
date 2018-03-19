#include <stable.h>
#include "mainwindow.h"

#include "arpactdev.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    arpactdev arp;
    arp.show();


//    MainWindow w;
//    w.show();

    return a.exec();

}
