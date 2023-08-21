#ifndef XLIGHTSNETWORKS_H
#define XLIGHTSNETWORKS_H

#include <QString>
#include <QObject>
#include <QDomDocument>

#include "spdlog/spdlog.h"
#include "spdlog/common.h"

#include <memory>
#include <vector>

class xLightsNetworks : public QObject
{
	Q_OBJECT
public:
	explicit xLightsNetworks(QObject* parent = 0);

	bool LoadXML(QString const& xgbNetworks);
	bool SaveXML(QString const& xgbNetworks);

	bool SetController(QString const& ip, QString const& vendor, QString const& model, QString const& variant);

	QStringList GetControllerIPs() const;
	QStringList GetControllerNames() const;
	QMap<QString, QString> GetControllerIPMap() const;

private:
	std::shared_ptr<spdlog::logger> m_logger{ nullptr };

	QDomDocument xgbnetworks_doc;
	QStringList xgbnetworks_data;
};

#endif