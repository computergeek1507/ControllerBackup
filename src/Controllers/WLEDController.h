#ifndef WLEDCONTROLLER_H
#define WLEDCONTROLLER_H

#include "BaseController.h"

#include <QString>
#include <memory>

struct WLEDController : BaseController
{
	WLEDController(QString name, QString  ip): BaseController(std::move(name), std::move(ip)) {}
	QString GetType() const override{ return "WLED"; };

    static const QString GetInfoURL() { return"/json/info"; };
    static const QString GetCfgURL() { return"/json/cfg"; };

	void accept(BackupVisitor * v) override;
	void accept(ConfigVisitor * v) override;

	QString DecodePixelProtocol(int protocol) const;
	QString DecodeColorOrder(int color) const;
};

#endif