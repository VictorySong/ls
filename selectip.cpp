#include "selectip.h"
#include "ui_selectip.h"

extern QString ip;                 //ip
extern QString broadcast;                  //广播地址
selectip::selectip(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::selectip)
{
    setAttribute(Qt::WA_DeleteOnClose);             //关闭窗口后调用析构函数
    ui->setupUi(this);

    ser = NULL;

    wifi = new searchwifi();            //搜索wifi的线程
    connect(wifi,SIGNAL(wificonnected()),this,SLOT(wificonnected()));
    ui->pushButton_2->hide();               //在未连接到指定wifi的情况下不现实按钮
    ui->pushButton_3->hide();               //在未连接到指定wifi的情况下不现实按钮
    wifi->start();

}

selectip::~selectip()
{
    wifi->terminate();
    wifi->wait();
    delete wifi;
    if(ser)
        ser->close();

    for(int i = 0;i<cli.count();i++){
        if(cli.at(i))
            cli.at(i)->close();
    }
    exit(0);
}

void selectip::updateipcombox()
{
    usableip.clear();       //清楚可用ip列表
    ui->ipcomboBox->clear();    //清楚可用ip显示
    //获取所有网络接口的列表
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
    foreach(QNetworkInterface interface,list) //遍历每一个网络接口
    {
        qDebug() << "Device: "<<interface.name(); //设备名
        //硬件地址
        qDebug() << "HardwareAddress: "<<interface.hardwareAddress();

        //获取IP地址条目列表，每个条目中包含一个IP地址，
        //一个子网掩码和一个广播地址
       QList<QNetworkAddressEntry> entryList= interface.addressEntries();
        foreach(QNetworkAddressEntry entry,entryList)//遍历每个IP地址条目
        {
            qDebug()<<"IP Address: "<<entry.ip().toString(); //IP地址
            qDebug()<<"Netmask: "<<entry.netmask().toString(); //子网掩码
            qDebug()<<"Broadcast: "<<entry.broadcast().toString();//广播地址
            if(entry.broadcast().toString() != QString("")){
                usableip.insert(entry.ip().toString(),entry.broadcast().toString());
            }
        }
    }
    if(0 == usableip.count()){
        QMessageBox::information(this,QString("提示："),QString("无可用ip，程序即将退出！"));
        exit(1);
    }

    QList<QString> tem ;
    QMapIterator<QString,QString> i(usableip);
    while(i.hasNext()){
        i.next();
        tem.append(i.key());
    }
    ui->ipcomboBox->insertItems(0,tem);
}



void selectip::on_ipcomboBox_currentIndexChanged(const QString &arg1)
{
    ip = arg1;
    broadcast = usableip.value(ip);

    //ip地址发生变化时，小卫星和中控监听的地址也要发生变化
    if(NULL != ser)
        ser->wificonnected();
    for(int i = 0;i<cli.count();i++){
        if(NULL != cli.at(i)){
            cli.at(i)->wificonnected();
        }
    }
}

void selectip::on_pushButton_3_clicked()
{
    if(NULL == ser){
        ser = new server(0);
        ser->setWindowTitle(QString("中控"));
        connect(ser,SIGNAL(destroyed(QObject*)),this,SLOT(newser(QObject*)));  //服务器窗口摧毁后进行
    }
    if(ser->isHidden())
        ser->show();
}

void selectip::on_pushButton_2_clicked()
{
    client *tem = new client(0);
    cli.append(tem);
    tem->setWindowTitle(QString("小卫星"));
    connect(tem,SIGNAL(destroyed(QObject*)),this,SLOT(destoryclient(QObject*)));  //服务器窗口摧毁后进行
    tem->show();
}

void selectip::newser(QObject *)
{
    ser = NULL;
}

void selectip::destoryclient(QObject *tem)
{
    for(int i = 0;i<cli.count();i++){
        if(cli.at(i) == tem){
            cli.replace(i,NULL);
        }
    }
}

void selectip::wificonnected()
{
    ui->pushButton_2->show();
    ui->pushButton_3->show();
    updateipcombox();
}

