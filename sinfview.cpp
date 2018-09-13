#include "sinfview.h"
#include "ui_sinfview.h"

sinfview::sinfview(QString id,QString ip,float x,float y,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::sinfview)
{
    ui->setupUi(this);
    this->id = id;
    this->ip = ip;
    this->x = x;
    this->y = y;
    ui->label_id->setText(id);
    ui->label_ip->setText(ip);
    this->inf_update();
}

sinfview::~sinfview()
{
    delete ui;
}

void sinfview::inf_update()
{
    ui->label_x->setText(QString("x:%1").arg(x));
    ui->label_y->setText(QString("x:%1").arg(y));
}
