#include "readme.h"
#include "ui_readme.h"

readme::readme(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::readme)
{
    ui->setupUi(this);
}

readme::~readme()
{
    delete ui;
}

void readme::on_confirmButton_clicked()
{
    this->hide();
}
