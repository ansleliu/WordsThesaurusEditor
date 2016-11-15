#include<QMessageBox>
#include <QFile>
#include<QFileDialog>
#include<QFileInfo>
#include<QString>
#include "startdialog.h"
#include "ui_startdialog.h"
#include "pathmanage.h"
startDialog::startDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::startDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("欢迎使用");
    this->setWindowIcon(QIcon("./rc/edit.ico"));

    PathManage::createRelExeDir("/WortschatzEditor/database/");

    ui->openButton->hide();
    ui->addlineEdit->hide();

    ui->fileNamelineEdit->hide();
    ui->tableNamelineEdit->hide();
    ui->label->hide();
    ui->tableNamelabel->hide();
    ui->saveButton->hide();

    ui->acceptButton->setEnabled(false);

    DatabaseName = "";
    TableName = "";
}

startDialog::~startDialog()
{
    delete ui;
}

QString startDialog::getDatabaseName()
{

    return DatabaseName;
}

QString startDialog::getTableName()
{
    return TableName;
}

void startDialog::on_openradioButton_clicked()
{
    ui->germanlabel->hide();
    ui->addlineEdit->show();
    ui->addlineEdit->setReadOnly(true);
    ui->openButton->show();

    ui->fileNamelineEdit->hide();
    ui->tableNamelineEdit->hide();
    ui->label->hide();
    ui->tableNamelabel->hide();
    ui->saveButton->hide();

    ui->openButton->setFocus();

//    ui->acceptButton->setEnabled(false);
}

void startDialog::on_newradioButton_clicked()
{
    ui->germanlabel->hide();
    ui->openButton->hide();
    ui->addlineEdit->hide();

    ui->fileNamelineEdit->show();
    ui->tableNamelineEdit->show();
    ui->label->show();
    ui->tableNamelabel->show();
    ui->saveButton->show();

    ui->saveButton->setFocus();

//    ui->acceptButton->setEnabled(true);

}

void startDialog::on_openButton_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this,
            tr("打开数据库"), QString("./WortschatzEditor/database/"), tr("数据库文件(*.db)"));
    if (file_name.isEmpty())
        return;
    else if(!file_name.contains(".db", Qt::CaseInsensitive))
    {
        QMessageBox::warning(this, tr("出错了"),
         tr("请把数据库文件放到database目录下并将其打开！"));
        return;
    }
    else
    {
        QFileInfo fd(file_name);
        DatabaseName=fd.filePath();
        ui->addlineEdit->setText(DatabaseName);
        ui->acceptButton->setEnabled(true);
        ui->acceptButton->setFocus();
    }
}

void startDialog::on_saveButton_clicked()
{
    QString dbName = ui->fileNamelineEdit->text();
    QString tabName = ui->tableNamelineEdit->text();
    if(dbName == "" && tabName == "")
    {
        QMessageBox::warning(this, tr("出错了"),
         tr("请填写词库名和表名！"));
    }
    else
    {
        DatabaseName = "./WortschatzEditor/database/" + ui->fileNamelineEdit->text() + ".db";
        TableName = ui->tableNamelineEdit->text();
        ui->acceptButton->setEnabled(true);
        ui->acceptButton->setFocus();
    }
    ui->saveButton->setEnabled(false);
    ui->fileNamelineEdit->setReadOnly(true);
}

void startDialog::on_acceptButton_clicked()
{
    accept();
}

void startDialog::on_quitButton_clicked()
{
    close();
}

