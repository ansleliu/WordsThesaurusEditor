#ifndef IMPORTTXTDIALOG_H
#define IMPORTTXTDIALOG_H

#include <QDialog>

namespace Ui {
class ImportTXTDialog;
}

class ImportTXTDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ImportTXTDialog(QWidget *parent = 0);
    ~ImportTXTDialog();

public:
    QString getTXTName();
    QString getTXTPath();

private slots:
    void on_OpenButton_clicked();

    void on_OKButton_clicked();

    void on_QuitButton_clicked();

private:
    Ui::ImportTXTDialog *ui;

    QString TXTName;
    QString TXTPath;
};

#endif // IMPORTTXTDIALOG_H
