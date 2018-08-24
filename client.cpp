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
    ui->fileprogressBar->hide();
    ui->pushButton_5->setEnabled(false);

    loadSize = 4*1024;
    totalBytes = 0;
    bytesWritten = 0;
    bytesToWrite = 0;

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
       if(error.error == 0){
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
//    ui->pushButton->setText(QString("已连接"));
    ui->localip->setText(tcpsender->localAddress().toString());         //显示当前ip
    ui->localport->setText(QString("%1").arg(tcpsender->localPort()));      //显示当前端口
}

void client::on_pushButton_3_clicked()
{
    //发送位置信息
    QJsonObject json;
    json.insert("type","location"); //信息类型为位置信息
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

    if(result["type"].toString() == QString("location")){
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
    if(result["type"].toString() == QString("order")){
        if(QMessageBox::No == QMessageBox::question(0,
                                                     tr("收到指令"),
                                                     mess,
                                                     QMessageBox::Yes ,
                                                    QMessageBox::Yes)){

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
    tcpsender_file = NULL;
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
    connect(tcpsender,SIGNAL(sendclientverifyresult(QString)),this,SLOT(verifyresult(QString)));        //验证过程中提示信息
}

void client::getid(QString id)
{
    this->setWindowTitle(id);
    ui->id->setText(id);
}

void client::verifyresult(QString tem)
{
    if(tem == "abort")
        tcpsender->abort();
    else if(tem == "success"){
        ui->pushButton->setText("连接成功");
        //连接成功后发起文件客户端的连接
        if(tcpsender_file == NULL){
            tcpsender_file = new tcpsocket(this,3,ui->id->text());
            connect(tcpsender_file,SIGNAL(sendclientverifyresult(QString)),this,SLOT(verifyresult_file(QString)));
            connect(tcpsender_file,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));
        }
        //连接到主机，这里从界面获取主机地址和端口号
        tcpsender_file->connectToHost(QHostAddress(ui->serverip->text()),ui->serverport->text().toInt());
    }else if(tem == "fail")
        QMessageBox::information(0, QString("验证出错！"), QString("id或密码错误"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    else
        ui->pushButton->setText(tem);
}

void client::on_pushButton_5_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if(!fileName.isEmpty())
        ui->fileprogressBar->show();
        ui->pushButton_5->hide();
        totalBytes = 0;
        bytesWritten = 0;
        bytesToWrite = 0;
        qDebug()<<fileName;
        localFile = new QFile(fileName);
        if(!localFile->open(QFile::ReadOnly))
        {
           qDebug() << "open file error!";
           return;
        }
        //文件总大小
        totalBytes = localFile->size();
        QString currentFileName = fileName.right(fileName.size()- fileName.lastIndexOf('/')-1);
        //流,不知道对手机端接收文件有无影响
        QDataStream sendOut(&outBlock,QIODevice::WriteOnly);
        sendOut.setVersion(QDataStream::Qt_4_6);
        //依次写入总大小信息空间，文件名大小信息空间，文件名
        sendOut << qint64(0) << qint64(0) << currentFileName;
        //这里的总大小是文件名大小等信息和实际文件大小的总和
        totalBytes += outBlock.size();
        sendOut.device()->seek(0);
        //返回outBolock的开始，用实际的大小信息代替两个qint64(0)空间
        sendOut<<totalBytes<<qint64((outBlock.size() - sizeof(qint64)*2));
        //发送完头数据后剩余数据的大小
        bytesToWrite = totalBytes - tcpsender_file->write(outBlock);
        //清空发送缓冲区
        outBlock.resize(0);
}

void client::verifyresult_file(QString tem)
{
    if(tem == "success"){
        ui->pushButton_5->setText(QString("文件传输连接完成，点击发送"));
        connect(tcpsender_file,SIGNAL(bytesWritten(qint64)),this,SLOT(updateClientProgress(qint64)));
        ui->pushButton_5->setEnabled(true);
    }else if(tem == "fail"){
        ui->pushButton_5->setText(QString("文件传输连接失败"));
        ui->pushButton_5->setEnabled(false);
    }
}

void client::updateClientProgress(qint64 tem)
{
    //已经发送数据的大小
    bytesWritten += tem;
    if(bytesToWrite > 0) //如果已经发送了数据
    {
       //每次发送loadSize大小的数据，这里设置为4KB，如果剩余的数据不足4KB，
       //就发送剩余数据的大小
       outBlock = localFile->read(qMin(bytesToWrite,loadSize));
       //发送完一次数据后还剩余数据的大小
       bytesToWrite -= tcpsender_file->write(outBlock);

       //清空发送缓冲区
       outBlock.resize(0);
    } else {
       localFile->close(); //如果没有发送任何数据，则关闭文件
    }

    //更新进度条
    ui->fileprogressBar->setMaximum(totalBytes);
    ui->fileprogressBar->setValue(bytesWritten);

    if(bytesWritten == totalBytes) //发送完毕
    {
        ui->pushButton_5->setText(QString("传送文件 %1 成功,点击继续发送").arg(fileName));
        ui->fileprogressBar->hide();
        ui->pushButton_5->show();
        localFile->close();
    }
}

void client::displayError(QAbstractSocket::SocketError tem)
{
    qDebug() << tcpsender_file->errorString();
}
