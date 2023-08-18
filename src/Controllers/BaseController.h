#ifndef BASECONTROLLER_H
#define BASECONTROLLER_H

#include <QObject>
#include <QString>
#include <memory>

class ViewerVisitor;
class BackupVisitor;
class ConfigVisitor;

struct BaseController: public QObject
{
	Q_OBJECT
public:
	
	BaseController(QString name, QString  ip): Name(std::move(name)), IP(std::move(ip))
	{}
	[[nodiscard]] virtual QString GetType() const = 0;
	[[nodiscard]] virtual QString GetFileName() const;
	virtual void accept(BackupVisitor * v) = 0;
	virtual void accept(ViewerVisitor * v) = 0;
	virtual void accept(ConfigVisitor * v) = 0;

	QString Name;
	QString IP;
	QString FilePath;
};

#endif