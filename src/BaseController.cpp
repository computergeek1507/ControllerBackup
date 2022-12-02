#include "BaseController.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QFile>
#include <QDir>

QString BaseController::GetFileName() const
{
	QString ip = IP;
	ip = ip.replace(".", "_");
	return Name + "_" + ip + ".json";
}

QJsonObject BaseController::DownloadJson(QString const& url) const
{
    QNetworkAccessManager m_manager;

    QNetworkRequest request = QNetworkRequest(QUrl(url));
    QNetworkReply* reply = m_manager.get(request);

    QObject::connect(reply, &QNetworkReply::finished, [reply]() {

        QString ReplyText = reply->readAll();
        // qDebug() << ReplyText;
        // ask doc to parse it
        QJsonDocument doc = QJsonDocument::fromJson(ReplyText.toUtf8());
        // we know first element in file is object, to try to ask for such
        return doc.object();
        reply->deleteLater(); // make sure to clean up
    });

    return QJsonObject();
}

void BaseController::SaveJson(QJsonObject const& json, QString const& backupfolder) const
{
    QFile saveFile(backupfolder + QString(QDir::separator()) + GetFileName());

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return;
    }
    saveFile.write(QJsonDocument(json).toJson());
}