#include "client.h"
#include "ui_client.h"

extern QString ip;                 //ip
extern QString broadcast;                  //广播地址

client::client(QWidget *parent ) :
    QWidget(parent),
    ui(new Ui::client)
{
    setAttribute(Qt::WA_DeleteOnClose);             //关闭窗口后调用析构函数
    ui->setupUi(this);

    socketinit();
}

client::~client()
{
    qDebug()<<"调用客户端析构函数";
    delete udpServer;
    delete tcpsender;
}

void client::udpget()
{
    if(udpServer->hasPendingDatagrams())  //拥有等待的数据报
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

       //进行tcp连接
       tcpsender->abort(); //取消已有的连接
       ui->pushButton->setText(QString("连接中"));
       //连接到主机，这里从界面获取主机地址和端口号
       tcpsender->connectToHost(QHostAddress(ui->serverip->text()),ui->serverport->text().toInt());

       //关闭udpserver
       udpServer->close();
    }
}

void client::on_pushButton_clicked()
{
    if(ui->serverip->text().length() == 0)
        return;         //没有服务器ip 所以不发起连接
    tcpsender->abort(); //取消已有的连接
    ui->pushButton->setText(QString("连接中"));
    //连接到主机，这里从界面获取主机地址和端口号
    tcpsender->connectToHost(QHostAddress(ui->serverip->text()),ui->serverport->text().toInt());
}

void client::on_pushButton_2_clicked()
{
    tcpsender->abort(); //取消已有的连接
    ui->pushButton->setText(QString("发起tcp连接"));
}

void client::tcpconnected()
{
    ui->pushButton->setText(QString("已连接"));
    ui->localip->setText(tcpsender->localAddress().toString());         //显示当前ip
    ui->localport->setText(QString("%1").arg(tcpsender->localPort()));      //显示当前端口
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

    qDebug()<<datagram;

    if(tcpsender->isWritable())
        tcpsender->write(datagram);
}

void client::newdata(QByteArray mess, tcpsocket *clientsocket)
{
    QJsonParseError error;
    QJsonDocument jsondoc = QJsonDocument::fromJson(mess,&error);       //转化成json对象
    QVariantMap result = jsondoc.toVariant().toMap();
    if(result.contains(QString("x"))){
        int row = ui->tableWidget->rowCount();
        if(0 ==row){
            ui->tableWidget->setRowCount(row+1);
            ui->tableWidget->setItem(row,0,new QTableWidgetItem(result["id"].toString()));
            ui->tableWidget->setItem(row,1,new QTableWidgetItem(result["ip"].toString()));
            ui->tableWidget->setItem(row,2,new QTableWidgetItem(result["port"].toString()));
            ui->tableWidget->setItem(row,3,new QTableWidgetItem(result["x"].toString()));
            ui->tableWidget->setItem(row,4,new QTableWidgetItem(result["y"].toString()));
        }else{
            int i ;
            for( i = 0;i<row;i++){
                if(ui->tableWidget->item(i,0)->text() ==result["id"].toString()){
                    ui->tableWidget->removeCellWidget(i,1);             //先清掉
                    ui->tableWidget->removeCellWidget(i,2);             //先清掉
                    ui->tableWidget->removeCellWidget(i,3);             //先清掉
                    ui->tableWidget->removeCellWidget(i,4);             //先清掉
                    ui->tableWidget->setItem(row,1,new QTableWidgetItem(result["ip"].toString()));  //更新ip
                    ui->tableWidget->setItem(row,2,new QTableWidgetItem(result["port"].toString()));  //更新端口
                    ui->tableWidget->setItem(i,3,new QTableWidgetItem(result["x"].toString()));       //更新横坐标
                    ui->tableWidget->setItem(i,4,new QTableWidgetItem(result["y"].toString()));           //更新纵坐标
                    break;
                }
            }
            if(i == row){
                ui->tableWidget->setRowCount(row+1);
                ui->tableWidget->setItem(row,0,new QTableWidgetItem(result["id"].toString()));
                ui->tableWidget->setItem(row,1,new QTableWidgetItem(result["ip"].toString()));
                ui->tableWidget->setItem(row,2,new QTableWidgetItem(result["port"].toString()));
                ui->tableWidget->setItem(row,3,new QTableWidgetItem(result["x"].toString()));
                ui->tableWidget->setItem(row,4,new QTableWidgetItem(result["y"].toString()));
            }
        }
    }
}

void client::on_pushButton_4_clicked()
{
    //生成随机位置

    float x = qrand()%200;
    float y = qrand()%200;
    ui->x->setText(QString("%1").arg(x));
    ui->y->setText(QString("%1").arg(y));
    emit ui->pushButton_3->clicked();


}

void client::tcpdisconnect()
{
    ui->pushButton->setText(QString("连接已断开，点击发起连接"));
}

void client::wificonnected()
{
    delete udpServer;
    delete tcpsender;

    socketinit();
}

void client::socketinit()
{
    udpServer = new QUdpSocket(this);                   //实例化udpsocket对象
    tcpsender = new tcpsocket(this,1);

    if(!udpServer->bind(QHostAddress(ip),ui->udpport->text().toInt(),QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint))
        qDebug()<<"udp接受端绑定端口出错";

    //获取局域网广播地址
    ui->multicastip->setText(broadcast);

    //采用多播模式会有客户端接收不到的情况
    udpServer->joinMulticastGroup(QHostAddress(ui->multicastip->text()));
    connect(udpServer,SIGNAL(readyRead()),this,SLOT(udpget()));             //收到服务器ip和端口信息后立即进行tcp连接并关闭udpsocket句柄
    connect(tcpsender,SIGNAL(connected()),this,SLOT(tcpconnected()));
    connect(tcpsender,SIGNAL(updateClients(QByteArray,tcpsocket*)),this,SLOT(newdata(QByteArray,tcpsocket*)));  //接收数据
    connect(tcpsender,SIGNAL(disconnected()),this,SLOT(tcpdisconnect()));                  //连接断开后
    connect(tcpsender,SIGNAL(sendid(QString)),this,SLOT(getid(QString)));                       //更新id
}

void client::getid(QString id)
{
    this->setWindowTitle(id);
    ui->id->setText(id);
}
