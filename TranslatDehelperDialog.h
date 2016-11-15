#ifndef TRANSLATDEHELPERDIALOG_H
#define TRANSLATDEHELPERDIALOG_H

#include <QDialog>

namespace Ui {
class TranslatDehelperDialog;
}

class TranslatDehelperDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit TranslatDehelperDialog(QWidget *parent = 0);
    ~TranslatDehelperDialog();

public:
    QString getXMLName();
    QString getXMLPath();

private slots:
    void on_OpenButton_clicked();

    void on_OKButton_clicked();

    void on_QuitButton_clicked();

private:
    Ui::TranslatDehelperDialog *ui;

    QString XMLName;
    QString XMLPath;
};

#endif // TRANSLATDEHELPERDIALOG_H
