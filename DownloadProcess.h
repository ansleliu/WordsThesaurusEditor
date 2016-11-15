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
    void httpFinished();  //������غ�Ĵ���
    void httpReadyRead();  //���յ�����ʱ�Ĵ���
    void haveError(QNetworkReply::NetworkError error);//����������Ĵ���

    void download();

protected:
    virtual void run();

private:
    WordListDB *wordList;
    int startID;
    int endID;

    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QUrl url;   //�洢�����ַ
    QFile *file;  //�ļ�ָ��
    QString soundPath;
    QString wordText;

    QTime starttime;
    int flag;
    
};

#endif // DOWNLOADPROCESS_H
