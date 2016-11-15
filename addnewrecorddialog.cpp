#include "addnewrecorddialog.h"
#include "ui_addnewrecorddialog.h"

AddNewRecordDialog::AddNewRecordDialog(QWidget *parent, const int &rowcount) :
    QDialog(parent),
    ui(new Ui::AddNewRecordDialog)
{
    ui->setupUi(this);

    rowCount = rowcount;
    int lektion = (rowCount+1)/200 + 1;
    ui->LektionLineEdit->setText(QString::number(lektion,'i',0));
}

AddNewRecordDialog::~AddNewRecordDialog()
{
    delete ui;
}

QString AddNewRecordDialog::getWord()
{
    return ui->WordLineEdit->text();
}

QString AddNewRecordDialog::getMean()
{
    return ui->MeanTextEdit->toPlainText();
}

QString AddNewRecordDialog::getOther()
{
    return ui->OtherTextEdit->toPlainText();
}

int AddNewRecordDialog::getLektion()
{
    return ui->LektionLineEdit->text().toInt();
}

void AddNewRecordDialog::on_CloseButton_clicked()
{
    QDialog::close();
}

void AddNewRecordDialog::on_OKButton_clicked()
{
    QDialog::accept();
}
