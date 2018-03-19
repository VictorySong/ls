#include "arpgetthread.h"
#include "arpactdev.h"

arpgetthread::arpgetthread()
{
    stopped = false;

}

void arpgetthread::run()
{
    int res;
    struct pcap_pkthdr * pkt_header;//数据包头
    const u_char * pkt_data;//数据
    while(!stopped){
        if((res = pcap_next_ex(arpinf->adhandle,&pkt_header,&pkt_data)) >0){
            if(*(unsigned short *) (pkt_data+12)==htons(ETH_ARP)
                    &&*(unsigned short *)(pkt_data + 20) == htons(ARP_REPLY)
                    &&*(unsigned long *)(pkt_data + 38) == inet_addr(arpinf->ip_addr)){

                ArpPacket *recv = (ArpPacket *) pkt_data;
                qDebug("IP地址:%d.%d.%d.%d",
                        recv->ah.SourceIpAdd & 255,
                        recv->ah.SourceIpAdd >> 8 & 255,
                        recv->ah.SourceIpAdd >> 16 & 255,
                        recv->ah.SourceIpAdd >> 24 & 255);
                QString tem = mactoqstring((char *)(pkt_data+22));              //将unsigned char 转换成 qstring
                qDebug()<<recv->ah.SourceMacAdd[0]<<recv->ah.SourceMacAdd[1]<<recv->ah.SourceMacAdd[2]<<recv->ah.SourceMacAdd[3]<<recv->ah.SourceMacAdd[4]<<recv->ah.SourceMacAdd[5];
                emit sendactmac(recv->ah.SourceIpAdd,tem);
            }
        }
    }

    stopped = false;


}

void arpgetthread::stop()
{
    stopped = true;
}

void arpgetthread::sendallarp()
{
    qDebug()<<"stop";
    stopped = true;
}

QString arpgetthread::mactoqstring(char *mac)
{
    return QString("%1%2%3%4%5%6").arg(mac[0]).arg(mac[1]).arg(mac[2]).arg(mac[3]).arg(mac[4]).arg(mac[5]);
}
