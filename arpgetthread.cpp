#include "arpgetthread.h"
#include "arpactdev.h"

arpgetthread::arpgetthread()
{
    this->stopped = false;
    this->sendthreadstopped = false;
}

void arpgetthread::run()
{
    int res;
    struct pcap_pkthdr * pkt_header;//数据包头
    const u_char * pkt_data;//数据

    pcap_if_t *alldevs=NULL;               //所有适配器
    pcap_if_t *d;                     //当前适配器
    char errbuf[PCAP_ERRBUF_SIZE];
    QVector <pcap_if_t *> usabledev;
    QList <QString> ip_combox;

    int i = 0;

    arpinf->newadhandle();                      //解决第一次 ipcombox 的indexchanged
    bool sendflag=true;                    //标志位
    while(!stopped){
        if(sendthreadstopped){
            emit upmactable();
            sendthreadstopped = false;
        }
        if((res = arpinf->pcap_get(arpinf->adhandle,&pkt_header,&pkt_data)) >0){
            qDebug()<<++i;

            //活动主机的arp回复包
            if(*(unsigned short *) (pkt_data+12)==htons(ETH_ARP)
                    &&*(unsigned short *)(pkt_data + 20) == htons(ARP_REPLY)
                    &&*(unsigned long *)(pkt_data + 38) == inet_addr(arpinf->ip_addr)
                    ){

                ArpPacket *recv = (ArpPacket *) pkt_data;
                QString tem = mactoqstring((char *)(pkt_data+22));              //将unsigned char 转换成 qstring
                qDebug()<<recv->ah.SourceMacAdd[0]<<recv->ah.SourceMacAdd[1]<<recv->ah.SourceMacAdd[2]<<recv->ah.SourceMacAdd[3]<<recv->ah.SourceMacAdd[4]<<recv->ah.SourceMacAdd[5];
                emit sendactmac(recv->ah.SourceIpAdd,tem);
            }

            //主机回复 100.100.100.100 的包
            if (*(unsigned short *)(pkt_data + 12) == htons(ETH_ARP)
                && *(unsigned short*)(pkt_data + 20) == htons(ARP_REPLY)
                && *(unsigned long*) (pkt_data + 38) == inet_addr("100.100.100.100")) {
                sendflag = false;               //发从探测自己mac包后 应当在检测到回复包后将sendflag 设为false 才能保证在更换网络时更新ip
                for (int i = 0; i < 6; i++) {
                    arpinf->ip_mac[i] = *(unsigned char *)(pkt_data + 22 + i);

                }
                qDebug()<<"获取自己主机的MAC地址成功!thread";

                emit sendnetstate(true);            //网络可用


            }
        }else if(res==0){
            if(sendflag){
                emit sendnetstate(false);           //网络不可用

//                //已经发过包仍超时      更新设备列表  而后重新发包
//                qDebug()<<usabledev;
//                ip_combox.clear();
//                usabledev.remove(0,usabledev.count());
//                //获取本地适配器列表
//                if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1) {
//                    exit(1);
//                }
//                for (d = alldevs; d != NULL; d = d->next) {
//                    pcap_addr_t *a;       //网络适配器的地址用来存储变量
//                    for (a = d->addresses; a; a = a->next) {
//                        switch (a->addr->sa_family)
//                        {
//                        case AF_INET:  //代表IPV4类型地址
//                            if (a->addr) {
//                                QString ip= arpinf->iptos(((struct sockaddr_in *)a->addr)->sin_addr.s_addr);
//                                usabledev.append(d);    // 添加可用的适配器
//                                ip_combox.append(ip);
//                            }

//                            break;
//                        case AF_INET6: //代表IPV6类型地址
//                            break;
//                        default:
//                            break;
//                        }
//                    }
//                }
//                if(usabledev.count() == 0)
//                {
//                    //代表没有可用的ipv4 地址
//                    pcap_freealldevs(alldevs);
//                }
//                else
//                {
//                    arpinf->changealldev(alldevs,usabledev,ip_combox);
//                }
//                arpinf->newadhandle();
                arpinf->GetSelfMac(arpinf->adhandle,arpinf->ip_addr);

            }else{
                arpinf->GetSelfMac(arpinf->adhandle,arpinf->ip_addr);           //超时后未发包则发一次包
                sendflag = true;
            }
            i = 0;                  //第几次捕获到包
        }
    }
    this->stopped = false;

    //this->arpinf->upmactab();             //此语句会致使线程之间出现问题。现象是，网络连接检测线程出现阻塞

}


void arpgetthread::sendallarp()
{
    qDebug()<<"发送探测mac线程stop";
    sendthreadstopped = true;
}


QString arpgetthread::mactoqstring(char *mac)
{
    return QString("%1%2%3%4%5%6").arg(mac[0]).arg(mac[1]).arg(mac[2]).arg(mac[3]).arg(mac[4]).arg(mac[5]);
}
