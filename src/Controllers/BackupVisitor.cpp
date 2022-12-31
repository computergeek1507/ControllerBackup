#include "BackupVisitor.h"

#include "FalconV3Controller.h"
#include "FalconV4Controller.h"
#include "FPPController.h"
#include "GeniusController.h"
#include "WLEDController.h"

#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QFile>
#include <QDir>
#include <QElapsedTimer>
#include <QCoreApplication>

void BackupVisitor::BackUp(FalconV3Controller* c)
{
    auto json = DownloadData("http://" + c->IP + "/settings.xml");
	SaveData(json.toLatin1(), Folder, c->GetFileName());
	c->FilePath = BackUpPath;
}

void BackupVisitor::BackUp(FalconV4Controller* c)
{
    QJsonObject controllerObject;
    //"Q", "SP", batch, 0, 0,

    QString url = "http://" + c->IP + c->GetAPI();

    controllerObject.insert("C", c->GetReturnParm( DownloadData(url, c->BuildParm("Q", "ST"))));
    controllerObject.insert("I", c->GetReturnParm( DownloadData(url, c->BuildParm("Q", "IN"))));
    controllerObject.insert("S", c->GetReturnParm( DownloadData(url, c->BuildParm("Q", "SP"))));

    controllerObject.insert("SC", c->GetReturnParm(DownloadData(url, c->BuildParm("Q", "SC"))));
    controllerObject.insert("P", c->GetReturnParm(DownloadData(url, c->BuildParm("Q", "IP"))));

    //{"T":"Q","M":"IN","B":0,"E":0,"I":0,"P":{}}
    QJsonDocument doc(controllerObject);
    //qDebug() << doc.toJson();
    SaveData(doc.toJson(), Folder, c->GetFileName());
	c->FilePath = BackUpPath;
}

void BackupVisitor::BackUp(FPPController* c)
{
    auto stringurl = c->GetBBBStringsURL();
	if ("Pi Hat" == c->Model)
	{
		stringurl = c->GetPIStringsURL();
	}
	///fppjson.php?command=getChannelOutputs&file=co-bbbStrings
	auto json = DownloadData("http://" + c->IP + stringurl);
	SaveData(json.toLatin1(), Folder, c->GetFileName());
	c->FilePath = BackUpPath;
}

void BackupVisitor::BackUp(GeniusController* c)
{
    auto json = DownloadData("http://" + c->IP + c->GetConfigURL());
	SaveData(json.toLatin1(), Folder, c->GetFileName());
	c->FilePath = BackUpPath;
}

void BackupVisitor::BackUp(WLEDController* c)
{
    auto json = DownloadData("http://" + c->IP + c->GetCfgURL());
	SaveData(json.toLatin1(), Folder, c->GetFileName());
	c->FilePath = BackUpPath;
}

QString BackupVisitor::DownloadData(QString const& url) const
{
	QNetworkAccessManager manager;
	QNetworkRequest request(url);
	request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);
	QNetworkReply* response = manager.get(request);

	QElapsedTimer timer;
	timer.start();

	while (!response->isFinished())
	{
		if (timer.elapsed() >= (5 * 1000))
		{
			response->abort();
			return QString();
		}
		QCoreApplication::processEvents();
	}

	auto content = response->readAll();
	response->deleteLater();
	return content;
}

QString BackupVisitor::DownloadData(QString const& url, QString const& post) const
{
	//"application/json"
	QNetworkAccessManager manager;
	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);
	QNetworkReply* response = manager.post(request, post.toLatin1());

	QElapsedTimer timer;
	timer.start();

	while (!response->isFinished())
	{
		if (timer.elapsed() >= (5 * 1000))
		{
			response->abort();
			return QString();
		}
		QCoreApplication::processEvents();
	}

	auto content = response->readAll();
	response->deleteLater();
	return content;
}

void BackupVisitor::SaveData(QByteArray const& fileData, QString const& backupfolder, QString const& fileName)
{
	if (fileData.isEmpty()) 
	{
		qWarning("data is empty.");
		return;
	}
	QString filePath{ backupfolder + QString(QDir::separator()) + fileName };
	QFile saveFile(filePath);

	if (!saveFile.open(QIODevice::WriteOnly)) 
	{
		qWarning("Couldn't open save file.");
		return;
	}
	saveFile.write(fileData);
	BackUpPath = filePath;
}