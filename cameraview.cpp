#include "cameraview.h"
#include "ui_cameraview.h"

cameraview::cameraview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cameraview)
{
    ui->setupUi(this);
}

cameraview::~cameraview()
{
    delete ui;
}
