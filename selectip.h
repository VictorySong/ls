#ifndef SELECTIP_H
#define SELECTIP_H

#include <QWidget>
#include <winpcap.h>
#include <server.h>
#include <client.h>

namespace Ui {
class selectip;
}

class selectip : public QWidget
{
    Q_OBJECT

public:
    explicit selectip(QWidget *parent = 0,winpcap *tem = NULL);
    ~selectip();

private slots:
    void on_pushButton_clicked();

    void on_ipcomboBox_currentIndexChanged(const QString &arg1);

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::selectip *ui;
    winpcap *arp;
    server *ser;                            //服务端窗口
    client *cli;                            //客户端窗口
    void updateipcombox();                          //更新ipcombox
};

#endif // SELECTIP_H
