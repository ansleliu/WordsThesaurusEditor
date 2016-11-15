#include "ImportTxt.h"
#include <QMessageBox>
#include <QFileInfo>
#include <QDebug>
#include <QProgressDialog>

ImportTxt::ImportTxt(const QString &filepath,
                     CreatWordListDB *wordlistdb,
                     QObject *parent)
    : QThread(parent)
{
    QFileInfo fileInfo(filepath);
    QString fileName = fileInfo.baseName();
    ///////////////////////////////////////////////////////////
    //
    txtFile = new QFile(filepath);
    if(!txtFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(NULL, tr(" 错误 "),
                             QString("文件 %1 打开失败！\n %2")
                                .arg(fileName)
                                .arg(txtFile->errorString()));
        qDebug() << QString("file %1 open false").arg(fileName)
                 << txtFile->errorString();
    }
    else
    {
        qDebug() << QString("file: %1 have been opened").arg(fileName);
        textStream = new QTextStream(txtFile);
    }

    wordListDB = wordlistdb;
}

ImportTxt::~ImportTxt()
{
    txtFile->close();
    delete txtFile;
    qDebug() << "txtFile delete";
    txtFile = NULL;

    delete textStream;
    qDebug() << "textStream delete";
    textStream = NULL;

    wordListDB = NULL;
}

int ImportTxt::getWordCount()
{
    int wordcount = 0;
    while(!textStream->atEnd())
    {
        QString text = textStream->readLine();
        if(text.contains("</>"))
        {
            wordcount++;
        }
    }
    ////////////////////////////////////////////
    //返回头部
    textStream->seek(0);
    qDebug() << QString("word count %1").arg(wordcount);
    return wordcount;
}

void ImportTxt::run()
{
    qDebug() << "start to transform txtfile to dbfile";
    int count = 0;
    int wordcount = 0;
    int lektion = 0;

    QString word = "";
    while(!textStream->atEnd())
    {
        count++;
        qDebug() << QString("transform line : %1 ").arg(count);
        QString text = textStream->readLine();
        //////////////////////////////////////////////////////
        //
        if(!text.contains("</>"))
        {
            word.append(text);
        }
        else if(text.contains("</>"))
        {
            wordcount++;
            QStringList info =  word.split(QRegExp("`[0-9]`"));
            QStringList WordMean;
            for(int flag=0;flag<info.size();flag++)
            {
                QString textinfo = info.at(flag);
                qDebug() << QString("info at %1  : %2").arg(flag).arg(textinfo);

                if(textinfo.contains(QRegExp("<[a-zA-Z]+[1-9]?[^><]*>")) &&
                        textinfo.contains(QRegExp("</[a-zA-Z]+[1-9]?>")))
                {
                    textinfo.replace(QRegExp("<[a-zA-Z]+[1-9]?[^><]*>"),"");
                    textinfo.replace(QRegExp("</[a-zA-Z]+[1-9]?>"),"    ");
                    ///////////////////////////////////////////////////////////
                    //获取词义
                    WordMean.append(textinfo);
                }
                else
                {
                    WordMean.append(textinfo);
                }
                qDebug() << QString("info at %1  : %2").arg(flag).arg(textinfo);
            }

            ///////////////////////////////////////////////////////
            //获取单词
            QString Verification = WordMean.at(0);
            QString Mean = WordMean.at(2);
            WordMean.clear();

            qDebug() << QString("get word : %1 ").arg(wordcount);
            qDebug() << QString("Verificat word: %1 ").arg(Verification);
            ///////////////////////////////////////////////////////////
            lektion = (wordcount+1)/200 + 1;
            //保存单词到数据库中
            wordListDB->addNewRecord(Verification,Mean,"",lektion);

            word.clear();

            usleep(800);
            emit addNewWord();
        }
        else
        {
             qDebug() << text;
        }
                                                              ///
        /////////////////////////////////////////////////////////
        /*
        if(text.contains("<br>"))
        {
            wordcount++;
            QStringList info = text.split("<br>");
            {
                ///////////////////////////////////////////////////////////
                //验证单词
                QStringList wort = info.at(0).split(QRegExp("`[1-2]`"));
                QString Verification = wort.at(1);
                ///////////////////////////////////////////////////////
                //如果是该单词的话
                qDebug() << QString("get word : %1 ").arg(wordcount);
                qDebug() << QString("Verificat word: %1 ").arg(Verification);
                ///////////////////////////////////////////////////////////
                //获取词义
                QString mean = "";
                for(int i=1;i<info.size();i++)
                {
                    mean.append(info.at(i)+"\n");
                }
                ///////////////////////////////////////////////////////////
                lektion = (wordcount+1)/200 + 1;
                //保存单词到数据库中
                wordListDB->addNewRecord(Verification,mean,"",lektion);
                usleep(2000);
                emit addNewWord();
            }
        }
        */
        //////////////////////////////////////////////////////////////////
    }
    emit transformEnded();
}
