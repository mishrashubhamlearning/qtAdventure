#ifndef QTFTPCLASS_H
#define QTFTPCLASS_H
#include <QNetworkAccessManager>
#include <QtNetwork>
#include <QDebug>

class shkmQtFtpCleint:public QObject{
Q_OBJECT
     QNetworkAccessManager *manager;
     QUrl                   ftpHostConfig;
     QString                hostConfgFtpurl;
     QFile                  *fileToSend;
     QString                locationToSaveFile=QString::null;
     unsigned char          downLoading=0U;
     int                    networkConnection=-1;
     QNetworkReply*         serverReply;
 public:
    explicit shkmQtFtpCleint(quint16 Hostport, QString hostFtPIp, QString userId, QString password);
    ~shkmQtFtpCleint(){

     }

    void sendFile(QString fileName);
    void browseFile(void);
    void downloadFile(QString fileName, QString filePathToSaveFile);
    int getNetworkConnectionStatus(){return networkConnection;}
signals:
    void dataTransRecvSize(qint64 percent);
    int dataTransRecvCompleted(void);

private slots:
    void trxProgessData(qint64 byteSent, qint64 byteTotal);
    void trxCompleted(QNetworkReply *reply);
    void networkSSlError(QNetworkReply *reply, const QList<QSslError> &errors);
    void networkConnected();
    void serverReplyError(QNetworkReply::NetworkError reply);
};
#endif // QTFTPCLASS_H
