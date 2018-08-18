#include "qtftpClientclass.h"

shkmQtFtpCleint::shkmQtFtpCleint(quint16 Hostport, QString hostFtPIp,
                                 QString userId, QString password):QObject()
{
    hostConfgFtpurl =QString::null;
    manager = new QNetworkAccessManager(this);
    serverReply=manager->head(QNetworkRequest(QUrl()));
    hostConfgFtpurl.append("ftp://");
    hostConfgFtpurl.append(hostFtPIp);
    ftpHostConfig.setPort(Hostport);
    ftpHostConfig.setPassword(password);
    ftpHostConfig.setUserName(userId);
    connect(manager,&QNetworkAccessManager::networkSessionConnected,this,&shkmQtFtpCleint::networkConnected);
    connect(manager,&QNetworkAccessManager::finished,this,&shkmQtFtpCleint::trxCompleted);
    connect(manager,&QNetworkAccessManager::sslErrors,this,&shkmQtFtpCleint::networkSSlError);
    connect(serverReply,static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error),
            this,&shkmQtFtpCleint::serverReplyError);

}

void shkmQtFtpCleint::networkConnected()
{
    networkConnection=0;
    qDebug()<<"Network Connected";
}

void shkmQtFtpCleint::serverReplyError(QNetworkReply::NetworkError reply)
{

    switch(reply)
    {
    case QNetworkReply::ConnectionRefusedError:
        qDebug()<<"Connection Refused";
        break;
    case QNetworkReply::RemoteHostClosedError:
        qDebug()<<"Remote Host Closed";
        break;
    case QNetworkReply::HostNotFoundError:
        qDebug()<<"Host Not Found";
        break;
    case QNetworkReply::TimeoutError:
        qDebug()<<"Timeout";
        break;
    case QNetworkReply::OperationCanceledError:
        qDebug()<<"Operation cancelled";
        break;
    case QNetworkReply::SslHandshakeFailedError:
        qDebug()<<"SSL Hand Shake Failed";
        break;
    case QNetworkReply::TemporaryNetworkFailureError:
        qDebug()<<"Network Not Available";
        break;
    case QNetworkReply::NetworkSessionFailedError:
        qDebug()<<"Network Session Failed Error";
        break;
    case QNetworkReply::BackgroundRequestNotAllowedError:
        qDebug()<<"Back ground Request Not allowed";
        break;
    case QNetworkReply::UnknownNetworkError:
        qDebug()<<"Unknown Network Error";
        break;
    case QNetworkReply::ProxyConnectionRefusedError:
        qDebug()<<"Proxy Connection Refused"; break;
    case QNetworkReply::ProxyConnectionClosedError:
        qDebug()<<"Proxy Connection closed"; break;
    case QNetworkReply::ProxyNotFoundError:
        qDebug()<<"Proxy Not found Error";
        break;
    case QNetworkReply::ProxyTimeoutError:
        qDebug()<<"Proxy Timeout";break;
    case QNetworkReply::ProxyAuthenticationRequiredError:
        qDebug()<<"proxy Authentication Required";break;
    case QNetworkReply::UnknownProxyError:
        qDebug()<<"Unknown Proxy";
        break;
    case QNetworkReply::ContentAccessDenied:
        qDebug()<<"Content Access Denied";
        break;
    case QNetworkReply::ContentOperationNotPermittedError:
        qDebug()<<"Content Operation Not Permitted";
        break;
    case QNetworkReply::ContentNotFoundError:
        qDebug()<<"content Not Found";
        break;
    case QNetworkReply::AuthenticationRequiredError:
        qDebug()<<"Authentication Required";break;
    case QNetworkReply::ContentReSendError:
        qDebug()<<"Content Re send";
        break;
    case QNetworkReply::ContentConflictError:
        qDebug()<<"Duplicate content"; break;
    case QNetworkReply::ContentGoneError:
        qDebug()<<"Content Error"; break;
    case QNetworkReply::UnknownContentError:
        qDebug()<<"Unknown Content"; break;
    case QNetworkReply::ProtocolUnknownError:
        qDebug()<<"Protocol Unknown";
        break;
    case QNetworkReply::ProtocolInvalidOperationError:
        qDebug()<<"Protocol Invalid Operation";
    case QNetworkReply::ProtocolFailure:
        qDebug()<<"Protocol Failure";
        break;
    case QNetworkReply::InternalServerError:
        qDebug()<<"Internal Server Error";
    case QNetworkReply::OperationNotImplementedError:
        qDebug()<<"Operation Error"; break;
    case QNetworkReply::ServiceUnavailableError:
        qDebug()<<"Service Not available";
        break;
    case QNetworkReply::UnknownServerError:
    default:
        qDebug()<<"Fatal Error";
        break;
    }
}

void shkmQtFtpCleint::networkSSlError(QNetworkReply *reply, const QList<QSslError> &errors)
{
        qDebug()<<"Error No = "<<reply->error();
        qDebug()<<"----Errors------\n"<<errors;
}

/**Common Function for Both Upload/Download Data Track*/
void shkmQtFtpCleint::trxProgessData(qint64 byteSent, qint64 byteTotal)
{
    qint64 dataTRXCompleted  = ((100*byteSent)/byteTotal);
    if(downLoading==1U)
    {
        qDebug()<<"Downloading "<<dataTRXCompleted<<"%"<<endl;
    }
    else
    {
        qDebug()<<"Uploading "<<dataTRXCompleted<<"%"<<endl;
    }
    emit   dataTransRecvSize(dataTRXCompleted);
}

void shkmQtFtpCleint::trxCompleted(QNetworkReply* reply)
{
    if(!reply->error())
    {
        emit dataTransRecvCompleted();
        if(downLoading==1U)
        {
            downLoading=0U;
            QFile *file = new QFile(locationToSaveFile);
            if(file->open(QFile::WriteOnly))
            {
                file->write(reply->readAll());
                file->close();
            }
            file->deleteLater();
            qDebug()<<"Downloading Succesfull";

        }
        else
        {
            qDebug()<<"Sending Succesfull";

        }
        disconnect(manager,&QNetworkAccessManager::finished,this,&shkmQtFtpCleint::trxCompleted);
        manager->deleteLater();
        fileToSend->close();
        fileToSend->deleteLater();
        serverReply->deleteLater();

    }
}

/**
 * @brief shkmQtFtpCleint::sendFile This function uplaod the file on Qt server.
 * @param fileName
 */
void shkmQtFtpCleint::sendFile(QString fileName)
{
    fileToSend               =new QFile(fileName);
    QFileInfo               fileInfo(fileName);
    QString                 completeAddres=QString::null;
    completeAddres.clear();
    completeAddres.append(hostConfgFtpurl);
    completeAddres.append(fileInfo.fileName());
    ftpHostConfig.setUrl(completeAddres);
    QNetworkRequest     networkRequest(ftpHostConfig);
    networkRequest.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    if(fileToSend->open(QFile::ReadOnly))
    {
        serverReply= manager->put(networkRequest,fileToSend);
        connect(serverReply,&QNetworkReply::uploadProgress,this,&shkmQtFtpCleint::trxProgessData);

    }
    else
    {
        qDebug()<<"unable To open File\n";
    }
}



/****Download Section***/
void shkmQtFtpCleint::downloadFile(QString fileName, QString filePathToSaveFile)
{
    QString completeAddres=QString::null;
    downLoading=1U;
    locationToSaveFile=filePathToSaveFile+fileName;
    completeAddres.append(hostConfgFtpurl);
    completeAddres.append(fileName);
    ftpHostConfig.setUrl(completeAddres);
    QNetworkRequest     networkRequest(ftpHostConfig);
    networkRequest.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    serverReply=manager->get(networkRequest);
    connect(serverReply,&QNetworkReply::downloadProgress,this,&shkmQtFtpCleint::trxProgessData);

}
