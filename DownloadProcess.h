#ifndef DOWNLOADPROCESS_H
#define DOWNLOADPROCESS_H

#include <QThread>
#include "WordListDB.h"
#include "QNetworkAccessManager"
#include "QUrl"
#include "QFile"
#include "QTime"
#include "QNetworkReply"
class DownloadProcess : public QThread
{
    Q_OBJECT
public:
    explicit DownloadProcess(const int &start,
                             const int &end,
                             WordListDB *wordlist,
                             QObject *parent = 0);
    ~DownloadProcess();
signals:
    void downloadNew();
    void downloadEnded();

    void NetWorkUnavailable();
    void fileOpenError();

private slots:
    void httpFinished();  //完成下载后的处理
    void httpReadyRead();  //接收到数据时的处理
    void haveError(QNetworkReply::NetworkError error);//网络有问题的处理

    void download();

protected:
    virtual void run();

private:
    WordListDB *wordList;
    int startID;
    int endID;

    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QUrl url;   //存储网络地址
    QFile *file;  //文件指针
    QString soundPath;
    QString wordText;

    QTime starttime;
    int flag;
    
};

#endif // DOWNLOADPROCESS_H
