#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QProgressDialog>
#include "editordialog.h"
#include "aboutdialog.h"
#include "addnewrecorddialog.h"
#include "importexceldialog.h"
#include "CreatWordListDB.h"
#include "ImportExcel.h"
#include "ImportTxt.h"
#include "TranslatDehelperBackup.h"
#include "TranslatDehelperDialog.h"
#include "StandardDB.h"
#include "WordListCreate.h"
#include "DownloadGoogleDialog.h"
#include "DownloadProcess.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void displayTime(void);

    void saveRecord();

    void threadAddNewWord();
    void transformEnd();

    void threadStandardNewRecord();
    void threadStandardEnd();

    void threadAutoFillingNewRecord();
    void threadAutoFillingEnd();

    void threadCreatWordListEnded();
    void threadAddNewRecord();

    void threadDownloadEnded();
    void threadDownloadNewRecord();
    void networkunable();

private slots:
    void loadBook();

    void on_newRecordButton_clicked();

    void on_deleteRecordButton_clicked();

    void on_showAllButton_clicked();

    void on_ascendingOrderButton_clicked();

    void on_descendingOrderButton_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_idSelectradioButton_clicked();

    void on_wortSelectradioButton_clicked();

    void on_action_about_triggered();

    void on_action_import_triggered();

    void on_action_export_triggered();

    void on_action_QT_triggered();

    void on_action_quit_triggered();

    void on_goTolineEdit_textChanged(const QString &keyWort);

    void on_action_TXT_triggered();

    void on_action_GoogleTTS_triggered();

    void on_action_Standard_triggered();

    void on_action_change_triggered();

    void on_action_AutoLektion_triggered();

    void on_action_givemoney_triggered();

    void on_action_translation_triggered();

private:
    void reloadBook();
    bool addNewRow(const uint &wid,
                   const QString &wort,
                   const QString &mean,
                   const QString &connection,
                   const uint lektion);
    bool addNewRecord(const QSqlRecord &record);

private:
    Ui::MainWindow *ui;
    QSqlDatabase *db;
    CreatWordListDB *wordListDB;
    QSqlTableModel *model;

    QString asIdSelect;
    QString asWortSelect;
    QString databaseName;
    QString tableName;
    QString descendingOrder;

    int wordid;

    WordListCreate * wordList;
    WordListDB *M_WordList;
};

#endif // MAINWINDOW_H
