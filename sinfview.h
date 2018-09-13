#ifndef SINFVIEW_H
#define SINFVIEW_H

#include <QWidget>

namespace Ui {
class sinfview;
}

class sinfview : public QWidget
{
    Q_OBJECT

public:
    explicit sinfview(QString ,QString,float,float,QWidget *parent = 0);
    ~sinfview();

private:
    Ui::sinfview *ui;
    QString id;
    QString ip;
public:
    float x;
    float y;

    void inf_update();


};

#endif // SINFVIEW_H
