#include "XMLParser.h"
#include <QMessageBox>

XMLParser::XMLParser(QFile *xml,
                     CreatWordListDB *wordlistdb,
                     QObject *parent) :
    QObject(parent)
{
    xmlText = xml;
    wordListDB = wordlistdb;
}

QStringList XMLParser::requestInfo(const QString &element,
                               const QString &Attribute1,
                               const QString &Attribute2,
                               const QString &Attribute3)
{
    QXmlStreamReader reader(xmlText);
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
                type = reader.readNext();
                /////////////////////////////////////////////////////////////////////////
                //
                if (type == QXmlStreamReader::Characters && !reader.isWhitespace())
                {
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
                }
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
    return requestInfo;
}
