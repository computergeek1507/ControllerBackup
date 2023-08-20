#ifndef XLIGHTSUPDATE_H
#define XLIGHTSUPDATE_H

#include <QString>
#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>

#include "../Controllers/controller_data.h"

#include "xlightsconnection.h"

#include "spdlog/spdlog.h"
#include "spdlog/common.h"

#include <memory>
#include <vector>

class xLightsUpdate : public QObject
{
	Q_OBJECT
public:
	explicit xLightsUpdate(QObject* parent = 0);

	bool UpdateXlightsModels(std::vector<ControllerData> controllers);

private:
	[[nodiscard]] int GetControllerPort(ControllerData const& control, QString startChannel) const;

	std::shared_ptr<spdlog::logger> m_logger{ nullptr };
	std::unique_ptr<xLightsConnection> m_xLightsConn{ nullptr };
};

#endif