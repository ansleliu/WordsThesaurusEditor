#ifndef EDITORDIALOG_H
#define EDITORDIALOG_H

#include <QDialog>

namespace Ui {
class editorDialog;
}

class editorDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit editorDialog(QWidget *parent = 0);
    ~editorDialog();

    QString getText();
    void setText(QString text);
    QString applyChange;
private slots:
    void on_clearButton_clicked();

    void on_applyButton_clicked();

    void on_closeButton_clicked();

private:
    Ui::editorDialog *ui;
};

#endif // EDITORDIALOG_H
