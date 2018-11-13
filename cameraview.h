#ifndef CAMERAVIEW_H
#define CAMERAVIEW_H

#include <QWidget>
#include <QCamera>
#include <QCameraImageCapture>
#include <QCameraViewfinder>
#include <QMediaRecorder>
#include <QDir>
namespace Ui {
class cameraview;
}

class cameraview : public QWidget
{
    Q_OBJECT

public:
    explicit cameraview(QWidget *parent = 0);
    ~cameraview();

private:
    Ui::cameraview *ui;
    QCamera *mCamera;
    QCameraImageCapture *mCameraImageCapture;
    QVideoWidget *widget;
    QMediaRecorder *recorder;
public:
    QString filename;
    void record();
    void disrecord();

};

#endif // CAMERAVIEW_H
