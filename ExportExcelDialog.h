#ifndef EXPORTEXCELDIALOG_H
#define EXPORTEXCELDIALOG_H

#include <QDialog>

namespace Ui {
class ExportExcelDialog;
}

class ExportExcelDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ExportExcelDialog(QString filename, QWidget *parent = 0);
    ~ExportExcelDialog();
    
private slots:
    void on_OKPushButton_clicked();

    void on_QuitPushButton_clicked();

public:
    QString getFilePath();

private:
    Ui::ExportExcelDialog *ui;
    QString fileName;
};

#endif // EXPORTEXCELDIALOG_H
