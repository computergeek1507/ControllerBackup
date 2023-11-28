#ifndef FALCONV4CONTROLLER_H
#define FALCONV4CONTROLLER_H

#include "BaseController.h"
#include "BackupVisitor.h"
#include "config_visitor.h"

#include <QString>
#include <memory>

struct FalconV4Controller : BaseController
{
	FalconV4Controller(QString name, QString  ip): BaseController(std::move(name), std::move(ip)) {}
	[[nodiscard]] QString GetType() const override{ return "FalconV4"; };
	[[nodiscard]] QString BuildParm(QString const& type, QString const& method) const;
	[[nodiscard]] QJsonObject GetReturnParm(QString const& json) const;

	static const QString GetAPI() { return "/api"; };

	void accept(BackupVisitor * v) override;
	void accept(ConfigVisitor * v) override;

	[[nodiscard]] QString DecodeMode(int mode) const;
	[[nodiscard]] QString DecodePixelProtocol(int protocol) const;
	[[nodiscard]] QString DecodeColorOrder(int color) const;
	[[nodiscard]] QString DecodeInputType(QString const& type) const;
};

#endif