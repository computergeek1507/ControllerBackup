#ifndef FPPCONTROLLER_H
#define FPPCONTROLLER_H

#include "BaseController.h"
#include "BackupVisitor.h"

#include <QString>
#include <memory>

struct FPPController : BaseController
{
	FPPController(QString name, QString ip, QString vender, QString model) : 
		BaseController(std::move(name), std::move(ip)),
		Vender(std::move(vender)), Model(std::move(model)) {}
	QString GetType() const override{ return "FPP"; };

	QString Vender;
	QString Model;

	static const QString GetInputsURL() { return "/api/configfile/ci-universes.json"; };
	static const QString GetBBBStringsURL() { return "/api/channel/output/co-bbbStrings"; };
	static const QString GetPIStringsURL() { return "/api/channel/output/co-pixelStrings"; };
	static const QString GetOtherOutputURL() { return "/api/channel/output/co-other"; };

	void accept(BackupVisitor * v) override;
	void accept(ConfigVisitor * v) override;

	QString DecodeInputType(int type) const;
};

#endif