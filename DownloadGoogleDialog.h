#ifndef DOWNLOADGOOGLEDIALOG_H
#define DOWNLOADGOOGLEDIALOG_H

#include <QDialog>
#include "WordListDB.h"

namespace Ui {
class DownloadGoogleDialog;
}

class DownloadGoogleDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit DownloadGoogleDialog(WordListDB*wordlist,
                                  QWidget *parent = 0);
    ~DownloadGoogleDialog();

    int getStartID();
    int getEndID();
    
private slots:
    void on_EndSpinBox_valueChanged(int arg1);

    void on_OKPushButton_clicked();

    void on_QuitPushButton_clicked();

private:
    Ui::DownloadGoogleDialog *ui;
    WordListDB*wordList;

    int startID;
    int endID;
};

#endif // DOWNLOADGOOGLEDIALOG_H
