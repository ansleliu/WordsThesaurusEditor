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
        QMessageBox::warning(NULL, tr(" ���� "),
                             QString("�ļ� %1 ��ʧ�ܣ�\n %2")
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

    // ���û�ж����ĵ���β������û�г��ִ���
    while(!reader.atEnd())
    {
        // ��ȡ��һ���Ǻ�
        QXmlStreamReader::TokenType type = reader.readNext();
        //�����ͷ����ñ���Ͱ汾
        if (type == QXmlStreamReader::StartDocument)
        {
            qDebug() << "<"
                     << reader.documentEncoding()
                     << reader.documentVersion()
                     << ">";
        }
        //����Ǳ�ǩ��ʼ
        if (type == QXmlStreamReader::StartElement)
        {
            qDebug() << "<" << reader.name() << ">";
            //�������Ҫ��ı�ǩ����ô��ñ�ǩ֮�������������Ϣ
            if("CustomizeListItem" == reader.name().toString())
            {
                type = reader.readNext();
                wordcount++;
                qDebug() << wordcount;
            }
        }
        //��ǩ����
        if (type == QXmlStreamReader::EndElement)
        {
            qDebug() << "</" << reader.name() << ">";
        }
        //�����ǩ��ʼ֮�е��ı���Ϣ
        if (type == QXmlStreamReader::Characters && !reader.isWhitespace())
        {
            qDebug() << reader.text();
        }
    }

    // �����ȡ�����г��ִ�����ô���������Ϣ
    if (reader.hasError())
    {
        qDebug() << "error: " << reader.errorString();
        QMessageBox::warning(NULL, tr(" ���� "),
                             QString("XML�ļ���ȡ�����г��ִ���\n�������Ϊ��\n %2")
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

    // ���û�ж����ĵ���β������û�г��ִ���
    while(!reader.atEnd())
    {
        // ��ȡ��һ���Ǻ�
        QXmlStreamReader::TokenType type = reader.readNext();
        //�����ͷ����ñ���Ͱ汾
        if (type == QXmlStreamReader::StartDocument)
        {
            qDebug() << "<"
                     << reader.documentEncoding()
                     << reader.documentVersion()
                     << ">";
        }
        //����Ǳ�ǩ��ʼ
        if (type == QXmlStreamReader::StartElement)
        {
            qDebug() << "<" << reader.name() << ">";
            //�������Ҫ��ı�ǩ����ô��ñ�ǩ֮�������������Ϣ
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
                //��ȡ����
                QString Verification = requestInfo.at(0);
                QString Mean = requestInfo.at(1);
                QString OtherInfo = requestInfo.at(2);
                requestInfo.clear();

                qDebug() << QString("get word : %1 ").arg(wordcount);
                qDebug() << QString("Verificat word: %1 ").arg(Verification);
                ///////////////////////////////////////////////////////////
                lektion = (wordcount+1)/100 + 1;
                //���浥�ʵ����ݿ���
                worddb->addNewRecord(Verification,Mean,OtherInfo,lektion);

                usleep(800);
                emit addNewWord();
            }
        }
        //��ǩ����
        if (type == QXmlStreamReader::EndElement)
        {
            qDebug() << "</" << reader.name() << ">";
        }
        //�����ǩ��ʼ֮�е��ı���Ϣ
        if (type == QXmlStreamReader::Characters && !reader.isWhitespace())
        {
            qDebug() << reader.text();
        }
    }

    // �����ȡ�����г��ִ�����ô���������Ϣ
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


