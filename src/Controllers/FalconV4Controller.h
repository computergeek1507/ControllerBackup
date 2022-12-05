#ifndef FALCONV4CONTROLLER_H
#define FALCONV4CONTROLLER_H

#include "BaseController.h"

#include <QJsonObject>
#include <QString>
#include <memory>

struct FalconV4Controller : BaseController
{
	FalconV4Controller(QString name, QString  ip): BaseController(std::move(name), std::move(ip)) {}
	QString BackUpConfig(QString const& folder) const override;
	QString GetType() const override{ return "FalconV4"; };
	QString BuildParm(QString const& type, QString const& method) const;
	QJsonObject GetReturnParm(QString const& json) const;

	static const QString GetAPI() { return "/api"; };
};

#endif