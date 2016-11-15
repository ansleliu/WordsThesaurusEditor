#ifndef STANDARDDB_H
#define STANDARDDB_H

#include <QThread>
#include "CreatWordListDB.h"
class StandardDB : public QThread
{
    Q_OBJECT
public:
    explicit StandardDB(CreatWordListDB *wordlistdb,
                        QObject *parent = 0);
    ~StandardDB();

signals:
    void standardNewRecord();
    void standardEnded();
    
public slots:

protected:
    virtual void run();

public:
    void setStandardItems(const QStringList &standarditems);

private:
    CreatWordListDB *wordListDB;

    QString todie;
    QString todas;
    QString toder;
    QString moveadj;
    QString moveadv;
    QString movevt;
    QString movevi;
    QString movekonj;
    QString movepron;
};

#endif // STANDARDDB_H
