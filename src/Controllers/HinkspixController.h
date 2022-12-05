#ifndef HINKSPIXCONTROLLER_H
#define HINKSPIXCONTROLLER_H

#include "BaseController.h"

#include <QString>
#include <memory>

struct HinkspixController : BaseController
{
	HinkspixController(QString name, QString  ip): BaseController(std::move(name), std::move(ip)) {}
	QString BackUpConfig(QString const& folder) const override;
	QString GetType() const override{ return "Hinkspix"; };

    static const QString GetInfoURL() { return"/json/info"; };
    static const QString GetCfgURL() { return"/json/cfg"; };
};

#endif