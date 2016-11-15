#include "WordListCreate.h"

WordListCreate::WordListCreate(QWidget *widget,CreatWordListDB *wordlistdb,
                               QObject *parent) :
    QThread(parent)
{
    M_Widget = widget;
    PathManage::createRelExeDir(QString("/WortschatzEditor/WordList/"));
    wordListDB = wordlistdb;
    fileName = wordListDB->getWordListName();
    QString udatadbPath = PathManage::makePathStr(QString("/WortschatzEditor/WordList/%1")
                                                  .arg(fileName));
    wordList = new WordListDB(udatadbPath,"WORDLIST","QSQLITE");
}

WordListCreate::~WordListCreate()
{
    delete wordList;
    wordList = NULL;

    wordListDB = NULL;
}

WordListDB *WordListCreate::getWordList()
{
    return wordList;
}

QSqlTableModel *WordListCreate::getModel()
{
    return wordList->wordListDBModel(M_Widget,"WordList");
}

void WordListCreate::run()
{
    int count = wordListDB->getRowCount();
    for(int i=1;i<=count;i++)
    {
        QStringList wordall = wordListDB->getRecord(i);
        QString word = wordall.at(0);

        wordList->addNewRecord(word,"");
        usleep(2000);
        emit addNewRecord();
    }
    emit creatWordListEnded();
}
