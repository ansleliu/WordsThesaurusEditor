#include "AutomaticFillingUnit.h"

AutomaticFillingUnit::AutomaticFillingUnit(CreatWordListDB *wordlistdb,
                                             const int& worduni,
                                           QObject *parent) :
    QThread(parent)
{
    //////////////////////////////////
    //赋值
    wordListDB = wordlistdb;
    wordUni = worduni;

}

AutomaticFillingUnit::~AutomaticFillingUnit()
{
    wordListDB = NULL;
}

void AutomaticFillingUnit::run()
{
    for(int i=1;i<=wordListDB->getRowCount();i++)
    {
        ///////////////////////////////////////////////////////////
        uint lektion = (i)/wordUni + 1;
        qDebug() << QString("对第 %1 个单词进行单元编号").arg(i);

        wordListDB->updataLektion(QString::number(lektion,'i',0),i);
        usleep(3000);
        emit fillingNewWordUnit();
    }
    emit fillingEnded();
}
