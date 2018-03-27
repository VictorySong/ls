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

}

void server::on_pushButton_2_clicked()
{
    if(!this->tcpServer->isListening()){
        if(!this->tcpServer->listen(QHostAddress(arp->getip()),ui->tcpport->text().toInt()))
        {
            qDebug() << this->tcpServer->errorString();
        }else{
            qDebug()<<this->tcpServer->serverAddress();
            qDebug()<<this->tcpServer->serverPort();
        }
        ui->pushButton_2->setText(QString("正在监听，点击关闭"));
    }else{
        this->tcpServer->close();   //如果正在监听则关闭
        ui->pushButton_2->setText(QString("开始监听"));
    }
}


