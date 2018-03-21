#include "checknet.h"
#include "arpactdev.h"

checknet::checknet()
{
    stopped = false;
}


void checknet::run()
{

    while(!stopped){
//        arpinf->updatedev_tip();
        //emit updev_tip();
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
        ah.DestIpAdd = inet_addr(arpinf->ip_addr);
        memset(sendbuf, 0, sizeof(sendbuf));
        memcpy(sendbuf, &eh, sizeof(eh));
        memcpy(sendbuf + sizeof(eh), &ah, sizeof(ah));


        if (pcap_sendpacket(arpinf->adhandle, sendbuf, 42) == 0) {
            qDebug()<<"PacketSend succeed  checknet";
        }
        else {
            qDebug()<<"PacketSendPacket in getmine Error: "<<GetLastError();
        }

        sleep(5);
    }
}
