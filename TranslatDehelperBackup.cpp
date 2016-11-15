#include "TranslatDehelperBackup.h"
#include <QMessageBox>
#include <QFileInfo>
#include <QDebug>
#include <QProgressDialog>

TranslatDehelperBackup::TranslatDehelperBackup(const QString &filepath,
                                               CreatWordListDB *wordlistdb,
                                               QObject *parent) :
    QThread(parent)
{
    QFileInfo fileInfo(filepath);
    QString fileName = fileInfo.baseName();
    ///////////////////////////////////////////////////////////
    //
    xmlFile = new QFile(filepath);
    if(!xmlFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(NULL, tr(" 错误 "),
                             QString("文件 %1 打开失败！\n %2")
                                .arg(fileName)
                                .arg(xmlFile->errorString()));
        qDebug() << QString("file %1 open false").arg(fileName)
                 << xmlFile->errorString();
    }
    else
    {
        qDebug() << QString("file: %1 have been opened").arg(fileName);
    }

    wordListDB = wordlistdb;
}

TranslatDehelperBackup::~TranslatDehelperBackup()
{
    xmlFile->close();

    xmlFile->deleteLater();
    qDebug() << "txtFile delete";
    xmlFile = NULL;

    wordListDB = NULL;
}

int TranslatDehelperBackup::getWordCount()
{
    qDebug() << "start to count wordcount";
    int wordcount = 0;
    ////////////////////////////////////////////////////////////
    //
    QXmlStreamReader reader(xmlFile);

    // 如果没有读到文档结尾，而且没有出现错误
    while(!reader.atEnd())
    {
        // 读取下一个记号
        QXmlStreamReader::TokenType type = reader.readNext();
        //如果是头，获得编码和版本
        if (type == QXmlStreamReader::StartDocument)
        {
            qDebug() << "<"
                     << reader.documentEncoding()
                     << reader.documentVersion()
                     << ">";
        }
        //如果是标签开始
        if (type == QXmlStreamReader::StartElement)
        {
            qDebug() << "<" << reader.name() << ">";
            //如果是所要求的标签，那么获得标签之间包含的文字信息
            if("CustomizeListItem" == reader.name().toString())
            {
                type = reader.readNext();
                wordcount++;
                qDebug() << wordcount;
            }
        }
        //标签结束
        if (type == QXmlStreamReader::EndElement)
        {
            qDebug() << "</" << reader.name() << ">";
        }
        //输出标签起始之中的文本信息
        if (type == QXmlStreamReader::Characters && !reader.isWhitespace())
        {
            qDebug() << reader.text();
        }
    }

    // 如果读取过程中出现错误，那么输出错误信息
    if (reader.hasError())
    {
        qDebug() << "error: " << reader.errorString();
        QMessageBox::warning(NULL, tr(" 错误 "),
                             QString("XML文件读取过程中出现错误！\n错误代码为：\n %2")
                                .arg(reader.errorString()));
    }
    /////////////////////////////////////////////////////////////
    //
    qDebug() << QString("word count %1").arg(wordcount);

    xmlFile->close();

    return wordcount;
}

void TranslatDehelperBackup::XMLParser(QFile *xmlfile,
                                       CreatWordListDB *worddb,
                                       const QString &element,
                                       const QString &Attribute1,
                                       const QString &Attribute2,
                                       const QString &Attribute3)
{
    if(!xmlFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << QString("file open false")
                 << xmlFile->errorString();
    }
    else
    {
        qDebug() << QString("file have been opened");
    }
    qDebug() << "start to transform xmlfile to dbfile";
    int wordcount = 0;
    int lektion = 0;
    ////////////////////////////////////////////////////////////
    //
    QXmlStreamReader reader(xmlfile);
    QStringList requestInfo;

    // 如果没有读到文档结尾，而且没有出现错误
    while(!reader.atEnd())
    {
        // 读取下一个记号
        QXmlStreamReader::TokenType type = reader.readNext();
        //如果是头，获得编码和版本
        if (type == QXmlStreamReader::StartDocument)
        {
            qDebug() << "<"
                     << reader.documentEncoding()
                     << reader.documentVersion()
                     << ">";
        }
        //如果是标签开始
        if (type == QXmlStreamReader::StartElement)
        {
            qDebug() << "<" << reader.name() << ">";
            //如果是所要求的标签，那么获得标签之间包含的文字信息
            if(element == reader.name().toString())
            {
                wordcount++;
                qDebug() << reader.text();
                ///////////////////////////////////////////////////////////
                //
                if (reader.attributes().hasAttribute(Attribute1))
                {
                    qDebug() << reader.attributes().value(Attribute1);
                    requestInfo << reader.attributes().value(Attribute1).toString();
                }
                else
                {
                    requestInfo << " ";
                }
                ////////////////////////////////////////////////////////////
                //
                if (reader.attributes().hasAttribute(Attribute2))
                {
                    qDebug() << reader.attributes().value(Attribute2);
                    requestInfo << reader.attributes().value(Attribute2).toString();
                }
                else
                {
                    requestInfo << " ";
                }
                /////////////////////////////////////////////////////////////
                //
                if (reader.attributes().hasAttribute(Attribute3))
                {
                    qDebug() << reader.attributes().value(Attribute3);
                    requestInfo << reader.attributes().value(Attribute3).toString();
                }
                else
                {
                    requestInfo << " ";
                }
                ///////////////////////////////////////////////////////
                //获取单词
                QString Verification = requestInfo.at(0);
                QString Mean = requestInfo.at(1);
                QString OtherInfo = requestInfo.at(2);
                requestInfo.clear();

                qDebug() << QString("get word : %1 ").arg(wordcount);
                qDebug() << QString("Verificat word: %1 ").arg(Verification);
                ///////////////////////////////////////////////////////////
                lektion = (wordcount+1)/100 + 1;
                //保存单词到数据库中
                worddb->addNewRecord(Verification,Mean,OtherInfo,lektion);

                usleep(800);
                emit addNewWord();
            }
        }
        //标签结束
        if (type == QXmlStreamReader::EndElement)
        {
            qDebug() << "</" << reader.name() << ">";
        }
        //输出标签起始之中的文本信息
        if (type == QXmlStreamReader::Characters && !reader.isWhitespace())
        {
            qDebug() << reader.text();
        }
    }

    // 如果读取过程中出现错误，那么输出错误信息
    if (reader.hasError())
    {
        qDebug() << "error: " << reader.errorString();
    }
}

void TranslatDehelperBackup::run()
{
    XMLParser(xmlFile,wordListDB,"CustomizeListItem","word",NULL,NULL);
    usleep(800);
    emit transformEnded();
}


