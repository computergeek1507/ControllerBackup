#ifndef FALCONV4CONTROLLER_H
#define FALCONV4CONTROLLER_H

#include "BaseController.h"

#include <QString>
#include <memory>

struct FalconV4Controller : BaseController
{
	FalconV4Controller(QString name, QString  ip): BaseController(std::move(name), std::move(ip)) {}
	void BackUpConfig(QString const& folder) const override;
	QString GetType() const override{ return "FalconV4"; };

};

#endif