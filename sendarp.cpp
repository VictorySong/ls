#include "sendarp.h"
#include "winpcap.h"
sendarp::sendarp(winpcap* tem)
{
    stopped = false;
    arp = tem;
}

void sendarp::run()
{
    stopped = false;
    unsigned char sendbuf[42];
    EthernetHeader eh;
    Arpheader ah;
    char ip[16];
    char netmask[16];
    arp->getip(ip);
    arp->getnetmask(netmask);
    //赋值mac 地址
    memset(eh.DestMAC,0xff,6);              //目的地址为全广播地址
//    memcpy(eh.SourMAC,arp->ip_mac,6);
//    memcpy(ah.SourceMacAdd,arp->ip_mac,6);
    arp->getip_mac(eh.SourMAC);
    arp->getip_mac(ah.SourceMacAdd);
    memset(ah.DestMacAdd,0x00,6);
    eh.EthType = htons(ETH_ARP);
    ah.HardwareType = htons(ARP_HARDWARE);
    ah.ProtocolType = htons(ETH_IP);
    ah.HardwareAddLen = 6;
    ah.ProtocolAddLen = 4;
    ah.SourceIpAdd = inet_addr(ip);      //请求方ip 为自身ip
    ah.OperationField = htons(ARP_REQUEST);

    //向局域网内发送arp包
    unsigned long myip = inet_addr(ip);
    unsigned long mynetmask = inet_addr(netmask);
    unsigned long toip = htonl((myip & mynetmask));
    unsigned long num = htonl(inet_addr("255.255.255.255")-mynetmask);

    emit setmaxprogressbar(num);            //发送设置进度条最大值的信号
    for(unsigned long i = 0; i<num ;i++)
    {
        if(!stopped){
            ah.DestIpAdd = htonl(toip+i);
            memset(sendbuf,0,sizeof(sendbuf));
            memcpy(sendbuf,&eh,sizeof(eh));
            memcpy(sendbuf+sizeof(eh),&ah,sizeof(ah));
            if(arp->pcap_send(sendbuf,42)==0){
                emit setprogressbar(i);             //发送设置进度条值的信号
            }
            msleep(0.1);
        }else{
            break;
        }
    }
    emit setprogressbar(num);
    msleep(400);
    qDebug()<<"发送线程关闭";
    stopped = false;
}

void sendarp::stop()
{
    stopped = true;
}
