#ifndef ADDNEWRECORDDIALOG_H
#define ADDNEWRECORDDIALOG_H

#include <QDialog>

namespace Ui {
class AddNewRecordDialog;
}

class AddNewRecordDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AddNewRecordDialog(QWidget *parent = 0, const int &rowcount = 0);
    ~AddNewRecordDialog();

public:
    QString getWord();
    QString getMean();
    QString getOther();
    int getLektion();
    
private slots:
    void on_CloseButton_clicked();

    void on_OKButton_clicked();

private:
    Ui::AddNewRecordDialog *ui;
    int rowCount;
};

#endif // ADDNEWRECORDDIALOG_H
