#ifndef SELECTIP_H
#define SELECTIP_H

#include <QWidget>
#include <winpcap.h>

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
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::selectip *ui;
    winpcap *arp;
    void updateipcombox();                          //更新ipcombox
};

#endif // SELECTIP_H
