#include "selectip.h"
#include "ui_selectip.h"

selectip::selectip(QWidget *parent,winpcap *tem) :
    QWidget(parent),
    ui(new Ui::selectip)
{
    ui->setupUi(this);
    if(tem == NULL)
        exit(1);
    arp = tem;
    ser = NULL;
    cli = NULL;
//    ser = new server(0,arp);
//    cli = new client(0,arp);
    updateipcombox();
}

selectip::~selectip()
{
    delete ui;
    delete arp;
}

void selectip::updateipcombox()
{
    QList<QString> tem = arp->getusableip();
    if(tem.length() == 0)
        exit(1);
    ui->ipcomboBox->insertItems(0,tem);
}


void selectip::on_pushButton_clicked()
{
    arp->getactmac();
}

void selectip::on_ipcomboBox_currentIndexChanged(const QString &arg1)
{
    ui->dev_tip->setText(QString("网络检测中"));
    qDebug()<<"ip changged";
    arp->newhandle(arg1);

    qDebug()<<arp->getselfmac().length();
    QString mac = QString("子网掩码：%1\nmac地址：%2").arg(arp->getnetmask()).arg(arp->macos());
    ui->dev_tip->setText(mac);
}

void selectip::on_pushButton_3_clicked()
{
    qDebug()<<(void *)ser;
    if(ser == NULL)
        ser = new server(0,arp);
    if(ser->isHidden())
        ser->show();
}

void selectip::on_pushButton_2_clicked()
{
//    if(cli == NULL)
//        cli = new client(0,arp);
//    if(cli->isHidden())
//        cli->show();
    cli = new client(0,arp);
    cli->show();
}
