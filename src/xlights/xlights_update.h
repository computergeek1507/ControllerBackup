#ifndef XLIGHTSUPDATE_H
#define XLIGHTSUPDATE_H

#include <QString>
#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>

#include "../Controllers/controller_data.h"

#include "xlights_rgbeffects.h"
#include "xlights_networks.h"

#include "spdlog/spdlog.h"
#include "spdlog/common.h"

#include <memory>
#include <vector>
#include <optional>

class xLightsUpdate : public QObject
{
	Q_OBJECT
public:
	explicit xLightsUpdate(QObject* parent = 0);

	bool UpdateXlightsModels(std::vector<ControllerData> controllers, QString const& effects_file, QString const& network_file);

private:
	[[nodiscard]] int GetControllerPort(ControllerData const& control, QString startChannel) const;
	std::optional<std::tuple<QString, int>> GetControllerPortList(std::vector<ControllerData> const& controllers, QString const& xsc) const;
	std::optional<std::tuple<QString, int>> GetBeginningModelStartingUniverse(std::vector<ControllerData> const& controllers, QString xsc) const;

	std::shared_ptr<spdlog::logger> m_logger{ nullptr };
	std::unique_ptr<xLightsRGBEffects> m_xLightsRGB{ nullptr };
	std::unique_ptr<xLightsNetworks> m_xLightsNetwork{ nullptr };
};

#endif