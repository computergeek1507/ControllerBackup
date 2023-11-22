#include "ControllerManager.h"

#include "FalconV3Controller.h"
#include "FalconV4Controller.h"
#include "FPPController.h"
#include "GeniusController.h"
#include "WLEDController.h"

#include "BackupVisitor.h"
#include "config_visitor.h"
#include "../xlights/xlights_update.h"

#include <QtXml>
#include <QFile>

ControllerManager::ControllerManager():
		m_logger(spdlog::get("ControllerBackup"))
{
}

bool ControllerManager::BackUpControllerConfigs(QString const& folder)
{
	std::unique_ptr<BackupVisitor> visitor = std::make_unique< BackupVisitor>(folder);
	bool worked{true};
	for (auto const& c : m_controllers)
	{
		try
		{
			c->accept(visitor.get());
			emit UpdateControllerStatus(c->IP, visitor->BackUpPath, "Saved: ");
		}
		catch (const std::exception&)
		{
			emit UpdateControllerStatus(c->IP, "", "Failed To Connect!");
			worked = false;
		}
	}
	return worked;
}

bool ControllerManager::BackUpControllerConfig(QString const& folder, int index)
{
	std::unique_ptr<BackupVisitor> visitor = std::make_unique< BackupVisitor>(folder);	
	auto c{ GetController(index) };

	try
	{
		c->accept(visitor.get());
		emit UpdateControllerStatus(c->IP, visitor->BackUpPath, "Saved: ");
		return true;
	}
	catch (const std::exception&)
	{
		emit UpdateControllerStatus(c->IP, "", "Failed To Connect!");
		return false;
	}
}

void ControllerManager::UpdateXLightsController(QString const& folder)
{
	xLightsUpdate test;
	//std::vector<ControllerData> controllers;
	std::unique_ptr<ConfigVisitor> visitor = std::make_unique< ConfigVisitor>();
	for (auto const& c : m_controllers)
	{
		if (QFile::exists(folder + QDir::separator() + c->GetFileName()))
		{
			c->FilePath = folder + QDir::separator() + c->GetFileName();
		}
		else
		{
			continue;
		}
		c->accept(visitor.get());
	}
	test.UpdateXlightsModels(visitor->controllers, m_showdir + QDir::separator() + "xlights_rgbeffects.xml", m_showdir + QDir::separator() + "xlights_networks.xml");
}

bool ControllerManager::LoadControllers(QString const& outputConfig, QString const& backupFolder)
{
	QDomDocument xmlNetworks;
	QFile f(outputConfig + QDir::separator() + "xlights_networks.xml");
	if (!f.open(QIODevice::ReadOnly))
	{
		return false;
	}
	m_showdir = outputConfig;
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
		if ("Falcon" == vendor && (model == "F16V4" || model == "F48V4"))
		{
			m_controllers.emplace_back(std::make_unique<FalconV4Controller>(name, ipAddress));
		}
		else if ("Falcon" == vendor && (model == "F16V3" || model == "F48"))
		{
			m_controllers.emplace_back(std::make_unique<FalconV3Controller>(name, ipAddress));
		}
		else if ("FPP" == vendor || "ScottNation" == vendor || "KulpLights" == vendor)
		{
			m_controllers.emplace_back(std::make_unique<FPPController>(name, ipAddress, vendor, model));
		}
		else if ("Experience Lights" == vendor)
		{
			m_controllers.emplace_back(std::make_unique<GeniusController>(name, ipAddress));
		}
		else if ("WLED" == vendor)
		{
			m_controllers.emplace_back(std::make_unique<WLEDController>(name, ipAddress));
		}
		else
		{
			m_logger->warn("Unsupported Controller type: {}", vendor.toStdString());
			//unsupported type
			m_controllers.emplace_back(std::make_unique<FalconV4Controller>(name, ipAddress));
		}
	}
	LookForBackups(backupFolder);
	emit ReloadSetFolder(outputConfig);
	emit ReloadControllers();
	return true;
}


void ControllerManager::LookForBackups(QString const& folder)
{
	for (auto const& c : m_controllers)
	{
		if (QFile::exists(folder + QDir::separator() + c->GetFileName()))
		{
			c->FilePath = folder + QDir::separator() + c->GetFileName();
		}
	}
}