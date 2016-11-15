#ifndef TRANSLATDEHELPERBACKUP_H
#define TRANSLATDEHELPERBACKUP_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include "CreatWordListDB.h"
#include <QThread>

class TranslatDehelperBackup : public QThread
{
    Q_OBJECT
public:
    explicit TranslatDehelperBackup(const QString &filepath,
                                    CreatWordListDB *wordlistdb,
                                    QObject *parent = 0);
    ~TranslatDehelperBackup();

signals:
    void addNewWord();
    void transformEnded();

public slots:

public:
    int getWordCount();

    void XMLParser(QFile *xmlfile,
                   CreatWordListDB *worddb,
                   const QString &element,
                   const QString &Attribute1,
                   const QString &Attribute2,
                   const QString &Attribute3);

protected:
    virtual void run();

private:
    QFile *xmlFile;

    CreatWordListDB *wordListDB;
};

#endif // TRANSLATDEHELPERBACKUP_H
