#include "cameraview.h"
#include "ui_cameraview.h"

cameraview::cameraview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cameraview)
{
    ui->setupUi(this);
    filename = QString("c:/lsfile/video");
    QDir dir(filename);
    if(!dir.exists())
        dir.mkpath(filename);//创建多级目录
    filename += "/";
    mCamera = new QCamera;//启用系统摄像头
    mCamera->setCaptureMode(QCamera::CaptureVideo);//设置摄像头的模式，可以抓取视频
    recorder = new QMediaRecorder(mCamera);
    mCamera->setViewfinder(ui->viewfinder);//设置显示的窗体
    mCamera->start();//摄像头启动
}

cameraview::~cameraview()
{
    delete mCamera;
    delete recorder;
    delete ui;
}

void cameraview::record()
{
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy_MM_dd_hh:mm:ss");
    filename += current_date;
    filename += QString(".mp4");

    recorder->setOutputLocation(QUrl(filename));
    recorder->record();
}

void cameraview::disrecord()
{
    recorder->stop();
}
