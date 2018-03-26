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
    ui->ipcomboBox->insertItems(0,tem);
}

void selectip::on_pushButton_2_clicked()
{
    arp->newhandle(ui->ipcomboBox->currentText());
    QString mac = QString("子网掩码：%1\nmac地址：%2").arg(arp->getnetmask()).arg(arp->getselfmac());
    ui->dev_tip->setText(mac);
}

void selectip::on_pushButton_clicked()
{

}
