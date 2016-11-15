#include "editordialog.h"
#include "ui_editordialog.h"

editorDialog::editorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editorDialog)
{
    ui->setupUi(this);
    applyChange = "FALSE";

    ui->applyButton->setFocus();
}

editorDialog::~editorDialog()
{
    delete ui;
}
QString editorDialog::getText()
{
    QString getTheText;
    if(applyChange == "TRUE")
    {
        getTheText = ui->textEdit->toPlainText();
        applyChange == "FALSE";
    }
    return getTheText;
}

void editorDialog::setText(QString text)
{
    ui->textEdit->update();
    ui->textEdit->setText(text);
}

void editorDialog::on_clearButton_clicked()
{
    ui->textEdit->clear();
}

void editorDialog::on_applyButton_clicked()
{
    this->close();
    applyChange = "TRUE";
}

void editorDialog::on_closeButton_clicked()
{
    this->close();
    applyChange = "FALSE";
}
