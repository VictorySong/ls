#include "verification.h"
#include "ui_verification.h"

verification::verification(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::verification)
{
    ui->setupUi(this);
}

verification::~verification()
{
    delete ui;
}

void verification::on_id_editingFinished()
{
    id = ui->id->text();
}

void verification::on_secret_editingFinished()
{
    secret = ui->secret->text();
}
