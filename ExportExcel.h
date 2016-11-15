#ifndef EXPORTEXCEL_H
#define EXPORTEXCEL_H
#include "MySQLite.h"
#include <QSqlTableModel>

class ExportExcel : public MySQLite
{
public:
    explicit ExportExcel(const QString &dbpath,
                         const QString &connName,
                         const QString &DBName);
    ~ExportExcel();
    
public:
    bool addNewRecord(const int &wid, const QString &word,
                      const QString &mean, const QString &connection, const int &lektion);
    QSqlDatabase *getdb();
    QSqlTableModel *wordListDBModel(QWidget *parent,const QString &tableName);

protected:
    bool createTable();	// ´´½¨±í
};

#endif // EXPORTEXCEL_H
