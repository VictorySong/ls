#include <stable.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    pcap_if_t *alldevs;
        pcap_if_t *d;
        int i = 0;
        char errbuf[PCAP_ERRBUF_SIZE];
        if(pcap_findalldevs(&alldevs, errbuf) == -1)
        {
            qDebug() << errbuf;
        }
        for(d = alldevs; d; d = d->next)
        {
            qDebug() << ++i << d->name;
            if(d->description)
                qDebug() << d->description;
            else
                qDebug("(No description available)");
        }
        if(0 == i)
        {
            qDebug("No interfaces found! Make sure WinPcap is installed.");
        }
        pcap_freealldevs(alldevs);


    /*
    MainWindow w;
    w.show();
    */
    return a.exec();

}
