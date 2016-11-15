#ifndef AUTOMATICFILLINGUNIT_H
#define AUTOMATICFILLINGUNIT_H

#include <QThread>
#include "CreatWordListDB.h"
class AutomaticFillingUnit : public QThread
{
    Q_OBJECT
public:
    explicit AutomaticFillingUnit(CreatWordListDB *wordlistdb,
                                    const int &worduni,
                                   QObject *parent = 0);
    ~AutomaticFillingUnit();
    
signals:
    void fillingNewWordUnit();
    void fillingEnded();
    
public slots:

protected:
    virtual void run();

private:
    CreatWordListDB *wordListDB;

    int wordUni;
    
};

#endif // AUTOMATICFILLINGUNIT_H
