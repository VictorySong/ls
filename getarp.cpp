#include "getarp.h"
#include "winpcap.h"

getarp::getarp(winpcap * tem)
{
    stopped = false;
    this->arp = tem;
}

void getarp::stop()
{
    stopped = true;
}

void getarp::run()
{
    stopped = false;
    if(arp->getintstatus() && arp->getselfmac().length()!=0){
        int res;
        struct pcap_pkthdr * pkt_header;//数据包头
        const u_char * pkt_data;//数据
        while(!stopped){
            if((res = arp->pcap_get(&pkt_header,&pkt_data))>0){
                //活动主机的arp回复包
                if(*(unsigned short *) (pkt_data+12)==htons(ETH_ARP)
                        &&*(unsigned short *)(pkt_data + 20) == htons(ARP_REPLY)
                        &&*(unsigned long *)(pkt_data + 38) != inet_addr("100.100.100.100")){
                    ArpPacket *recv = (ArpPacket *) pkt_data;
                    QString mac = arp->macos((unsigned char *)(pkt_data+22));              //将unsigned char 转换成 qstring
                    QString ip = arp->iptos(recv->ah.SourceIpAdd);
                    qDebug()<<ip<<":"<<mac;
                    actdevinf tem;
                    tem.mac = mac;
                    tem.tcpconsta = false;
                    arp->actmac.insert(ip,tem);

                }
            }
        }
    }
    qDebug()<<"接收线程关闭";
    emit arp->updatealivemac();             //发送更新活动mac的信号
    stopped = false;
}
