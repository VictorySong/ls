#include <stable.h>
#include "mainwindow.h"

//#include "arpactdev.h"
#include "winpcap.h"
#include "selectip.h"
#include "server.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    winpcap *winpcap_t = new winpcap();
    winpcap_t->init();
    selectip se(0,winpcap_t);
    se.setWindowTitle(QString("选择要用的ip地址"));
    se.show();

//    arpactdev arp;
//    arp.show();


//    MainWindow w;
//    w.show();

    return a.exec();

}
