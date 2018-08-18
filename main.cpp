#include <QCoreApplication>

#include "qtftpClientclass.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    shkmQtFtpCleint shkmQtFtpCleintMain(8080,"speedtest.tele2.net/", "anonymous","anonymous");
    QString  filePath ="../testFolder/shubhFTpTest.txt";
    shkmQtFtpCleintMain.sendFile(filePath);
    QString downloadFilePath ="../testFolder/";
    shkmQtFtpCleintMain.downloadFile("3MB.zip",downloadFilePath);
    return a.exec();
}
