#include "searchwifi.h"

searchwifi::searchwifi()
{
    stopped = false;
}

void searchwifi::run()
{
    char *tar = "MI";
    char *tarkey = "123456qaz";
    changetarget(tar,tarkey);
    in();
    qDebug()<<"searching MI ...";
    for(;;){
        for(;;){
            //wifi断开的情况下搜寻wifi
            if(searchwlan()){
                qDebug()<<"MI is searched";

                if (!isconnected()) {
                    connectwlan();
                }else{
                    qDebug()<<"connect success";
                    emit this->wificonnected();         //触发wifi连接成功的信号
                    break;
                }
                Sleep(5000);
            }else{
                setwlanInterfacesoff();
                setwlanInterfaceson();
                qDebug()<<"MI can not be searched";
                Sleep(2000);
            }
        }
        for(;;){
            //检查wifi连接状态
            searchwlan();
            if(isconnected()){
                qDebug()<<"wlan connected";
                Sleep(6000);
            }else{
                setwlanInterfacesoff();
                setwlanInterfaceson();
                break;
            }
        }
    }

    wlanfree();
}

void searchwifi::stop()
{
    stopped = true;
}
