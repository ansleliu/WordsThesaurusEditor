#ifndef STARTDIALOG_H
#define STARTDIALOG_H

#include <QDialog>
#include <Qpainter.h>
#include "QPixmap"
#include "QImage"
#include "QPicture"

namespace Ui {
class startDialog;
}

class startDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit startDialog(QWidget *parent = 0);
    ~startDialog();
    QString getDatabaseName();
    QString getTableName();

private slots:
    void on_openradioButton_clicked();

    void on_newradioButton_clicked();

    void on_openButton_clicked();

    void on_saveButton_clicked();

    void on_acceptButton_clicked();

    void on_quitButton_clicked();

private:
    Ui::startDialog *ui;
    QString DatabaseName;
    QString TableName;
};

#endif // STARTDIALOG_H
