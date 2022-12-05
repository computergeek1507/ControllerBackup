#ifndef HINKSPIXCONTROLLER_H
#define HINKSPIXCONTROLLER_H

#include "BaseController.h"

#include <QString>
#include <memory>

struct HinkspixController : BaseController
{
	HinkspixController(QString name, QString  ip): BaseController(std::move(name), std::move(ip)) {}
	QString BackUpConfig(QString const& folder) const override;
	QString GetType() const override{ return "Hinkspix"; };

    static const QString GetJSONInfoURL() { return "/XLights_BoardInfo.cgi"; };
    static const QString GetJSONPortURL() { return "/Xlights_Board_Port_Config.cgi"; };
    static const QString GetJSONModeURL() { return "/Xlights_Data_Mode.cgi"; };
};

#endif