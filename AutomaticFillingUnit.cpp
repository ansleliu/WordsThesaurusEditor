#include "AutomaticFillingUnit.h"

AutomaticFillingUnit::AutomaticFillingUnit(CreatWordListDB *wordlistdb,
                                             const int& worduni,
                                           QObject *parent) :
    QThread(parent)
{
    //////////////////////////////////
    //��ֵ
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
        qDebug() << QString("�Ե� %1 �����ʽ��е�Ԫ���").arg(i);

        wordListDB->updataLektion(QString::number(lektion,'i',0),i);
        usleep(3000);
        emit fillingNewWordUnit();
    }
    emit fillingEnded();
}
