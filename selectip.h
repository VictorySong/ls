#ifndef SELECTIP_H
#define SELECTIP_H

#include <QWidget>
#include <server.h>
#include <client.h>

namespace Ui {
class selectip;
}

class selectip : public QWidget
{
    Q_OBJECT

public:
    explicit selectip(QWidget *parent = 0);
    ~selectip();
    QMap<QString,QString> usableip;                 //存储ip broadcast
private slots:

    void on_ipcomboBox_currentIndexChanged(const QString &arg1);

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::selectip *ui;
    server *ser;                            //服务端窗口
    QList<client *>cli;                            //客户端窗口
    void updateipcombox();                          //更新ipcombox
public slots:
    void newser(QObject*);                  //服务器窗口摧毁后执行
    void destoryclient(QObject *);           //客户端窗口摧毁后执行

};

#endif // SELECTIP_H
