#ifndef FALCONV3CONTROLLER_H
#define FALCONV3CONTROLLER_H

#include "BaseController.h"
#include "BackupVisitor.h"
#include "config_visitor.h"

#include <QString>
#include <memory>

struct FalconV3Controller : BaseController
{
	FalconV3Controller(QString name, QString  ip): BaseController(std::move(name), std::move(ip)) {}
	QString GetType() const override{ return "FalconV3"; };
	[[nodiscard]] QString GetFileExtension() const override;

	void accept(BackupVisitor * v) override;
	void accept(ConfigVisitor * v) override;

	[[nodiscard]] QString DecodeMode(int mode) const;
	[[nodiscard]] QString DecodePixelProtocol(int protocol) const;
	[[nodiscard]] QString DecodeSerialProtocol(int protocol) const;
	[[nodiscard]] QString DecodeColorOrder(int color) const;
	[[nodiscard]] int DecodeBrightness(int brightnessCode) const;
	[[nodiscard]] float DecodeGamma(int gammaCode) const;
};

#endif
