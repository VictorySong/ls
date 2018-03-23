#include "arpsendthread.h"
#include "arpactdev.h"

arpsendthread::arpsendthread()
{
    stopped = false;
}

void arpsendthread::run()
{
    unsigned char sendbuf[42];
    EthernetHeader eh;
    Arpheader ah;

    //赋值mac 地址
    memset(eh.DestMAC,0xff,6);              //目的地址为全广播地址
    memcpy(eh.SourMAC,arpinf->ip_mac,6);
    memcpy(ah.SourceMacAdd,arpinf->ip_mac,6);
    memset(ah.DestMacAdd,0x00,6);
    eh.EthType = htons(ETH_ARP);
    ah.HardwareType = htons(ARP_HARDWARE);
    ah.ProtocolType = htons(ETH_IP);
    ah.HardwareAddLen = 6;
    ah.ProtocolAddLen = 4;
    ah.SourceIpAdd = inet_addr(arpinf->ip_addr);      //请求方ip 为自身ip
    ah.OperationField = htons(ARP_REQUEST);

    //向局域网内发送arp包
    unsigned long myip = inet_addr(arpinf->ip_addr);
    unsigned long mynetmask = inet_addr(arpinf->ip_netmask);
    unsigned long toip = htonl((myip & mynetmask));
    unsigned long num = htonl(inet_addr("255.255.255.255")-mynetmask);


    for(unsigned int i = 0; i<num ;i++)
    {
        if(!stopped){
            ah.DestIpAdd = htonl(toip+i);
            memset(sendbuf,0,sizeof(sendbuf));
            memcpy(sendbuf,&eh,sizeof(eh));
            memcpy(sendbuf+sizeof(eh),&ah,sizeof(ah));
            if(pcap_sendpacket(arpinf->adhandle,sendbuf,42)==0){

            }
            msleep(5);
        }else{
            break;
        }
    }
    sleep(1);               //发送完毕一秒钟后发送信号
    emit sendall();       //发送结束信号
    stopped = false;
}

void arpsendthread::stop()
{
    stopped = true;
}


