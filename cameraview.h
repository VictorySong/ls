#ifndef CAMERAVIEW_H
#define CAMERAVIEW_H

#include <QWidget>

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
};

#endif // CAMERAVIEW_H
