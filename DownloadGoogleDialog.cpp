#include "DownloadGoogleDialog.h"
#include "ui_DownloadGoogleDialog.h"

DownloadGoogleDialog::DownloadGoogleDialog(WordListDB *wordlist,
                                           QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DownloadGoogleDialog)
{
    ui->setupUi(this);
    wordList = wordlist;

    int downloadCount = wordList->getDownloadCount();
    startID = downloadCount;

    int wordCount = wordList->getRowCount();

    ui->StartLineEdit->setText(QString::number(downloadCount,'i',0));
    ui->EndSpinBox->setMinimum(downloadCount+(wordCount-downloadCount)/2);
    ui->EndSpinBox->setMaximum(wordCount);
    endID = 0;
}

DownloadGoogleDialog::~DownloadGoogleDialog()
{
    delete ui;
}

int DownloadGoogleDialog::getStartID()
{
    return startID;
}

int DownloadGoogleDialog::getEndID()
{
    return endID;
}

void DownloadGoogleDialog::on_EndSpinBox_valueChanged(int arg1)
{
    endID = arg1;
}

void DownloadGoogleDialog::on_OKPushButton_clicked()
{
    accept();
}

void DownloadGoogleDialog::on_QuitPushButton_clicked()
{
    reject();
}
