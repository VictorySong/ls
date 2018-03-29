#include "selectip.h"
#include "ui_selectip.h"

selectip::selectip(QWidget *parent,winpcap *tem) :
    QWidget(parent),
    ui(new Ui::selectip)
{
    setAttribute(Qt::WA_DeleteOnClose);             //关闭窗口后调用析构函数
    ui->setupUi(this);
    ui->progressBar->hide();                //进度条隐藏

    if(tem == NULL)
        exit(1);
    arp = tem;
    ser = NULL;
    cli = NULL;
//    ser = new server(0,arp);
//    cli = new client(0,arp);
    updateipcombox();       //获得可用ip地址
    //关联活动mac监听进程结束信号，更新活动mac列表
    connect(arp,SIGNAL(updatealivemac()),this,SLOT(updatealivemactable()));
    connect(arp,SIGNAL(setmaxprogressbar(ulong)),this,SLOT(setmaxprogressbar(ulong)));
    connect(arp,SIGNAL(setprogressbar(ulong)),this,SLOT(setprogressbar(ulong)));
}

selectip::~selectip()
{
    delete ui;
    delete arp;
    exit(1);
}

void selectip::updateipcombox()
{
    QList<QString> tem = arp->getusableip();
    if(tem.length() == 0){
        QMessageBox::information(this,QString("提示："),QString("无可用ip，程序即将退出！"));
        exit(1);
    }
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
    if(NULL == ser){
        ser = new server(0,arp);
        ser->setWindowTitle(QString("中控"));
        connect(ser,SIGNAL(destroyed(QObject*)),this,SLOT(newser(QObject*)));  //服务器窗口摧毁后进行
    }
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
    cli->setWindowTitle(QString("小卫星"));
    cli->show();
}

void selectip::updatealivemactable()
{
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);
    QMapIterator<QString,actdevinf> i(arp->actmac);
    int row = ui->tableWidget->rowCount();
    while(i.hasNext()){
        i.next();
        ui->tableWidget->setRowCount(row+1);
        ui->tableWidget->setItem(row,0,new QTableWidgetItem(i.key()));
        ui->tableWidget->setItem(row,1,new QTableWidgetItem(i.value().mac));
        row++;
    }
}

void selectip::newser(QObject *)
{
    ser = NULL;
}

void selectip::setmaxprogressbar(unsigned long t)
{
    ui->progressBar->show();
    ui->progressBar->setMaximum((int)t);
}

void selectip::setprogressbar(unsigned long t)
{
    ui->progressBar->setValue((int)t);
    if(ui->progressBar->maximum() == (int)t)
        ui->progressBar->hide();
}
