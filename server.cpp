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

    //获取局域网广播地址
    char ip[16];
    char netmask[16];
    arp->getip(ip);
    arp->getnetmask(netmask);
    unsigned long myip = inet_addr(ip);
    unsigned long mynetmask = inet_addr(netmask);
    unsigned long toip = htonl((myip & mynetmask));
    unsigned long num = htonl(inet_addr("255.255.255.255")-mynetmask);
    toip += num;
    toip = htonl(toip);
    ui->multicastip->setText(arp->iptos(toip));         //设置udp广播地址

    //加入广播组
    udpsender->joinMulticastGroup(QHostAddress(ui->multicastip->text()));
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

    pix = QPixmap(300,200);         //设置画布大小
    pix.fill(Qt::white);
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
    if(QMessageBox::Yes == QMessageBox::question(this,
                                                 tr("Question"),
                                                 tr("Are you OK?"),
                                                 QMessageBox::Yes | QMessageBox::No,
                                                 QMessageBox::Yes)){

    }

}

void server::on_pushButton_2_clicked()
{

}

void server::updatetabelwidget(QByteArray mess, tcpsocket * clientsocket)
{
    qDebug()<<"有新数据";
    QJsonParseError error;
    QJsonDocument jsondoc = QJsonDocument::fromJson(mess,&error);       //转化成json对象
    qDebug()<<error.errorString();
    QVariantMap result = jsondoc.toVariant().toMap();
    if(result.contains(QString("x"))){
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
        //画图并更改卫星实时位置
        QHashIterator <QString,tcpsocket *> i(this->tcpServer->tcpClientSocketList);
        while(i.hasNext()){
            i.next();
            if(i.value()->peerAddress().toString() == clientsocket->peerAddress().toString()
                    && i.value()->peerPort() == clientsocket->peerPort()){
                inf pretem;         //前一个位置结构
                pretem = locationlist.value(i.key());
                inf tem;            //现在的位置结构
                tem.x = result["x"].toFloat();
                tem.y = result["y"].toFloat();
                locationlist.insert(i.key(),tem);
                //设置起始点
                lastpoint.setX(pretem.x);
                lastpoint.setY(pretem.y);
                endpoint.setX(tem.x);
                endpoint.setY(tem.y);
                this->update();
                break;
            }
        }

    }else{
        qDebug()<<"收到的信息不是位置信息";
        qDebug()<<mess;
    }
}

void server::updatenewclient(tcpsocket * clientsocket)
{
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->setRowCount(row+1);
    ui->tableWidget->setItem(row,0,new QTableWidgetItem(clientsocket->peerAddress().toString()));       //更新新连接ip
    ui->tableWidget->setItem(row,1,new QTableWidgetItem(QString("%1").arg(clientsocket->peerPort())));      //更新新连接端口

    //添加卫星实时位置
    QHashIterator <QString,tcpsocket *> i(this->tcpServer->tcpClientSocketList);
    while(i.hasNext()){
        i.next();
        if(i.value()->peerAddress().toString() == clientsocket->peerAddress().toString()
                && i.value()->peerPort() == clientsocket->peerPort()){
            inf tem;
            locationlist.insert(i.key(),tem);
            qDebug()<<tem.x;
            break;
        }
    }
    qDebug()<<locationlist.keys();
}

void server::disconnected(tcpsocket *clientsocket)
{
    qDebug()<<"lianjieyiduankaifjkfjsdkfl";
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
//    free(clientsocket);                    //释放这个不用的连接的内存  此处要用free 不能用delete 会出错
}

void server::paintEvent(QPaintEvent *)
{
    QPainter pp(&pix);    // 根据鼠标指针前后两个位置就行绘制直线
    pp.drawLine(lastpoint,endpoint);    // 让前一个坐标值等于后一个坐标值，这样就能实现画出连续的线
    QPainter painter(ui->locateopenGLWidget);
    painter.drawPixmap(0, 0, pix);
}
