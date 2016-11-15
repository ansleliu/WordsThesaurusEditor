#include "DownloadProcess.h"
#include "pathmanage.h"
#include "QMutex"
#include "QDebug"
DownloadProcess::DownloadProcess(const int &start,
                                 const int &end,
                                 WordListDB *wordlist,
                                 QObject *parent) :
    QThread(parent)
{
    wordList = wordlist;
    startID = start;
    endID = end;
    flag = 0;
    PathManage::createRelExeDir(QString("/WortschatzEditor/GoogleTTSVoice/"));
}

DownloadProcess::~DownloadProcess()
{
    wordList = NULL;
}

void DownloadProcess::run()
{
    for(int i = startID; i<= endID; i++)
    {
        wordText = wordList->getWord(i);
        download();
    }
    emit downloadEnded();
}

void DownloadProcess::httpFinished()
{
    ///////////////////////////
    //�������
    QMutex mutex;
    mutex.lock();

    file->flush();
    file->close();

    reply->deleteLater();
    reply = NULL;

    file->deleteLater();
    file = NULL;

    mutex.unlock();
    qDebug() << "�������";
    flag = 0;
    emit downloadNew();
}

void DownloadProcess::httpReadyRead()
{
    if ( !file )
    {
        qDebug() << "�ļ�ָ�붪ʧ";
        return;
    }
    QMutex mutex;
    mutex.lock();
    QByteArray buffer = reply->readAll();
    file->write(buffer);
    mutex.unlock();
}

void DownloadProcess::haveError(QNetworkReply::NetworkError error)
{
    if(flag > 10)
    {
        disconnect(reply, SIGNAL(readyRead()),
                this, SLOT(httpReadyRead()));
        disconnect(reply, SIGNAL(finished()),
                this, SLOT(httpFinished()));
        disconnect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
                this, SLOT(haveError(QNetworkReply::NetworkError)));
        qDebug() << "�������"<< error;
        emit NetWorkUnavailable();
    }
    else
    {
        for(int i=flag;i<10;i++)
        {
            flag++;
            download();
        }
    }
}

void DownloadProcess::download()
{
    QString strUrl = QString("http://translate.google.com/translate_tts?tl=de&q=%1")
                            .arg(wordText);
    QUrl url(strUrl);

    //////////////////////////////////////////////////////////////
    //
    if(wordText.contains(QRegExp("[\?\\*\\:<>\\|]")))
    {
        wordText.remove(QRegExp("[\?\\*\\:<>\\|]"));
    }

    soundPath = PathManage::makePathStr(QString("/WortschatzEditor/GoogleTTSVoice/%2.mp3")
                                        .arg(wordText));
    file = new QFile(soundPath);
    if(file->exists())
    {
        qDebug() << "�ļ�����";
        file->remove(soundPath);
    }
    else
    {
        if(!file->open(QIODevice::WriteOnly))
        {
            qDebug() << "�ļ���ʧ��";
            delete file;
            file = 0;

            emit fileOpenError();
            return;
        }
        /////////////////////////////////////////////////////////////////
        //
        QEventLoop loop;
        reply = manager->get(QNetworkRequest(url));
        connect(reply, SIGNAL(readyRead()),
                this, SLOT(httpReadyRead()));

        connect(reply, SIGNAL(finished()),
                this, SLOT(httpFinished()));
        connect(reply, SIGNAL(finished()),
                         &loop, SLOT(quit()));

        connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
                this, SLOT(haveError(QNetworkReply::NetworkError)));
        loop.exec();
    }
}
