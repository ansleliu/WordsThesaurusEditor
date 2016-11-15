#include "ImportExcel.h"

ImportExcel::ImportExcel(const QString &dbpath,
                         const QString &connName,
                         const QString &DBName)
    : MySQLite(dbpath, connName, DBName)
{

}

ImportExcel::~ImportExcel()
{

}

QSqlDatabase *ImportExcel::getdb()
{
    return db;
}

QSqlTableModel *ImportExcel::wordListDBModel(QWidget *parent,
                                             const QString &tableName)
{
    QSqlTableModel *wordListDBModel;
    wordListDBModel = new QSqlTableModel(parent,*db);
    wordListDBModel->setTable(tableName);
    wordListDBModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    wordListDBModel->select(); //选取整个表的所有行
    return wordListDBModel;
}

bool ImportExcel::createTable()
{
    db->transaction();


    db->commit();
    return true;
}
