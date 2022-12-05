#ifndef ALPHAPIXCONTROLLER_H
#define ALPHAPIXCONTROLLER_H

#include "BaseController.h"

#include <QString>
#include <memory>

struct AlphapixController : BaseController
{
	AlphapixController(QString name, QString  ip): BaseController(std::move(name), std::move(ip)) {}
	QString BackUpConfig(QString const& folder) const override;
	QString GetType() const override{ return "Alphapix"; };

    static const QString GetInfoURL() { return"/json/info"; };
    static const QString GetCfgURL() { return"/json/cfg"; };
};

#endif