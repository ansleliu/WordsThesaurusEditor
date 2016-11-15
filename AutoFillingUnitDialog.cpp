#include "AutoFillingUnitDialog.h"
#include "ui_AutoFillingUnitDialog.h"

AutoFillingUnitDialog::AutoFillingUnitDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AutoFillingUnitDialog)
{
    ui->setupUi(this);
}

AutoFillingUnitDialog::~AutoFillingUnitDialog()
{
    delete ui;
}

int AutoFillingUnitDialog::returnWordNumUnit()
{
    return ui->WordNumSpinBox->value();
}

void AutoFillingUnitDialog::on_OKButton_clicked()
{
    QDialog::accept();
}

void AutoFillingUnitDialog::on_quiteButton_clicked()
{
    QDialog::reject();
}

