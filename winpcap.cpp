#include "winpcap.h"

winpcap::winpcap()
{
    ip_addr = (char *)malloc(sizeof(char) * 16); //申请内存存放IP地址
    ip_netmask = (char *)malloc(sizeof(char) * 16); //申请内存存放NETMASK地址
    ip_mac = (unsigned char *)malloc(sizeof(unsigned char) * 6); //申请内存存放MAC地址
    sthread = new sendarp(this);
    gthread = new getarp(this);
    connect(sthread,SIGNAL(finished()),gthread,SLOT(stop()));
    //关联设置进度条的信号
    connect(sthread,SIGNAL(setmaxprogressbar(ulong)),this,SLOT(maxprogressbar(ulong)));
    connect(sthread,SIGNAL(setprogressbar(ulong)),this,SLOT(progressbar(ulong)));

    dev_num = 0; //初始化适配器数量为0
    this->adhandle = NULL;
}

winpcap::~winpcap()
{
    delete ip_addr;
    delete ip_netmask;
    delete ip_mac;
    sthread->terminate();
    sthread->wait();
    gthread->terminate();
    gthread->wait();
    delete sthread;
    delete gthread;
    pcap_freealldevs(alldevs);
}

void winpcap::init()
{
    //获取本地适配器列表
    if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1) {
        exit(1);
    }
    for (d = alldevs; d != NULL; d = d->next) {
        qDebug()<<"-----------------------------------------------------------------\nnumber:"<<++dev_num<<"\nname:"<<d->name<<"\n";
        if (d->description) {
            //打印适配器的描述信息
            qDebug()<<"description:"<<d->description<<"\n";
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
                    usabledev.insert(ip,d);    // 添加可用的适配器
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
        qDebug()<<"interface not found,please check winpcap installation";
        exit(1);
    }else{
        if(usabledev.count() == 0)
        {
            //代表没有可用的ipv4 地址
            pcap_freealldevs(alldevs);
            exit(1);
        }else{
            QMapIterator<QString,pcap_if_t *> i(usabledev);
            while(i.hasNext()){
                i.next();
                newhandle(i.key());
                GetSelfMac();
                if(!getsendarp()){
                    usabledev.remove(i.key());
                }
            }
        }
    }
}

QString winpcap::iptos(u_long in)
{
    QString output;
    u_char *p;
    p = (u_char *)&in;
    output = QString("%1.%2.%3.%4").arg(p[0]).arg(p[1]).arg(p[2]).arg(p[3]);
    return output;
}

bool winpcap::newhandle(QString ip)
{
    if(!usabledev.contains(ip))
        return false;
    mutex.lock(); //线程锁
    if(this->adhandle != NULL){
        qDebug()<<"关闭设备描述字";
        pcap_close(this->adhandle);
    }
    d = usabledev[ip];
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
    ifget(tem_netmask); //切换适配器后同时更新当前网络的ip 子网掩码
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
    mutex.unlock();
    return true;
}

void winpcap::ifget(unsigned int &unetmask)
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

void winpcap::GetSelfMac()
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
    }
}

bool winpcap::getsendarp()
{
    //只需要用此函数确定有网络连接
    int res;
    struct pcap_pkthdr * pkt_header;//数据包头
    const u_char * pkt_data;//数据
    if((res = pcap_get(&pkt_header,&pkt_data)) >0)
        return true;
    else
        return false;

}

int winpcap::pcap_get(pcap_pkthdr **pcap_h, const u_char **pcap_d)
{
    mutex.lock();                //线程锁
    int tem;
    tem = pcap_next_ex(adhandle,pcap_h,pcap_d);
    mutex.unlock();
    return tem;
}

QList<QString> winpcap::getusableip()
{
    return usabledev.keys();
}

QString winpcap::getselfmac()
{
    GetSelfMac();
    int res;
    struct pcap_pkthdr * pkt_header;//数据包头
    const u_char * pkt_data;//数据
    int i = 0;
    while((res = pcap_get(&pkt_header,&pkt_data)) >0 && i<4){
        //主机回复 100.100.100.100 的包
        if (*(unsigned short *)(pkt_data + 12) == htons(ETH_ARP)
            && *(unsigned short*)(pkt_data + 20) == htons(ARP_REPLY)
            && *(unsigned long*) (pkt_data + 38) == inet_addr("100.100.100.100")) {
            for (int i = 0; i < 6; i++) {
                ip_mac[i] = *(unsigned char *)(pkt_data + 22 + i);
            }
            qDebug()<<"获取自己主机的MAC地址成功!thread";
            break;
        }
        i++;
    }
    if(i<4 && res>0)
        return macos(ip_mac);
    else
        return QString("");
}

QString winpcap::macos(unsigned char *mac)
{
    if(NULL == mac)
        mac = ip_mac;
    return QString("%1-%2-%3-%4-%5-%6").arg(mac[0],2,16).arg(mac[1],2,16).arg(mac[2],2,16).arg(mac[3],2,16).arg(mac[4],2,16).arg(mac[5],2,16);
}

QString winpcap::getnetmask()
{
    return QString("%1").arg(ip_netmask);
}

void winpcap::getactmac()
{
    actmac.clear();
    if(!gthread->isRunning())
        gthread->start();
    if(!sthread->isRunning())
        sthread->start();
}

bool winpcap::getintstatus()
{
    GetSelfMac();
    return getsendarp();
}

int winpcap::pcap_send(const u_char *buff, int size)
{
    return pcap_sendpacket(adhandle,buff,size);
}

void winpcap::getip_mac(unsigned char *mac)
{
    memcpy(mac,ip_mac,6);
}

void winpcap::getip(char *ip)
{
    memcpy(ip,ip_addr,16);
}

void winpcap::getnetmask(char *netmask)
{
    memcpy(netmask,ip_netmask,16);
}

QString winpcap::getip()
{
    return QString("%1").arg(ip_addr);
}

void winpcap::maxprogressbar(unsigned long t)
{
    emit setmaxprogressbar(t);
}

void winpcap::progressbar(unsigned long t)
{
    emit setprogressbar(t);
}
