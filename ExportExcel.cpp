#include "ExportExcel.h"
#include "QDebug"

ExportExcel::ExportExcel(const QString &dbpath,
                         const QString &connName,
                         const QString &DBName) :
    MySQLite(dbpath, connName, DBName)
{
    createTable();
}

ExportExcel::~ExportExcel()
{

}

bool ExportExcel::addNewRecord(const int &wid,
                               const QString &word,
                               const QString &mean,
                               const QString &connection,
                               const int &lektion)
{
    qDebug() << QString("为EXCEL数据库添加一条记录");
    QString sql = QString("insert into [Sheet1] "
                          "(Wid, Wort, Bedeutung, Connection, Lektion)"
                          " values ('%1', '%2', '%3', '%4', '%5')")
            .arg(wid)
            .arg(word)
            .arg(mean)
            .arg(connection)
            .arg(lektion);
    qDebug() << QString("为EXCEL数据库添加一条记录结束");
    return exec(sql);
}

QSqlDatabase *ExportExcel::getdb()
{
    return db;
}

QSqlTableModel *ExportExcel::wordListDBModel(QWidget *parent,
                                             const QString &tableName)
{
    QSqlTableModel *wordListDBModel;
    wordListDBModel = new QSqlTableModel(parent,*db);
    wordListDBModel->setTable(tableName);
    wordListDBModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    wordListDBModel->select(); //选取整个表的所有行
    return wordListDBModel;
}

bool ExportExcel::createTable()
{
    db->transaction();
    qDebug() << QString("为EXCEL数据库创建表开始");
    QSqlQuery query(*db);
    query.exec("CREATE TABLE [Sheet1] (\
               [Wid] INTEGER, \
               [Wort] TEXT, \
               [Bedeutung] TEXT, \
               [Connection] TEXT, \
               [Lektion] INTEGER);");
    qDebug() << query.lastError();
    db->commit();
    return true;
}
