#ifndef GENIUSCONTROLLER_H
#define GENIUSCONTROLLER_H

#include "BaseController.h"

#include <QString>
#include <memory>

struct GeniusController : BaseController
{
	GeniusController(QString name, QString  ip): BaseController(std::move(name), std::move(ip)) {}
	QString GetType() const override{ return "Genius"; };

	static const QString GetConfigURL() { return "/api/config"; };
	
	void accept(BackupVisitor * v) override;
	void accept(ConfigVisitor * v) override;
};

#endif