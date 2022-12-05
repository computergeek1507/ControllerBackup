#ifndef WLEDCONTROLLER_H
#define WLEDCONTROLLER_H

#include "BaseController.h"

#include <QString>
#include <memory>

struct WLEDController : BaseController
{
	WLEDController(QString name, QString  ip): BaseController(std::move(name), std::move(ip)) {}
	QString BackUpConfig(QString const& folder) const override;
	QString GetType() const override{ return "WLED"; };

    static const QString GetInfoURL() { return"/json/info"; };
    static const QString GetCfgURL() { return"/json/cfg"; };
};

#endif