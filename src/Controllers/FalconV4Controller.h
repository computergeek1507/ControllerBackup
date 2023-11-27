#ifndef FALCONV4CONTROLLER_H
#define FALCONV4CONTROLLER_H

#include "BaseController.h"
#include "BackupVisitor.h"
#include "../backup_viewer/viewer_visitor.h"
#include "config_visitor.h"

#include <QString>
#include <memory>

struct FalconV4Controller : BaseController
{
	FalconV4Controller(QString name, QString  ip): BaseController(std::move(name), std::move(ip)) {}
	QString GetType() const override{ return "FalconV4"; };
	QString BuildParm(QString const& type, QString const& method) const;
	QJsonObject GetReturnParm(QString const& json) const;

	static const QString GetAPI() { return "/api"; };

	void accept(BackupVisitor * v) override;
	void accept(ViewerVisitor * v) override;
	void accept(ConfigVisitor * v) override;


	QString DecodeMode(int mode) const;
	QString DecodePixelProtocol(int protocol) const;
	QString DecodeColorOrder(int color) const;
};

#endif