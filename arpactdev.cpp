#include "arpactdev.h"
#include "ui_arpactdev.h"

arpactdev::arpactdev(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::arpactdev)
{
    ui->setupUi(this);
}

arpactdev::~arpactdev()
{
    delete ui;
}
