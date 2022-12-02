#ifndef GENIUSCONTROLLER_H
#define GENIUSCONTROLLER_H

#include "BaseController.h"

#include <QString>
#include <memory>

struct GeniusController : BaseController
{
	GeniusController(QString name, QString  ip): BaseController(std::move(name), std::move(ip)) {}
	void BackUpConfig(QString const& folder) const override;
	QString GetType() const override{ return "Genius"; };

	static const QString GetConfigURL() { return "/api/config"; };
};

#endif