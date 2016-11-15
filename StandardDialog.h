#ifndef STANDARDDIALOG_H
#define STANDARDDIALOG_H

#include <QDialog>

namespace Ui {
class StandardDialog;
}

class StandardDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit StandardDialog(QWidget *parent = 0);
    ~StandardDialog();
    
private slots:
    void on_AcceptButton_clicked();

    void on_QuitButton_clicked();

public:
    QStringList getStandardItems();

private:
    Ui::StandardDialog *ui;

    QString todie;
    QString todas;
    QString toder;
    QString moveadj;
    QString moveadv;
    QString movevt;
    QString movevi;
    QString movekonj;
    QString movepron;
};

#endif // STANDARDDIALOG_H
