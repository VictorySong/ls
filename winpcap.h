#ifndef WINPCAP_H
#define WINPCAP_H

#include <stable.h>
#include <sendarp.h>
#include <getarp.h>

class winpcap :public QObject
{
    Q_OBJECT
public:
    winpcap();
    ~winpcap();
signals:
    void updatealivemac();
private:
    bool flag;                      //ip地址是否可用的标志
    char *ip_addr;                                    //IP地址
    char *ip_netmask;                             //子网掩码
    unsigned char *ip_mac;          //本机MAC地址
    pcap_if_t *alldevs;               //所有适配器
    pcap_if_t *d;                       //当前适配器
    pcap_t *adhandle;           //捕捉实例,是pcap_open返回的对象
    char errbuf[PCAP_ERRBUF_SIZE];
    int dev_num;                            //适配器计数变量
    QMap<QString,pcap_if_t *> usabledev;       //ip可用的适配器
    QMutex mutex;               //锁保证线程安全
    sendarp *sthread;                //向局域网内所有ip 发送arp探测包
    getarp *gthread;                  //获取局域网内活动主机的arp回复包
public:
    QMap<QString,actdevinf> actmac;      //活动设备的mac地址

public:
    void init();
    QString iptos(u_long in);       //转换ip地址为qstring 格式
    bool newhandle(QString ip);         //新的适配器描述子
    void ifget(unsigned int &unetmask);      //获取网卡上的ip 子网掩码
    void GetSelfMac();      //发送获取自身mac地址的数据包
    bool getsendarp();          //捕获探测自身mac地址的arp包   确定有网络连接
    int pcap_get(pcap_pkthdr ** pcap_h,const u_char **pcap_d);    //为pcap_next_ex 加线程锁
    QString getselfmac();      //获取当前选定网卡mac信息
    QString macos(unsigned char *mac = NULL);             //将char* 类型的mac 转化为qstring
    QString getnetmask();               //获取子网掩码
    QList<QString> getusableip();           //获取可用ip列表
    void getactmac();                       //获取活动mac列表
    bool getintstatus();                    //获取当前网络连接状态
    int pcap_send(const u_char * buff ,int size);           //发送arp包
    void getip_mac(unsigned char * mac);                //获取mac地址 unsigned char* 类型  mac既是参数也是返回值
    void getip(char *ip);                               //获取char* 类型的ip地址
    void getnetmask(char *netmask);                     //获取char* 类型的子网掩码
    QString getip();                                    //获取qstring 类型的ip地址

};

#endif // WINPCAP_H
