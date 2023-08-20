#ifndef XLIGHTSRGBEFFECTS_H
#define XLIGHTSRGBEFFECTS_H

#include <QString>
#include <QObject>
#include <QDomDocument>

#include "spdlog/spdlog.h"
#include "spdlog/common.h"

#include <memory>
#include <vector>

class xLightsRGBEffects : public QObject
{
	Q_OBJECT
public:
	explicit xLightsRGBEffects(QObject* parent = 0);

	bool LoadXML(QString const& xgbEffects);

	void SetModelControllerPort(QString const& model, int port);
	void SetModelController(QString const& model, QString const& controller);

private:
	std::shared_ptr<spdlog::logger> m_logger{ nullptr };

	QDomDocument rgbeffects_doc;
};

#endif