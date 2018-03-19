#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <stable.h>
#include <QMainWindow>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
public:
    QButtonGroup *c;

public slots:
    void onclickradio();

};

#endif // MAINWINDOW_H
