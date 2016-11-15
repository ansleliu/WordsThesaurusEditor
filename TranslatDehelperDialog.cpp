#include "TranslatDehelperDialog.h"
#include "ui_TranslatDehelperDialog.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>

TranslatDehelperDialog::TranslatDehelperDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TranslatDehelperDialog)
{
    ui->setupUi(this);

    XMLName = "";
    XMLPath = "";
    ui->OKButton->setEnabled(false);
}

TranslatDehelperDialog::~TranslatDehelperDialog()
{
    delete ui;
}

QString TranslatDehelperDialog::getXMLName()
{
    return XMLName;
}

QString TranslatDehelperDialog::getXMLPath()
{
    return XMLPath;
}

void TranslatDehelperDialog::on_OpenButton_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this,
            tr("打开德语助手生词本XML文件"), QString("./WortschatzEditor/importtxt/"), tr("文本文件(*.xml)"));
    if (file_name.isEmpty())
    {
        return;
    }
    else if(!file_name.contains(".xml", Qt::CaseInsensitive))
    {
        QMessageBox::warning(this, tr("出错了"),
         tr("请把要转化的XML文件放到importtxt目录下并将其打开！"));
        return;
    }
    else
    {
        QFileInfo fd(file_name);
        XMLName = fd.baseName();
        XMLPath = fd.filePath();
        ui->PathLineEdit->setText(XMLPath);
        ui->OKButton->setEnabled(true);
    }
}

void TranslatDehelperDialog::on_OKButton_clicked()
{
    accept();
}

void TranslatDehelperDialog::on_QuitButton_clicked()
{
    reject();
}
