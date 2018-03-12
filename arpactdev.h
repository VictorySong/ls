#ifndef ARPACTDEV_H
#define ARPACTDEV_H

#include <QWidget>

namespace Ui {
class arpactdev;
}

class arpactdev : public QWidget
{
    Q_OBJECT

public:
    explicit arpactdev(QWidget *parent = 0);
    ~arpactdev();

private:
    Ui::arpactdev *ui;
};

#endif // ARPACTDEV_H
