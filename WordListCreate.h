#ifndef WORDLISTCREATE_H
#define WORDLISTCREATE_H

#include <QThread>
#include "WordListDB.h"
#include "CreatWordListDB.h"
#include "pathmanage.h"
#include"QSqlTableModel"

class WordListCreate : public QThread
{
    Q_OBJECT
public:
    explicit WordListCreate(QWidget *widget=0,
                            CreatWordListDB *wordlistdb = 0,
                            QObject *parent=0);
    ~WordListCreate();

    WordListDB * getWordList();
    QSqlTableModel*getModel();

signals:
    void creatWordListEnded();
    void addNewRecord();
public slots:

protected:
    virtual void run();

private:
    CreatWordListDB *wordListDB;
    WordListDB *wordList;

    QString fileName;
    QWidget *M_Widget;
};

#endif // WORDLISTCREATE_H
