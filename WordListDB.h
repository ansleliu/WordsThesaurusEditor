#ifndef WORDLISTDB_H
#define WORDLISTDB_H
#include "MySQLite.h"
#include <QSqlTableModel>
#include <QWidget>

class WordListDB : public MySQLite
{
public:
    WordListDB(const QString &dbpath,
               const QString &connName,
               const QString &DBName);
    ~WordListDB();

protected:
    bool createTable();

public:
    bool addNewRecord(const QString &word,
                      const QString &soundpath);
    QString getWord(const int &wid);

    QString getSoundPath(const QString &word);
    bool updateSoundPath(const QString &word, const QString &soundpath);

    QSqlDatabase *getdb();

    int getDownloadCount();
    int getRowCount();
    QSqlTableModel *wordListDBModel(QWidget *parent,
                                    const QString &tableName);

private:
    QSqlTableModel *DBModel;
};

#endif // WORDLISTDB_H
