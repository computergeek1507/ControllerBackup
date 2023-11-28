#ifndef BASECONTROLLER_H
#define BASECONTROLLER_H

#include "backup_file.h"

#include <QObject>
#include <QString>
#include <memory>

class BackupVisitor;
class ConfigVisitor;

struct BaseController: public QObject
{
	Q_OBJECT
public:
	
	BaseController(QString name, QString  ip): Name(std::move(name)), IP(std::move(ip))
	{}
	[[nodiscard]] virtual QString GetType() const = 0;
	[[nodiscard]] virtual QString GetFileName(BackupType type = BackupType::All) const;
	[[nodiscard]] virtual QString GetFileExtension() const;
	[[nodiscard]] QString GetBackupFile(BackupType type = BackupType::All) const;
	[[nodiscard]] QStringList GetBackupFilePaths() const;
	[[nodiscard]] QStringList GetBackupFileNames() const;
	virtual void accept(BackupVisitor * v) = 0;
	virtual void accept(ConfigVisitor * v) = 0;

	QString Name;
	QString IP;
	std::vector<BackupFile> FilePaths;
};

#endif