#ifndef FPPCONTROLLER_H
#define FPPCONTROLLER_H

#include "BaseController.h"

#include <QString>
#include <memory>

struct FPPController : BaseController
{
	FPPController(QString name, QString ip, QString vender, QString model) : 
		BaseController(std::move(name), std::move(ip)),
		Vender(std::move(vender)), Model(std::move(model)) {}
	QString BackUpConfig(QString const& folder) const override;
	QString GetType() const override{ return "FPP"; };

	QString Vender;
	QString Model;

	static const QString GetBBBStringsURL() { return "/fppjson.php?command=getChannelOutputs&file=co-bbbStrings"; };
	static const QString GetPIStringsURL() { return "/fppjson.php?command=getChannelOutputs&file=co-pixelStrings"; };
};

#endif