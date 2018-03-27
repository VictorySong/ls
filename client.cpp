#include "client.h"
#include "ui_client.h"

client::client(QWidget *parent ,winpcap *tem) :
    QWidget(parent),
    ui(new Ui::client)
{
    setAttribute(Qt::WA_DeleteOnClose);             //关闭窗口后调用析构函数
    ui->setupUi(this);
    if(NULL == tem)
        exit(1);
    arp = tem;
    udpServer = new QUdpSocket(this);                   //实例化udpsocket对象
    tcpsender = new tcpsocket(this);
    if(!udpServer->bind(QHostAddress(arp->getip()),ui->udpport->text().toInt(),QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint))
        qDebug()<<"udp接受端绑定端口出错";
    udpServer->joinMulticastGroup(QHostAddress(ui->multicastip->text()));
    connect(udpServer,SIGNAL(readyRead()),this,SLOT(udpget()));
    connect(tcpsender,SIGNAL(connected()),this,SLOT(tcpconnected()));
}

client::~client()
{
    qDebug()<<"调用客户端析构函数";
    delete ui;
    delete udpServer;
    delete tcpsender;
}

void client::udpget()
{
    while(udpServer->hasPendingDatagrams())  //拥有等待的数据报
    {
       QByteArray datagram; //拥于存放接收的数据报
        //让datagram的大小为等待处理的数据报的大小，这样才能接收到完整的数据
       datagram.resize(udpServer->pendingDatagramSize());
       //接收数据报，将其存放到datagram中
       udpServer->readDatagram(datagram.data(),datagram.size());
       //将数据报内容显示出来
       QJsonParseError error;
       QJsonDocument jsondoc = QJsonDocument::fromJson(datagram,&error);
       QVariantMap result = jsondoc.toVariant().toMap();
       ui->serverip->setText(result["ip"].toString());
       ui->serverport->setText(result["port"].toString());
    }
}

void client::on_pushButton_clicked()
{
    tcpsender->abort(); //取消已有的连接
    ui->pushButton->setText(QString("连接中"));
    //连接到主机，这里从界面获取主机地址和端口号
    tcpsender->connectToHost(QHostAddress(ui->serverip->text()),ui->serverport->text().toInt());
}

void client::on_pushButton_2_clicked()
{
    qDebug()<<tcpsender->peerAddress();
    qDebug()<<tcpsender->peerPort();

//    qDebug()<<tcpsender->localAddress();
//    qDebug()<<tcpsender->localPort();
}

void client::tcpconnected()
{
    ui->pushButton->setText(QString("已连接"));
}

void client::on_pushButton_3_clicked()
{
    //发送位置信息
    QJsonObject json;
    json.insert("x",ui->x->text().toFloat());
    json.insert("y",ui->y->text().toFloat());
    QJsonDocument document;
    document.setObject(json);
    QByteArray datagram = document.toJson(QJsonDocument::Compact);  //将位置信息转化为json格式
    QByteArray block;
    //使用数据流写入数据
    QDataStream out(&block,QIODevice::WriteOnly);
    //设置数据流的版本，客户端和服务器端使用的版本要相同
    out.setVersion(QDataStream::Qt_4_6);
    out<<datagram;
    tcpsender->write(block);
}
