#ifndef IMPORTTXT_H
#define IMPORTTXT_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include "CreatWordListDB.h"
#include <QThread>

class ImportTxt : public QThread
{
    Q_OBJECT
public:
    explicit ImportTxt(const QString &filepath,
                       CreatWordListDB *wordlistdb,
                       QObject *parent = 0);
    ~ImportTxt();
    
signals:
    void addNewWord();
    void transformEnded();

public slots:

public:
    int getWordCount();

protected:
    virtual void run();

private:
    QFile *txtFile;
    QTextStream *textStream;

    CreatWordListDB *wordListDB;
};

#endif // IMPORTTXT_H
