#include <stable.h>
#include "mainwindow.h"


#include "selectip.h"
#include "server.h"
QString ip;                 //ip
QString broadcast;                  //广播地址

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    selectip se(0);
    se.setWindowTitle(QString("选择要用的ip地址"));
    se.show();

//    arpactdev arp;
//    arp.show();


//    MainWindow w;
//    w.show();

    return a.exec();

}
