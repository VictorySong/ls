#include "arpactdev.h"
#include "ui_arpactdev.h"

arpactdev::arpactdev(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::arpactdev)
{
    ui->setupUi(this);
    ip_addr = (char *)malloc(sizeof(char) * 16); //申请内存存放IP地址
    ip_netmask = (char *)malloc(sizeof(char) * 16); //申请内存存放NETMASK地址
    ip_mac = (unsigned char *)malloc(sizeof(unsigned char) * 6); //申请内存存放MAC地址
    dev_num = 0; //初始化适配器数量为0
    this->adhandle = NULL;

    //初始化线程 中的arpinf 指针
    arpth.arpinf = this;
    arpgth.arpinf = this;

    flag = false;               //初始化时ip地址不可用
    if(connect(&arpth,SIGNAL(sendall()),&arpgth,SLOT(sendallarp()))){       //将发送arp完的信号和接受arp线程中槽函数绑定实现发送完结束接收线程
        qDebug()<<"关联成功";
    }
    if(connect(&arpgth,SIGNAL(sendactmac(ulong,QString)),this,SLOT(getactmac(ulong,QString)))){               //将探测到新mac 与更新活动主机mac 相关联
        qDebug()<<"关联成功";
    }
    if(connect(&arpgth,SIGNAL(sendnetstate(bool)),this,SLOT(updatedev_tip(bool)))){
        qDebug()<<"关联成功";
    }
    if(connect(&arpgth,SIGNAL(upmactable()),this,SLOT(upmactab()))){
        qDebug()<<"关联成功";
    }
    if(connect(&arpgth,SIGNAL(upadhandle()),this,SLOT(newadhandle()))){
        qDebug()<<"关联成功";
    }

    this->init();

    this->tcpServer = new QTcpServer(this);                   //实例化tcpserver对象
    this->tcpsender = new QTcpSocket(this);
    this->udpsender = new QUdpSocket(this);                   //实例化udpsender 对象
    this->udpServer = new QUdpSocket(this);                   //实例化udpsocket对象
    this->udpServer->bind(45454,QUdpSocket::ShareAddress);
    connect(udpServer,SIGNAL(readyRead()),this,SLOT(udpget()));
    connect(tcpServer,SIGNAL(newConnection()),this,SLOT(tcpserversend()));
    connect(tcpsender,SIGNAL(readyRead()),this,SLOT(tcpclientread()));
    connect(tcpsender,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayerror(QAbstractSocket::SocketError)));


}

arpactdev::~arpactdev()
{
    delete ui;
    delete ip_addr;
    delete ip_netmask;
    delete ip_mac;
    delete tcpServer;
    arpth.terminate();
    arpth.wait();
    arpgth.terminate();
    arpgth.wait();

}

int arpactdev::init()
{

    //获取本地适配器列表
    if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1) {
        //结果为-1代表出现获取适配器列表失败
        qDebug()<<"Error in pcap_findalldevs_ex:\n";
        //exit(0)代表正常退出,exit(other)为非正常退出,这个值会传给操作系统
        exit(1);
    }
    for (d = alldevs; d != NULL; d = d->next) {
        qDebug()<<"-----------------------------------------------------------------\nnumber:"<<++dev_num<<"\nname:"<<d->name<<"\n";
        if (d->description) {
            //打印适配器的描述信息
            qDebug()<<"description:"<<d->description<<"\n";

        }
        else {
            //适配器不存在描述信息
            qDebug()<<"description:no description\n";

        }
        //打印本地环回地址
        qDebug()<<"\tLoopback:"<<((d->flags & PCAP_IF_LOOPBACK) ? "yes" : "no");

        pcap_addr_t *a;       //网络适配器的地址用来存储变量
        for (a = d->addresses; a; a = a->next) {

            //sa_family代表了地址的类型,是IPV4地址类型还是IPV6地址类型
            switch (a->addr->sa_family)
            {
            case AF_INET:  //代表IPV4类型地址

                qDebug()<<"Address Family Name:AF_INET";
                if (a->addr) {
                    //->的优先级等同于括号,高于强制类型转换,因为addr为sockaddr类型，对其进行操作须转换为sockaddr_in类型
                    qDebug()<<"Address:";
                    QString ip= iptos(((struct sockaddr_in *)a->addr)->sin_addr.s_addr);
                    qDebug()<<ip;
                    usabledev.append(d);    // 添加可用的适配器
                    ui->ipcombox->addItem(ip);  //添加IP地址选择项
                }
                if (a->netmask) {
                    qDebug()<<"Netmask:";
                    qDebug()<<iptos(((struct sockaddr_in *)a->netmask)->sin_addr.s_addr);
                }
                if (a->broadaddr) {
                    qDebug()<<"Broadcast Address: ";
                    qDebug()<<iptos(((struct sockaddr_in *)a->broadaddr)->sin_addr.s_addr);
                }
                if (a->dstaddr) {
                    qDebug()<<"Destination Address:";
                    qDebug()<<iptos(((struct sockaddr_in *)a->dstaddr)->sin_addr.s_addr);
                }
                break;
            case AF_INET6: //代表IPV6类型地址
                qDebug()<<"Address Family Name:AF_INET6\n"<<"this is an IPV6 address\n";

                break;
            default:
                break;
            }
        }
    }
    //i为0代表上述循环未进入,即没有找到适配器,可能的原因为Winpcap没有安装导致未扫描到
    if (dev_num == 0) {
        ui->dev_tip->setText("没有找到适配器");
        qDebug()<<"interface not found,please check winpcap installation";
    }
    else
    {
        if(!usabledev[0])
        {
            //代表没有可用的ipv4 地址
            ui->dev_tip->setText("没有可用的ipv4地址");
            pcap_freealldevs(alldevs);
            return 1;
        }
        else
        {
            //有可用ipv4 移除选项中的无
            ui->ipcombox->removeItem(0);
        }
    }
    return 0;
}

QString arpactdev::iptos(u_long in)
{
    QString output;

    u_char *p;

    p = (u_char *)&in;
    output = QString("%1.%2.%3.%4").arg(p[0]).arg(p[1]).arg(p[2]).arg(p[3]);
    return output;
}

QString arpactdev::mactos(unsigned char* mac){
    return QString("%1-%2-%3-%4-%5-%6").arg(mac[0],2,16).arg(mac[1],2,16).arg(mac[2],2,16).arg(mac[3],2,16).arg(mac[4],2,16).arg(mac[5],2,16);
}
void arpactdev::ifget(pcap_if_t *d, char *ip_addr, char *ip_netmask,unsigned int &unetmask)
{
    pcap_addr_t *a;
    //遍历所有的地址,a代表一个pcap_addr
    for (a = d->addresses; a; a = a->next) {
        switch (a->addr->sa_family) {
        case AF_INET:  //sa_family ：是2字节的地址家族，一般都是“AF_xxx”的形式。通常用的都是AF_INET。代表IPV4
            if (a->addr) {
                QString ipstr;
                //将地址转化为字符串
                ipstr = iptos(((struct sockaddr_in *) a->addr)->sin_addr.s_addr); //*ip_addr
                memcpy(ip_addr, ipstr.toLatin1().data(), 16);
            }
            if (a->netmask) {
                QString netmaskstr;
                netmaskstr = iptos(((struct sockaddr_in *) a->netmask)->sin_addr.s_addr);
                unetmask = ((struct sockaddr_in *) a->netmask)->sin_addr.s_addr;
                memcpy(ip_netmask, netmaskstr.toLatin1().data(), 16);
            }
        case AF_INET6:
            break;
        }
    }
}

int arpactdev::GetSelfMac(pcap_t *adhandle, const char *ip_addr)
{
    //构造探测自己mac地址的arp数据包  本机收到该数据包后会回复，arpgetthread 线程就会监听到该回复 进而取出mac地址
    unsigned char sendbuf[42]; //arp包结构大小

    EthernetHeader eh; //以太网帧头
    Arpheader ah;  //ARP帧头

    //将已开辟内存空间 eh.dest_mac_add 的首 6个字节的值设为值 0xff。
    memset(eh.DestMAC, 0xff, 6); //目的地址为全为广播地址
    memset(eh.SourMAC, 0x0f, 6);
    memset(ah.DestMacAdd, 0x0f, 6);
    memset(ah.SourceMacAdd, 0x00, 6);
    //htons将一个无符号短整型的主机数值转换为网络字节顺序
    eh.EthType = htons(ETH_ARP);
    ah.HardwareType = htons(ARP_HARDWARE);
    ah.ProtocolType = htons(ETH_IP);
    ah.HardwareAddLen = 6;
    ah.ProtocolAddLen = 4;
    ah.SourceIpAdd = inet_addr("100.100.100.100");
    ah.OperationField = htons(ARP_REQUEST);
    ah.DestIpAdd = inet_addr(ip_addr);
    memset(sendbuf, 0, sizeof(sendbuf));
    memcpy(sendbuf, &eh, sizeof(eh));
    memcpy(sendbuf + sizeof(eh), &ah, sizeof(ah));

    if (pcap_sendpacket(adhandle, sendbuf, 42) == 0) {
        qDebug()<<"PacketSend succeed\n";
    }else {
        qDebug()<<"PacketSendPacket in getmine Error: "<<GetLastError();
        return 0;
    }

}

void arpactdev::on_ipcombox_currentIndexChanged(const QString &arg1)
{

    //用户选择适配器进行抓包
    qDebug()<<"index changed ................";
    ui->dev_tip->setText(QString("网络检测中"));
    newadhandle();
    if(!this->arpgth.isRunning())
        this->arpgth.start();                     //启用监听arp包的线程
}


bool arpactdev::newadhandle()
{
    QMutexLocker locker(&mutex); //线程锁

    int num=ui->ipcombox->currentIndex();
    if(usabledev[0] && num<usabledev.count()){
        d = usabledev[num];
        if(this->adhandle != NULL){
            qDebug()<<"关闭设备描述字";
            pcap_close(this->adhandle);
        }
        if ((adhandle = pcap_open(d->name,        //设备名称
            60,       //存放数据包的内容长度
            PCAP_OPENFLAG_PROMISCUOUS,  //混杂模式
            100,           //超时时间
            NULL,          //远程验证
            errbuf         //错误缓冲
        )) == NULL) {
            //打开适配器失败,打印错误并释放适配器列表
            qDebug()<<stderr<<"\nUnable to open the adapter. %s is not supported by WinPcap\n"<<d->name;
            // 释放设备列表
            pcap_freealldevs(alldevs);
            return false;
        }


        unsigned int tem_netmask;
        ifget(d, ip_addr, ip_netmask,tem_netmask); //切换适配器后同时更新当前网络的ip 子网掩码
        struct bpf_program fcode;
        //设置过滤 只抓取arp类型包
        QString tem = QString("(dst host %1 or dst host 100.100.100.100) and arp").arg(ip_addr);
        qDebug()<<tem;
        QByteArray tem1 = tem.toLatin1();
        char *tem2 = tem1.data();
        if(pcap_compile(adhandle,&fcode,tem2,1,tem_netmask)<0){
            qDebug()<<"设置过滤出错";
        }
        if(pcap_setfilter(adhandle,&fcode)<0){
            qDebug()<<"设置过滤出错";
        }
        return true;
    }
    return false;
}

void arpactdev::changealldev(pcap_if_t * dev ,QVector <pcap_if_t *> udev,QList <QString> combox)
{
    QMutexLocker locker(&mutex); //线程锁
    //更新这个类中的 设备列表
    pcap_freealldevs(this->alldevs);
    this->alldevs = dev;
    this->usabledev = udev;
    int i = 0;
    while(i<combox.count()){
        if(i<ui->ipcombox->count())
            ui->ipcombox->setItemText(i,combox[i]);
        else
            ui->ipcombox->insertItem(i,combox[i]);
        i++;
    }

}


void arpactdev::updatedev_tip(bool online)
{
    qDebug()<<"updatedev_tip";
    if(online){
        flag = true;
        QString tem = QString("%1%2%3%4").arg("ip地址:").arg(ip_addr).arg("\n子网掩码:").arg(ip_netmask);
        tem += QString("\nmac地址:%1-%2-%3-%4-%5-%6").arg(ip_mac[0],2,16).arg(ip_mac[1],2,16).arg(ip_mac[2],2,16).arg(ip_mac[3],2,16).arg(ip_mac[4],2,16).arg(ip_mac[5],2,16);
        ui->dev_tip->setText(tem);

    }else{
        ui->dev_tip->setText(QString("网络不可用"));
        flag = false;
    }
}




void arpactdev::on_getmacbutton_clicked()
{
    if(!arpth.isRunning())
        arpth.start();                  //发送arp线程

}

void arpactdev::getactmac(unsigned long ip,QString mac)
{
    actdevinf tem;
    tem.mac = mac;
    tem.tcpconsta = false;
    if(actmac.contains(iptos(ip))){
        actmac.value(iptos(ip),tem);
    }else{
        actmac.insert(iptos(ip),tem);
    }

}

QString arpactdev::actmackey(QString t)
{
    QHash<QString, actdevinf>::const_iterator i = actmac.constBegin();
    while (i != actmac.constEnd()) {
        if(i.value().mac == t)
        {
            return i.key();
        }
        ++i;
    }
    return QString("");
}


void arpactdev::on_pushButton_clicked()
{
    //将当前tcp 监听地址和端口以udp广播的形式发送出去
    QJsonObject json;
    json.insert("ip",QString(ip_addr));
    json.insert("port",ui->tcpport->text().toInt());
    QJsonDocument document;
    document.setObject(json);
    QByteArray datagram = document.toJson(QJsonDocument::Compact);
    this->udpsender->writeDatagram(datagram.data(),datagram.size(),
                          QHostAddress::Broadcast,45454);

}


void arpactdev::upmactab()
{
    unsigned char mac[6];
    QString tem;
    ui->mactableview->setRowCount(actmac.count());
    QHash<QString, actdevinf>::const_iterator i = actmac.constBegin();
    int j = 0;
    while (i != actmac.constEnd()) {
        qDebug()<<"hfhhh";
        tem = i.value().mac;
        memcpy(mac,(unsigned char*)tem.toLatin1().data(),6);    //将qstring 转换成 unsigned char
        ui->mactableview->setItem(j,0,new QTableWidgetItem(i.key()));
        ui->mactableview->setItem(j,1,new QTableWidgetItem(mactos(mac)));
        if(i.value().tcpconsta){
            ui->mactableview->setItem(j,2,new QTableWidgetItem("已连接"));
        }else{
            ui->mactableview->setItem(j,2,new QTableWidgetItem("未连接"));
        }
        ++i;
        ++j;
    }
}

void arpactdev::on_listentcpport_clicked()
{

    if(!this->tcpServer->isListening()){
        if(!this->tcpServer->listen(QHostAddress(QString(ip_addr)),ui->tcpport->text().toInt()))
        {
            qDebug() << this->tcpServer->errorString();
        }else{
            qDebug()<<this->tcpServer->serverAddress();
            qDebug()<<this->tcpServer->serverPort();
        }
        ui->listentcpport->setText(QString("正在监听，点击关闭"));
    }else{
        this->tcpServer->close();   //如果正在监听则关闭
        ui->listentcpport->setText(QString("开始监听"));
    }

}

int arpactdev::pcap_get(pcap_t *thandle, pcap_pkthdr **pcap_h, const u_char **pcap_d)
{
    QMutexLocker locker(&mutex);                //线程锁
    return pcap_next_ex(thandle,pcap_h,pcap_d);
}

int arpactdev::pcap_send(pcap_t *a, const u_char *b, int c)
{
    QMutexLocker locker(&mutex);                //线程锁
    return pcap_sendpacket(a,b,c);
}




void arpactdev::udpget()
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
       qDebug()<<"ip:"<<result["ip"].toString();
       qDebug()<<"port:"<<result["port"].toInt();
       tcpserver_ip = result["ip"].toString();
       tcpserver_port = result["port"].toInt();

       ui->udpget->setText(datagram);
    }
}

void arpactdev::tcpserversend()
{
    //用于暂存我们要发送的数据
        QByteArray block;

        //使用数据流写入数据
        QDataStream out(&block,QIODevice::WriteOnly);

        //设置数据流的版本，客户端和服务器端使用的版本要相同
        out.setVersion(QDataStream::Qt_4_6);

        out<<(quint16) 0;
        out<<tr("hello Tcp!!!");
        out.device()->seek(0);
        out<<(quint16) (block.size() - sizeof(quint16));

        //我们获取已经建立的连接的子套接字
        QTcpSocket *clientConnection = this->tcpServer->nextPendingConnection();

        connect(clientConnection,SIGNAL(disconnected()),clientConnection,
               SLOT(deleteLater()));
        clientConnection->write(block);
        clientConnection->disconnectFromHost();

        //发送数据成功后，显示提示
        ui->statusLabel->setText("send message successful!!!");
}

void arpactdev::tcpclientread()
{
    QDataStream in(tcpsender);
    in.setVersion(QDataStream::Qt_4_6);
    //设置数据流版本，这里要和服务器端相同
    if(blockSize==0) //如果是刚开始接收数据
    {
       //判断接收的数据是否有两字节，也就是文件的大小信息
       //如果有则保存到blockSize变量中，没有则返回，继续接收数据
       if(tcpsender->bytesAvailable() < (int)sizeof(quint16)) return;
       in >> blockSize;
    }
    if(tcpsender->bytesAvailable() < blockSize) return;
    //如果没有得到全部的数据，则返回，继续接收数据
    in >> tcpmessage;
    //将接收到的数据存放到变量中
    ui->tcpget->setText(tcpmessage);
    //显示接收到的数据
}

void arpactdev::displayerror(QAbstractSocket::SocketError)
{
    qDebug()<<tcpsender->errorString();
}

void arpactdev::on_newtcpconnect_clicked()
{
    blockSize = 0; //初始化其为0
    tcpsender->abort(); //取消已有的连接

    //连接到主机，这里从界面获取主机地址和端口号
    tcpsender->connectToHost(QHostAddress(QString(ip_addr)),ui->tcpport->text().toInt());
}

void arpactdev::on_pushButton_2_clicked()
{

}
