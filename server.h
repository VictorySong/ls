#ifndef SERVER_H
#define SERVER_H

#include <stable.h>
#include <winpcap.h>

namespace Ui {
class server;
}

class server : public QWidget
{
    Q_OBJECT

public:
    explicit server(QWidget *parent = 0,winpcap *tem = NULL);
    ~server();

private:
    Ui::server *ui;
    winpcap *arp;
};

#endif // SERVER_H
