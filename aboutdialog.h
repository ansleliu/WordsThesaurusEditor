#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
#include <Qpainter.h>
#include "QPixmap"
#include "QImage"
#include "QPicture"
namespace Ui {
class aboutDialog;
}

class aboutDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit aboutDialog(QWidget *parent = 0);
    ~aboutDialog();
    
private:
    Ui::aboutDialog *ui;
};

#endif // ABOUTDIALOG_H
