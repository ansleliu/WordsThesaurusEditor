#include "ImportTXTDialog.h"
#include "ui_ImportTXTDialog.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>

ImportTXTDialog::ImportTXTDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImportTXTDialog)
{
    ui->setupUi(this);
    TXTName = "";
    TXTPath = "";
    ui->OKButton->setEnabled(false);
}

ImportTXTDialog::~ImportTXTDialog()
{
    delete ui;
}

QString ImportTXTDialog::getTXTName()
{
    return TXTName;
}

QString ImportTXTDialog::getTXTPath()
{
    return TXTPath;
}

void ImportTXTDialog::on_OpenButton_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this,
            tr("打开TXT文件"), QString("./WortschatzEditor/importtxt/"), tr("文本文件(*.txt)"));
    if (file_name.isEmpty())
    {
        return;
    }
    else if(!file_name.contains(".txt", Qt::CaseInsensitive))
    {
        QMessageBox::warning(this, tr("出错了"),
         tr("请把要转化的TXT文件放到importtxt目录下并将其打开！"));
        return;
    }
    else
    {
        QFileInfo fd(file_name);
        TXTName = fd.baseName();
        TXTPath = fd.filePath();
        ui->PathLineEdit->setText(TXTPath);
        ui->OKButton->setEnabled(true);
    }
}

void ImportTXTDialog::on_OKButton_clicked()
{
    accept();
}

void ImportTXTDialog::on_QuitButton_clicked()
{
    reject();
}
