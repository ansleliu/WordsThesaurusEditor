#include "WordListDB.h"

WordListDB::WordListDB(const QString &dbpath,
                       const QString &connName,
                       const QString &DBName)
    : MySQLite(dbpath, connName, DBName)
{
    if (!isTableExist("WordList"))	// 新建的数据库,创建表
    {
        createTable();
    }
    DBModel = NULL;
}

WordListDB::~WordListDB()
{
    if(DBModel != NULL)
    {
        qDebug() << "移除DBModel";
        DBModel->deleteLater();
        DBModel = NULL;
    }

    qDebug() << "移除WordListDB";
}

bool WordListDB::createTable()
{
    db->transaction();
    QSqlQuery query(*db);
    query.exec("CREATE TABLE [WordList] (\
               [Wid] INTEGER NOT NULL ON CONFLICT FAIL PRIMARY KEY AUTOINCREMENT, \
               [Wort] TEXT, \
               [SoundPath] TEXT);");

    db->commit();
    return true;
}

bool WordListDB::addNewRecord(const QString &word,const QString &soundpath)
{
    db->transaction();
    QString sql = QString("insert into WordList "
                          "(Wort, SoundPath)"
                          " values ('%1', '%2')")
            .arg(word)
            .arg(soundpath);
    exec(sql);
    db->commit();
    return true;
}

QString WordListDB::getWord(const int &wid)
{
    QSqlQuery query(QString("select Wort from WordList where Wid = '%1'").arg(wid), *db);
    query.next();
    return query.record().value(0).toString();
}

QString WordListDB::getSoundPath(const QString &word)
{
    QSqlQuery query(QString("select SoundPath from WordList where Wort = '%1'").arg(word), *db);
    query.next();
    return query.record().value(0).toString();
}

bool WordListDB::updateSoundPath(const QString &word, const QString &soundpath)
{
    QString sql = QString("update WordList set SoundPath = '%1' where Wort = '%2'")
            .arg(soundpath)
            .arg(word);
    return exec(sql);
}

QSqlDatabase *WordListDB::getdb()
{
    return db;
}

int WordListDB::getDownloadCount()
{
    QString nullStr = QString("");
    QSqlQuery query(QString("select count(*) from WordList where SoundPath != '%1'")
                    .arg(nullStr), *db);
    query.next();
    return query.value(0).toInt();
}

int WordListDB::getRowCount()
{
    QSqlQuery query("select count(*) from WordList", *db);
    query.next();
    return query.value(0).toInt();
}

QSqlTableModel *WordListDB::wordListDBModel(QWidget *parent,
                                            const QString &tableName)
{
    DBModel = new QSqlTableModel(parent,*db);
    DBModel->setTable(tableName);
    DBModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    DBModel->select(); //选取整个表的所有行
    return DBModel;
}
