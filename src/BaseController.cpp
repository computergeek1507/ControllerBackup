#include "BaseController.h"


#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QFile>
#include <QDir>
#include <QElapsedTimer>
#include <QCoreApplication>

QString BaseController::GetFileName() const
{
	QString ip = IP;
	ip = ip.replace(".", "_");
	return Name + "_" + ip + ".json";
}

QString BaseController::DownloadData(QString const& url) const
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

QString BaseController::DownloadData(QString const& url, QString const& post) const
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

QString BaseController::SaveData(QByteArray const& fileData, QString const& backupfolder) const
{
	QString fileName{ backupfolder + QString(QDir::separator()) + GetFileName() };
	QFile saveFile(fileName);

	if (!saveFile.open(QIODevice::WriteOnly)) 
	{
		qWarning("Couldn't open save file.");
		return QString();
	}
	saveFile.write(fileData);
	return fileName;
}