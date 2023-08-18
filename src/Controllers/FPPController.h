#ifndef FPPCONTROLLER_H
#define FPPCONTROLLER_H

#include "BaseController.h"
#include "BackupVisitor.h"
#include "../backup_viewer/viewer_visitor.h"

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

	static const QString GetBBBStringsURL() { return "/fppjson.php?command=getChannelOutputs&file=co-bbbStrings"; };
	static const QString GetPIStringsURL() { return "/fppjson.php?command=getChannelOutputs&file=co-pixelStrings"; };

	void accept(BackupVisitor * v) override;
	void accept(ViewerVisitor * v) override;
	void accept(ConfigVisitor * v) override;
};

#endif