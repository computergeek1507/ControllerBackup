#ifndef FPPCONTROLLER_H
#define FPPCONTROLLER_H

#include "BaseController.h"

#include <QString>
#include <memory>

struct FPPController : BaseController
{
	FPPController(QString name, QString ip) : BaseController(std::move(name), std::move(ip)) {}
	void BackUpConfig(QString const& folder) const override;
	QString GetType() const override{ return "FPP"; };

	static const QString GetBBBStringsURL() { return "/fppjson.php?command=getChannelOutputs&file=co-bbbStrings"; };
};

#endif