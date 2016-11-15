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
    return requestInfo;
}
