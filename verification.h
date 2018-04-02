#ifndef VERIFICATION_H
#define VERIFICATION_H

#include <QDialog>

namespace Ui {
class verification;
}

class verification : public QDialog
{
    Q_OBJECT

public:
    explicit verification(QWidget *parent = 0);
    ~verification();
    QString id;             //小卫星编号
    QString secret;         //小卫星登录密码

private slots:
    void on_id_editingFinished();

    void on_secret_editingFinished();

private:
    Ui::verification *ui;
};

#endif // VERIFICATION_H
