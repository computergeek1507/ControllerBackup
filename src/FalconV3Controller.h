#ifndef FALCONV3CONTROLLER_H
#define FALCONV3CONTROLLER_H

#include "BaseController.h"

#include <QString>
#include <memory>

struct FalconV3Controller : BaseController
{
	FalconV3Controller(QString name, QString  ip): BaseController(std::move(name), std::move(ip)) {}
	void BackUpConfig(QString const& folder) const override;
	QString GetType() const override{ return "FalconV3"; };

};

#endif
