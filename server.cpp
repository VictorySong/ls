#include "server.h"
#include "ui_server.h"

server::server(QWidget *parent,winpcap *tem) :
    QWidget(parent),
    ui(new Ui::server)
{
    setAttribute(Qt::WA_DeleteOnClose);             //关闭窗口后调用析构函数
    ui->setupUi(this);
    if(NULL == tem)
        exit(1);
    arp = tem;
    udpsender = new QUdpSocket(this);                   //实例化udpsender 对象
    tcpServer = new tcpserver(this);                   //实例化tcpserver对象
    udpsender->bind(QHostAddress(arp->getip()),0);

    //获取监听的端口和ip
    QJsonObject json;
    json.insert("ip",arp->getip());
    json.insert("port",ui->tcpport->text().toInt());
    QJsonDocument document;
    document.setObject(json);
    QByteArray datagram = document.toJson(QJsonDocument::Compact);
    //启动udp广播线程
    udpbro = new udpbroad(udpsender,datagram,ui->udpport->text().toInt(),ui->multicastip->text());
    udpbro->start();

    //监听tcp
    if(!this->tcpServer->isListening()){
        if(!this->tcpServer->listen(QHostAddress(arp->getip()),ui->tcpport->text().toInt()))
        {
            qDebug() << this->tcpServer->errorString();
        }else{
            qDebug()<<this->tcpServer->serverAddress();
            qDebug()<<this->tcpServer->serverPort();
        }
        ui->pushButton_2->setText(QString("正在监听"));
    }else{
        this->tcpServer->close();   //如果正在监听则关闭
        ui->pushButton_2->setText(QString("开始监听"));
    }

    //关联新的tcp连接产生与更新界面
    connect(tcpServer,SIGNAL(newclientsocket(tcpsocket*)),this,SLOT(updatenewclient(tcpsocket*)));
    //关联接收数据的信号与更新界面
    connect(tcpServer,SIGNAL(updateServer(QByteArray,tcpsocket*)),this,SLOT(updatetabelwidget(QByteArray,tcpsocket*)));
    //关联连接断开与更新界面
    connect(tcpServer,SIGNAL(disconnected(tcpsocket*)),this,SLOT(disconnected(tcpsocket*)));
}

server::~server()
{
    delete ui;
    delete udpsender;
    delete tcpServer;
    udpbro->terminate();
    udpbro->wait();
    delete udpbro;
}

void server::on_pushButton_clicked()
{
    qDebug()<<ui->tableWidget->rowCount();

}

void server::on_pushButton_2_clicked()
{

}

void server::updatetabelwidget(QByteArray mess, tcpsocket * clientsocket)
{
    QJsonParseError error;
    QJsonDocument jsondoc = QJsonDocument::fromJson(mess,&error);       //转化成json对象
    qDebug()<<error.errorString();
    QVariantMap result = jsondoc.toVariant().toMap();

    int row = ui->tableWidget->rowCount();
    QString ip = clientsocket->peerAddress().toString();
    QString port = QString("%1").arg(clientsocket->peerPort());
    for(int i = 0;i<row;i++){
        if(ui->tableWidget->item(i,0)->text() == ip
           && ui->tableWidget->item(i,1)->text() == port){
            ui->tableWidget->removeCellWidget(i,2);             //先清掉
            ui->tableWidget->removeCellWidget(i,3);             //先清掉
            ui->tableWidget->setItem(i,2,new QTableWidgetItem(result["x"].toString()));       //更新横坐标
            ui->tableWidget->setItem(i,3,new QTableWidgetItem(result["y"].toString()));           //更新纵坐标
            break;
        }
    }
}

void server::updatenewclient(tcpsocket * clientsocket)
{
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->setRowCount(row+1);
    ui->tableWidget->setItem(row,0,new QTableWidgetItem(clientsocket->peerAddress().toString()));       //更新新连接ip
    ui->tableWidget->setItem(row,1,new QTableWidgetItem(QString("%1").arg(clientsocket->peerPort())));      //更新新连接端口

}

void server::disconnected(tcpsocket *clientsocket)
{
    int row = ui->tableWidget->rowCount();
    QString ip = clientsocket->peerAddress().toString();
    QString port = QString("%1").arg(clientsocket->peerPort());
    for(int i = 0;i<row;i++){
        if(ui->tableWidget->item(i,0)->text() == ip
           && ui->tableWidget->item(i,1)->text() == port){
            ui->tableWidget->removeRow(i);
            break;
        }
    }
}
