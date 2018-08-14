#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <QDialog>

namespace Ui {
class Instruction;
}

class Instruction : public QDialog
{
    Q_OBJECT

public:
    explicit Instruction(QWidget *parent = 0);
    ~Instruction();

    QString order;

private slots:
    void on_order_textChanged();

private:
    Ui::Instruction *ui;
};

#endif // INSTRUCTION_H
