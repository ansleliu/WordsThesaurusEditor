#include "ExportExcelDialog.h"
#include "ui_ExportExcelDialog.h"

ExportExcelDialog::ExportExcelDialog(QString filename, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportExcelDialog)
{
    ui->setupUi(this);
    fileName = filename;
    ui->FileNameLineEdit->setText(fileName);
}

ExportExcelDialog::~ExportExcelDialog()
{
    delete ui;
}

QString ExportExcelDialog::getFilePath()
{
    return ui->FileNameLineEdit->text();
}

void ExportExcelDialog::on_OKPushButton_clicked()
{
    QDialog::accept();
}

void ExportExcelDialog::on_QuitPushButton_clicked()
{
    QDialog::reject();
}
