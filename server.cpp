#include "server.h"
#include "ui_server.h"
#include <QGraphicsView>

extern QString ip;                 //ip
extern QString broadcast;                  //广播地址

server::server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::server)
{
    setAttribute(Qt::WA_DeleteOnClose);             //关闭窗口后调用析构函数
    ui->setupUi(this);
    udpbro = NULL;
    socketinit();
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);   //表格禁止编辑
    connect(ui->tableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(tablewidget_clicked(int,int)));

    lastpoint.setX(0);
    lastpoint.setY(0);
    endpoint.setX(0);
    endpoint.setY(0);

}

server::~server()
{
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

}

void server::updatetabelwidget(QByteArray mess, tcpsocket * clientsocket,QString id)
{
    qDebug()<<"有新数据";
    QJsonParseError error;
    QJsonDocument jsondoc = QJsonDocument::fromJson(mess,&error);       //转化成json对象
    qDebug()<<error.errorString();
    QVariantMap result = jsondoc.toVariant().toMap();
    if(result.contains(QString("x"))){
        QString ip = clientsocket->peerAddress().toString();
        QString port = QString("%1").arg(clientsocket->peerPort());

        int row = ui->tableWidget->rowCount();

        for(int i = 0;i<row;i++){
            if(ui->tableWidget->item(i,0)->text() == id ){
                ui->tableWidget->removeCellWidget(i,1);             //先清掉
                ui->tableWidget->removeCellWidget(i,2);             //先清掉
                ui->tableWidget->removeCellWidget(i,3);             //先清掉
                ui->tableWidget->removeCellWidget(i,4);             //先清掉
                ui->tableWidget->setItem(i,3,new QTableWidgetItem(ip));     //更新ip地址
                ui->tableWidget->setItem(i,3,new QTableWidgetItem(port));         //更新端口
                ui->tableWidget->setItem(i,3,new QTableWidgetItem(result["x"].toString()));       //更新横坐标
                ui->tableWidget->setItem(i,4,new QTableWidgetItem(result["y"].toString()));           //更新纵坐标
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
                tem.color = pretem.color;

                if(pretem.x == -1)//发送的数据点不足两个的情况
                {
                    tem.lineItemNum=0;
                    pen.setColor(tem.color);
                    brush.setStyle(Qt::SolidPattern);
                    brush.setColor(tem.color);
                    tem.headRect=scene.addRect(tem.x-5,tem.y-5,10,10,pen,brush);
                    locationlist.insert(i.key(),tem);
                }

                //设置起始点
                qDebug() << pretem.x << "  " << pretem.y << "  "
                         << tem.x << "  " << tem.y << endl;

                if(pretem.x != -1){
                    //设置起始点
                    lastpoint.setX(pretem.x);
                    lastpoint.setY(pretem.y);
                    endpoint.setX(tem.x);
                    endpoint.setY(tem.y);
                    pen.setColor(pretem.color);//设置颜色
                    brush.setStyle(Qt::SolidPattern);
                    brush.setColor(pretem.color);
                    lineItemNum=pretem.lineItemNum;//设置轨迹数量
                    for(int k=0;k<pretem.lineItemNum;k++)//复制所有的轨迹段
                    {
                        tem.lineItemPointer[k]=pretem.lineItemPointer[k];
                    }

                    //总的二级指针lineItemPointer，指向tem的lineItemPointer，
                    //tem的lineItemPointer是专属于各个客户端的指针数组，用于记录每一个的轨迹段数据
                    lineItemPointer=tem.lineItemPointer;

                    //从这里开始正式画图
                    //this->update();
                    //在场景scene中添加新一段轨迹LineItem，同时将lineitem的数量+1，并用指针lineItemPointer记录

                    lineItemPointer[lineItemNum++] = scene.addLine(lastpoint.x(),lastpoint.y(),endpoint.x(),endpoint.y());

                   scene.removeItem(pretem.headRect);
                   tem.headRect = scene.addRect(endpoint.x()-5,endpoint.y()-5,10,10,pen,brush);
                    //设置画笔颜色
                    lineItemPointer[lineItemNum-1]->setPen(pen);
                    //为了防止轨迹太多，画面太乱，轨迹段数量超过一定量后就删减掉旧数据
                    if(lineItemNum>10)
                    {
                        scene.removeItem(lineItemPointer[0]);
                        for(int j=0;j<lineItemNum-1;j++)
                        {
                            lineItemPointer[j] = lineItemPointer[j+1];
                        }
                        lineItemNum--;
                    }

                    tem.lineItemNum=lineItemNum;       //记录轨迹段数量
                    locationlist.insert(i.key(),tem);  //将最新的inf结构体数据加入到locationlist中
                    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
                    ui->graphicsView->setScene(&scene);//把场景添加到ui中GraphicsView的框图中，即是画出图像

                }

                break;
            }
        }

    }else{
        qDebug()<<"收到的信息不是位置信息";
        qDebug()<<mess;
    }
}

void server::updatenewclient(QString id,tcpsocket * clientsocket)
{
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->setRowCount(row+1);
    ui->tableWidget->setItem(row,0,new QTableWidgetItem(id));                   //更新卫星id
    ui->tableWidget->setItem(row,1,new QTableWidgetItem(clientsocket->peerAddress().toString()));       //更新新连接ip
    ui->tableWidget->setItem(row,2,new QTableWidgetItem(QString("%1").arg(clientsocket->peerPort())));      //更新新连接端口

    QTableWidgetItem *tem = new QTableWidgetItem("断开");
    tem->setTextAlignment(Qt::AlignCenter);

    ui->tableWidget->setItem(row,5,tem);


    //添加卫星实时位置
    QHashIterator <QString,tcpsocket *> i(this->tcpServer->tcpClientSocketList);
    while(i.hasNext()){
        i.next();
        if(i.value()->peerAddress().toString() == clientsocket->peerAddress().toString()
                && i.value()->peerPort() == clientsocket->peerPort()){
            inf tem;
            tem.x = -1;
            tem.y = -1;
            tem.color = QColor(qrand() % 256, qrand() %256, qrand() % 256);//设置颜色，用于绘制轨迹
            //tem.color = Qt::green;
            qDebug() << "tem.color was set" << endl;
            locationlist.insert(i.key(),tem);
            qDebug()<<locationlist.value(i.key()).x;
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
        if(ui->tableWidget->item(i,1)->text() == ip
           && ui->tableWidget->item(i,2)->text() == port){
            ui->tableWidget->removeRow(i);
            break;
        }
    }
//    free(clientsocket);                    //释放这个不用的连接的内存  此处要用free 不能用delete 会出错
}



    //如果lastpoint或endpoint没有数据，则return跳出函数
//    if(lastpoint == QPoint(0,0) && endpoint == QPoint(0,0) )
//    {
//        return;
//    }

    //防止在没有发送新位置数据的情况下，paintEvent函数被触发导致数据丢失的问题
//    if(lineItemNum!=0)
//    {

//        if( endpoint.x() == lineItemPointer[lineItemNum-1]->line().x2()
//               && endpoint.y() == lineItemPointer[lineItemNum-1]->line().y2() )
//        {
//            return;
//        }
//    }


void server::wificonnected()
{
    //清除
    delete udpsender;
    delete tcpServer;
    udpbro->stop();
    socketinit();
}

void server::socketinit()
{
    udpsender = new QUdpSocket(this);                   //实例化udpsender 对象
    tcpServer = new tcpserver(this);                   //实例化tcpserver对象
    udpsender->bind(QHostAddress(ip),0);
    ui->multicastip->setText(broadcast);         //设置udp广播地址

    //加入广播组
    udpsender->joinMulticastGroup(QHostAddress(ui->multicastip->text()));
    //获取监听的端口和ip
    QJsonObject json;
    json.insert("ip",ip);
    json.insert("port",ui->tcpport->text().toInt());
    QJsonDocument document;
    document.setObject(json);
    QByteArray datagram = document.toJson(QJsonDocument::Compact);

    //启动udp广播线程
    if(NULL == udpbro)
        udpbro = new udpbroad(udpsender,datagram,ui->udpport->text().toInt(),ui->multicastip->text());
    else
        udpbro->reset(udpsender,datagram,ui->udpport->text().toInt(),ui->multicastip->text());
    udpbro->start();

    //监听tcp
    if(!this->tcpServer->isListening()){
        if(!this->tcpServer->listen(QHostAddress(ip),ui->tcpport->text().toInt()))
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
    connect(tcpServer,SIGNAL(newclientsocket(QString,tcpsocket*)),this,SLOT(updatenewclient(QString,tcpsocket*)));
    //关联接收数据的信号与更新界面
    connect(tcpServer,SIGNAL(updateServer(QByteArray,tcpsocket*,QString)),this,SLOT(updatetabelwidget(QByteArray,tcpsocket*,QString)));
    //关联连接断开与更新界面
    connect(tcpServer,SIGNAL(disconnected(tcpsocket*)),this,SLOT(disconnected(tcpsocket*)));
}

void server::tablewidget_clicked(int row, int colum)
{
    QString id = ui->tableWidget->item(row,0)->text();
    //断开连接
    tcpServer->tcpClientSocketList.value(id)->close();
    //从列表中删除
    tcpServer->tcpClientSocketList.remove(id);
    qDebug()<<row<<colum;
}
