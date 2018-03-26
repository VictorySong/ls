#include "server.h"
#include "ui_server.h"

server::server(QWidget *parent,winpcap *tem) :
    QWidget(parent),
    ui(new Ui::server)
{
    ui->setupUi(this);
    if(NULL == tem)
        exit(1);
    arp = tem;
}

server::~server()
{
    delete ui;
}
