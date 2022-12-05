#ifndef BASECONTROLLER_H
#define BASECONTROLLER_H

#include <QObject>
#include <QString>


#include <memory>

struct BaseController: public QObject
{
	Q_OBJECT
public:
	
	BaseController(QString name, QString  ip): Name(std::move(name)), IP(std::move(ip))
	{}
	[[nodiscard]] virtual QString BackUpConfig(QString const& folder) const = 0;
	[[nodiscard]] virtual QString GetType() const = 0;
	[[nodiscard]] virtual QString GetFileName() const;
	[[nodiscard]] QString DownloadData(QString const& url) const;
	[[nodiscard]] QString DownloadData(QString const& url, QString const& post) const;
	[[nodiscard]] QString SaveData(QByteArray const& json, QString const& backupfolder) const;

	QString Name;
	QString IP;
};

#endif