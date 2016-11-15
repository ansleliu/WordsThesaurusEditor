#include "importexceldialog.h"
#include "ui_importexceldialog.h"
#include "QMessageBox"
ImportExcelDialog::ImportExcelDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImportExcelDialog)
{
    ui->setupUi(this);
    ui->PathLineEdit->setEnabled(false);
}

ImportExcelDialog::~ImportExcelDialog()
{
    delete ui;
}

void ImportExcelDialog::setExcelName(const QString &excel)
{
    ui->PathLineEdit->setText(excel);
}

QString ImportExcelDialog::getExcelPath()
{
    return ExcelPath;
}

QString ImportExcelDialog::getExcelName()
{
    return ExcelName;
}

void ImportExcelDialog::on_OpenButton_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this,
            tr("打开Excel文件"), QString("./WortschatzEditor/importexcel/"), tr("Excel文件(*.xls)"));
    if (file_name.isEmpty())
        return;
    else if(!file_name.contains(".xls", Qt::CaseInsensitive))
    {
        QMessageBox::warning(this, tr("出错了"),
         tr("请把要转化的Excel文件放到importexcel目录下并将其打开！"));
        return;
    }
    else
    {
        QFileInfo fd(file_name);
        ExcelName = fd.baseName();
        ExcelPath = fd.filePath();
        setExcelName(ExcelName);
        ui->PathLineEdit->setEnabled(true);
    }
}

void ImportExcelDialog::on_OKButton_clicked()
{
    QDialog::accept();
}

void ImportExcelDialog::on_QuitButton_clicked()
{
    QDialog::close();
}
