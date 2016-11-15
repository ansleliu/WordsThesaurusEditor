#ifndef IMPORTEXCEL_H
#define IMPORTEXCEL_H
#include "MySQLite.h"
#include <QSqlTableModel>

class ImportExcel : public MySQLite
{
public:
    ImportExcel(const QString &dbpath,
                const QString &connName,
                const QString &DBName);
    ~ImportExcel();

public:
    QSqlDatabase *getdb();
    QSqlTableModel *wordListDBModel(QWidget *parent,const QString &tableName);

protected:
    bool createTable();	// ´´½¨±í
};

#endif // IMPORTEXCEL_H
