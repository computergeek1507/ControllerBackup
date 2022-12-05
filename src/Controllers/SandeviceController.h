#ifndef SANDEVICECONTROLLER_H
#define SANDEVICECONTROLLER_H

#include "BaseController.h"

#include <QString>
#include <memory>

struct SandeviceController : BaseController
{
	SandeviceController(QString name, QString  ip): BaseController(std::move(name), std::move(ip)) {}
	QString BackUpConfig(QString const& folder) const override;
	QString GetType() const override{ return "Sandevice"; };

    static const QString GetInfoURL() { return"/json/info"; };
    static const QString GetCfgURL() { return"/json/cfg"; };
};

#endif