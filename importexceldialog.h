#ifndef IMPORTEXCELDIALOG_H
#define IMPORTEXCELDIALOG_H

#include <QDialog>
#include <QFile>
#include<QFileDialog>
#include<QFileInfo>
#include<QString>

namespace Ui {
class ImportExcelDialog;
}

class ImportExcelDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ImportExcelDialog(QWidget *parent = 0);
    ~ImportExcelDialog();

public:
    void setExcelName(const QString &excel);
    QString getExcelPath();
    QString getExcelName();

private:
    QString ExcelName;
    QString ExcelPath;
    
private slots:
    void on_OpenButton_clicked();

    void on_OKButton_clicked();

    void on_QuitButton_clicked();

private:
    Ui::ImportExcelDialog *ui;
};

#endif // IMPORTEXCELDIALOG_H
