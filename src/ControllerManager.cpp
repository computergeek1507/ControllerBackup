#include "ControllerManager.h"

#include "FalconV3Controller.h"
#include "FalconV4Controller.h"
#include "FPPController.h"
#include "GeniusController.h"

#include <QtXml>
#include <QFile>

ControllerManager::ControllerManager():
		m_logger(spdlog::get("ControllerBackup"))
{

}

bool ControllerManager::BackUpControllerConfigs(QString const& folder)
{
	for (auto const& c : m_controllers)
	{
		c->BackUpConfig(folder);
	}
	return true;
}

bool ControllerManager::LoadControllers(QString const& outputConfig)
{
	QDomDocument xmlNetworks;
	QFile f(outputConfig + QDir::separator() + "xlights_networks.xml");
	if (!f.open(QIODevice::ReadOnly))
	{
		return false;
	}
	xmlNetworks.setContent(&f);
	f.close();

	QDomElement rootXML = xmlNetworks.documentElement();

	QString const Type = rootXML.tagName();
	
	for (QDomElement controllerXML = rootXML.firstChildElement("Controller"); !controllerXML.isNull(); controllerXML = controllerXML.nextSiblingElement("Controller"))
	{
		bool const active = controllerXML.attribute("ActiveState", "Active") == "Active";
		QString const name = controllerXML.attribute("Name", "");
		QString const type = controllerXML.attribute("Type", "");
		QString const vendor = controllerXML.attribute("Vendor", "");
		QString const model = controllerXML.attribute("Model", "");
		QString const ipAddress = controllerXML.attribute("IP", "");
		if ("Falcon" == vendor && (type == "F16V4" || type == "F48V4"))
		{
			m_controllers.emplace_back(std::make_unique<FalconV4Controller>(name, ipAddress));
		}
		else if ("Falcon" == vendor && (type == "F16V3" || type == "F48"))
		{
			m_controllers.emplace_back(std::make_unique<FalconV3Controller>(name, ipAddress));
		}
		else if ("FPP" == vendor || "ScottNation" == vendor || "KulpLights" == vendor)
		{
			m_controllers.emplace_back(std::make_unique<FPPController>(name, ipAddress));
		}
		else if ("Experience Lights" == vendor)
		{
			m_controllers.emplace_back(std::make_unique<GeniusController>(name, ipAddress));
		}
		else
		{
			m_logger->warn("Unsupported Controller type: {}", vendor.toStdString());
			//unsupported type
		}
	}
	emit ReloadSetFolder(outputConfig);
	emit ReloadControllers();
	return true;
}