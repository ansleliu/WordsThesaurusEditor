#include "StandardDB.h"

StandardDB::StandardDB(CreatWordListDB *wordlistdb, QObject *parent) :
    QThread(parent)
{
    wordListDB = wordlistdb;
    todie = "";
    todas = "";
    toder = "";
    moveadj = "";
    moveadv = "";
    movevt = "";
    movevi = "";
    movekonj = "";
    movepron = "";
}

StandardDB::~StandardDB()
{
    wordListDB = NULL;
}

void StandardDB::run()
{
    for(int i=1;i<=wordListDB->getRowCount();i++)
    {
        //////////////////////////////////////////////////////////
        //���һ����¼
        QStringList record = wordListDB->getRecord(i);
        QString word = record.at(0);
        QString mean = record.at(1);
//        QString connection = record.at(2);

        int size = word.size();
        ///////////////////////////////////////////////
        //�滻"e "
        if(word.startsWith(QString("%1 ").arg(todie)))
        {
            word.replace(0,1,"die");
        }
        ///////////////////////////////////////////////
        //�滻"r "
        if(word.startsWith(QString("%1 ").arg(toder)))
        {
            word.replace(0,1,"der");
        }
        ///////////////////////////////////////////////
        //�滻"s "
        if(word.startsWith(QString("%1 ").arg(todas)))
        {
            word.replace(0,1,"das");
        }
        //////////////////////////////////////////////
        //�滻" Adj."
        int indexAdj = word.indexOf(QString(" %1").arg(moveadj));
        if(indexAdj >0)
        {
            mean.insert(0,word.mid(indexAdj));
            word.replace(indexAdj,size-indexAdj,"");
        }
        //////////////////////////////////////////////
        //�滻" Adv."
        int indexAdv = word.indexOf(QString(" %1").arg(moveadv));
        if(indexAdv >0)
        {
            mean.insert(0,word.mid(indexAdv));
            word.replace(indexAdv,size-indexAdv,"");
        }
        //////////////////////////////////////////////
        //�滻" vt."
        int indexvt = word.indexOf(QString(" %1").arg(movevt));
        if(indexvt >0)
        {
            mean.insert(0,word.mid(indexvt));
            word.replace(indexvt,size-indexvt,"");
        }
        //////////////////////////////////////////////
        //�滻" vi."
        int indexvi = word.indexOf(QString(" %1").arg(movevi));
        if(indexvi >0)
        {
            mean.insert(0,word.mid(indexvi));
            word.replace(indexvi,size-indexvi,"");
        }
        //////////////////////////////////////////////
        //�滻" konj."
        int indexkonj = word.indexOf(QString(" %1").arg(movekonj));
        if(indexkonj >0)
        {
            mean.insert(0,word.mid(indexkonj));
            word.replace(indexkonj,size-indexkonj,"");
        }
        //////////////////////////////////////////////
        //�滻" Pron."
        int indexPraep = word.indexOf(QString(" %1").arg(movepron));
        if(indexPraep >0)
        {
            mean.insert(0,word.mid(indexPraep));
            word.replace(indexPraep,size-indexPraep,"");
        }
        //////////////////////////////////////////////
        //����
        wordListDB->updataMean(mean,i);
        wordListDB->updataWord(word,i);

        usleep(3000);
        emit standardNewRecord();
    }
    emit standardEnded();
}

void StandardDB::setStandardItems(const QStringList &standarditems)
{
    todie = standarditems.at(0);
    todas = standarditems.at(1);
    toder = standarditems.at(2);
    moveadj = standarditems.at(3);
    moveadv = standarditems.at(4);
    movevt = standarditems.at(5);
    movevi = standarditems.at(6);
    movekonj = standarditems.at(7);
    movepron = standarditems.at(8);
}
