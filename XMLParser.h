#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <QObject>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QDebug>
#include <QString>
#include <QStringList>
#include "CreatWordListDB.h"
#include <QFile>

class XMLParser : public QObject
{
    Q_OBJECT
public:
    explicit XMLParser(QFile *xml,
                       CreatWordListDB *wordlistdb,
                       QObject *parent = 0);
    
signals:
    
public slots:
    QStringList requestInfo(const QString &element,
                            const QString &Attribute1,
                            const QString &Attribute2,
                            const QString &Attribute3);
private:
    QFile *xmlText;
    CreatWordListDB *wordListDB;
};

#endif // XMLPARSER_H
