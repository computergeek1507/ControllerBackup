#ifndef XLIGHTSRGBEFFECTS_H
#define XLIGHTSRGBEFFECTS_H

#include <QStringList>
#include <QObject>
#include <QDomDocument>
#include <QMap>

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
	bool SaveXML(QString const& xgbEffects);

	bool SetModelControllerPort(QString const& model, int port);
	bool SetModelController(QString const& model, QString const& controller);
	bool SetModelControllerChain(QString const& model, QString const& chainModel);
	int GetModelControllerPort(QString const& model) const;
	QStringList GetModels() const;
	QMap<QString, QString> GetModelSettings(QString const& model) const;

private:
	std::shared_ptr<spdlog::logger> m_logger{ nullptr };

	QDomDocument rgbeffects_doc;
	QStringList rgbeffects_data;
};

#endif