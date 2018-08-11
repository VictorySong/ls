#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QButtonGroup>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QRadioButton *pButton = new QRadioButton(this);
    pButton->setText("我爱你");
    ui->netcard->addWidget(pButton);
    QRadioButton *b = new QRadioButton(this);
    b->setText("你好");
    ui->netcard->addWidget(b);
    c = new QButtonGroup(this);
    c->addButton(pButton,1);
    c->addButton(b,2);
    connect(pButton,SIGNAL(clicked()),this,SLOT(onclickradio()));
    connect(b,SIGNAL(clicked()),this,SLOT(onclickradio()));




}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onclickradio()
{
    switch(c->checkedId())
   {

   case 1:
       qDebug() << "我爱你";
       break;
   case 2:
       qDebug() << "你好";
       break;
   }
}


