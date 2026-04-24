#include "ControllerManager.h"

#include "FalconV3Controller.h"
#include "FalconV4Controller.h"
#include "FPPController.h"
#include "GeniusController.h"
#include "WLEDController.h"

#include "BackupVisitor.h"
#include "config_visitor.h"
#include "../xlights/xlights_update.h"

#include "pugixml.hpp"
#include <QDir>
#include <QFile>
#include "backup_file.h"

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
			emit UpdateControllerStatus(c->IP, c->GetBackupFilePaths(), "Saved: ");
		}
		catch (const std::exception&)
		{
			emit UpdateControllerStatus(c->IP, { "" }, "Failed To Connect!");
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
		emit UpdateControllerStatus(c->IP, c->GetBackupFilePaths(), "Saved: ");
		return true;
	}
	catch (const std::exception&)
	{
		emit UpdateControllerStatus(c->IP, { "" }, "Failed To Connect!");
		return false;
	}
}

void ControllerManager::UpdateXLightsController(QString const& folder)
{
	xLightsUpdate test;
	std::unique_ptr<ConfigVisitor> visitor = std::make_unique< ConfigVisitor>();
	for (auto const& c : m_controllers)
	{
		if (!LookForBackup(folder, c.get()))
		{
			continue;
		}
		c->accept(visitor.get());
	}
	test.UpdateXlightsModels(visitor->controllers, m_showdir + QDir::separator() + "xlights_rgbeffects.xml", m_showdir + QDir::separator() + "xlights_networks.xml");
}

bool ControllerManager::LoadControllers(QString const& outputConfig, QString const& backupFolder)
{
	pugi::xml_document xmlNetworks;
	QString const networksFile = outputConfig + QDir::separator() + "xlights_networks.xml";
	if (!QFile::exists(networksFile))
	{
		m_logger->warn("xlights_networks.xml not found in {}", outputConfig.toStdString());
		return false;
	}
	try {
		m_showdir = outputConfig;

		pugi::xml_parse_result result = xmlNetworks.load_file(networksFile.toStdString().c_str());


		if (!result)
		{
			m_logger->warn("Failed to parse xlights_networks.xml: {}", result.description());
			return false;
		}

		pugi::xml_node rootXML = xmlNetworks.document_element();

		QString const Type = QString::fromUtf8(rootXML.name());

		for (pugi::xml_node controllerXML : rootXML.children("Controller"))
		{
			pugi::xml_attribute activeAttr = controllerXML.attribute("ActiveState");
			bool const active = QString::fromUtf8(activeAttr.value()).isEmpty() ? true : QString::fromUtf8(activeAttr.value()) == "Active";
			QString const name = QString::fromUtf8(controllerXML.attribute("Name").value());
			QString const type = QString::fromUtf8(controllerXML.attribute("Type").value());
			QString const vendor = QString::fromUtf8(controllerXML.attribute("Vendor").value());
			QString const model = QString::fromUtf8(controllerXML.attribute("Model").value());
			QString const ipAddress = QString::fromUtf8(controllerXML.attribute("IP").value());
			if ("Falcon" == vendor && (model == "F16V4" || model == "F48V4"
				|| model == "F16V5" || model == "F48V5"))
			{
				m_controllers.emplace_back(std::make_unique<FalconV4Controller>(name, ipAddress));
			}
			else if ("Falcon" == vendor && (model == "F16V3" || model == "F48"))
			{
				m_controllers.emplace_back(std::make_unique<FalconV3Controller>(name, ipAddress));
			}
			else if ("FPP" == vendor || "ScottNation" == vendor || "KulpLights" == vendor
				|| "Wallys Lights" == vendor || "Hanson Electronics" == vendor || "MICROCYB" == vendor)
			{
				m_controllers.emplace_back(std::make_unique<FPPController>(name, ipAddress, vendor, model));
			}
			else if ("Experience Lights" == vendor || "Mattos Designs" == vendor)
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
	catch(const std::exception& e)
	{
		m_logger->warn("Exception caught while loading controllers: {}", e.what());
		return false;
	}
}


void ControllerManager::LookForBackups(QString const& folder)
{
	for (auto const& c : m_controllers)
	{
		LookForBackup(folder, c.get());
	}
}

bool ControllerManager::LookForBackup(QString const& folder, BaseController* c)
{
	bool found{false};
	for (BackupType t : BACKUP_TYPES) {
		if (QFile::exists(folder + QDir::separator() + c->GetFileName(t)))
		{
			c->FilePaths.emplace_back(t, folder + QDir::separator() + c->GetFileName(t));
			found = true;
		}
	}
	return found;
}