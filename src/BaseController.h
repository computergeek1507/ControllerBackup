#ifndef BASECONTROLLER_H
#define BASECONTROLLER_H

#include <QObject>
#include <QString>
#include <QJsonObject>

#include <memory>

struct BaseController: public QObject
{
	Q_OBJECT
public:
	
	BaseController(QString name, QString  ip): Name(std::move(name)), IP(std::move(ip))
	{}
	virtual void BackUpConfig(QString const& folder) const = 0;
	virtual QString GetType() const = 0;
	virtual QString GetFileName() const;
	QJsonObject DownloadJson(QString const& url) const;
	void SaveJson(QJsonObject const& json, QString const& backupfolder) const;

	QString Name;
	QString IP;
};

#endif