#ifndef FALCONV3CONTROLLER_H
#define FALCONV3CONTROLLER_H

#include "BaseController.h"
#include "BackupVisitor.h"
#include "../backup_viewer/viewer_visitor.h"
#include "config_visitor.h"

#include <QString>
#include <memory>

struct FalconV3Controller : BaseController
{
	FalconV3Controller(QString name, QString  ip): BaseController(std::move(name), std::move(ip)) {}
	QString GetType() const override{ return "FalconV3"; };
	QString GetFileName() const override;

	void accept(BackupVisitor * v) override;
	void accept(ViewerVisitor * v) override;
	void accept(ConfigVisitor * v) override;

	QString DecodeMode(int mode) const;
	QString DecodePixelProtocol(int protocol) const;
	QString DecodeSerialProtocol(int protocol) const;
	QString DecodeColorOrder(int color) const;
	int DecodeBrightness(int brightnessCode) const;
	float DecodeGamma(int gammaCode) const;
};

#endif
