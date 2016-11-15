#ifndef AUTOFILLINGUNITDIALOG_H
#define AUTOFILLINGUNITDIALOG_H

#include <QDialog>

namespace Ui {
class AutoFillingUnitDialog;
}

class AutoFillingUnitDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AutoFillingUnitDialog(QWidget *parent = 0);
    ~AutoFillingUnitDialog();
    
private slots:
    void on_OKButton_clicked();

    void on_quiteButton_clicked();

public:
    int returnWordNumUnit();

private:
    Ui::AutoFillingUnitDialog *ui;
};

#endif // AUTOFILLINGUNITDIALOG_H
