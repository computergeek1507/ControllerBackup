#include "xlights_update.h"

#include <QProgressDialog>
#include <QThread>
#include <QCoreApplication>
#include <QDebug>

xLightsUpdate::xLightsUpdate(QObject* parent) :
	QObject(parent), m_logger(spdlog::get("ControllerBackup"))
{
	m_xLightsRGB = std::make_unique<xLightsRGBEffects>();
	m_xLightsNetwork = std::make_unique<xLightsNetworks>();
}

bool xLightsUpdate::UpdateXlightsModels(std::vector<ControllerData> controllers, QString const& effects_file, QString const& network_file)
{
	if (!m_xLightsRGB->LoadXML(effects_file))
	{
		m_logger->error("Unable to Load: {}", network_file.toStdString());
		return false;
	}
	if (!m_xLightsNetwork->LoadXML(network_file))
	{
		m_logger->error("Unable to Load: {}", network_file.toStdString());
		return false;
	}

	QProgressDialog progress("Reading Models...", "Abort", 0, m_xLightsRGB->GetModels().length(), nullptr);
	progress.setWindowModality(Qt::WindowModal);

	auto ipmap = m_xLightsNetwork->GetControllerIPMap();

	for (auto& control : controllers)
	{
		if (ipmap.contains(control.ip))
		{
			control.name = ipmap.value(control.ip);
		}
	}

	int i = -1;
	for (auto const& model : m_xLightsRGB->GetModels())
	{
		++i;
		try
		{
			progress.setValue(i);
			progress.setLabelText("Looking for " + model +" data");
			//qDebug() << "Looking for " + model;
			if (progress.wasCanceled())
			{
				return false;
			}
			//Use Start Channel

			auto settings = m_xLightsRGB->GetModelSettings(model);
			QString con = settings.value("Controller");
			//if (!con.isEmpty())
			//{
			//	continue;
			//}
			QString xsc = settings.value("StartChannel");

			progress.setLabelText("Looking for " + model + " " + xsc);
			//m_logger->warn("Looking for: {} {}", model.toStdString(), xsc.toStdString());
			//qDebug() << "Looking for " + model + " " + xsc;
			QCoreApplication::processEvents();

			if (xsc.startsWith(">"))
			{
				auto port = GetBeginningModelStartingUniverse(controllers, xsc);
				if (port) 
				{
					xsc = xsc.mid(1, xsc.lastIndexOf(":")-1);
					m_xLightsRGB->SetModelControllerChain(model, xsc);
					m_xLightsRGB->SetModelControllerPort(model, std::get<1>(port.value()));
					m_xLightsRGB->SetModelController(model, std::get<0>(port.value()));
				}
				else
				{
					qDebug() << "Couldn't find: " << model << " " << xsc;
					m_logger->error("Couldn't find: {} {}", model.toStdString(), xsc.toStdString());
				}
				continue;
			}
			if (!xsc.startsWith("#")) 
			{
				continue;
			}
		
			auto port = GetControllerPortList(controllers, xsc);
			if (port)
			{
				m_xLightsRGB->SetModelControllerPort(model, std::get<1>(port.value()));
				m_xLightsRGB->SetModelController(model, std::get<0>(port.value()));
			}
			else
			{
				qDebug() << "Couldn't find: " << model << " " << xsc;
				m_logger->error("Couldn't find: {} {}", model.toStdString(), xsc.toStdString());
			}
		}
		catch (const std::exception& ex)
		{
			qDebug() << "Error: " << ex.what();
			m_logger->error(ex.what());
		}
	}
	QString newEffects = effects_file;
	newEffects += "2";
	m_xLightsRGB->SaveXML(newEffects);

	return true;
}

std::optional<std::tuple<QString,int>> xLightsUpdate::GetControllerPortList(std::vector<ControllerData> const& controllers, QString const& xsc) const
{
	for (auto const& control : controllers)
	{
		int port = GetControllerPort(control, xsc);
		if (port == -1)
		{
			continue;
		}
		return { { control.name ,port } };
	}
	return std::nullopt;
}

std::optional<std::tuple<QString, int>> xLightsUpdate::GetBeginningModelStartingUniverse(std::vector<ControllerData> const& controllers, QString xsc) const
{
	xsc = xsc.mid(1, xsc.lastIndexOf(":")-1);
	
	auto settings = m_xLightsRGB->GetModelSettings(xsc);
	QString new_xsc = settings.value("StartChannel");

	if (new_xsc.startsWith(">"))
	{
		return GetBeginningModelStartingUniverse(controllers, new_xsc);
	}
	if (new_xsc.startsWith("!"))
	{
		QString new_control = settings.value("Controller");
		int port = m_xLightsRGB->GetModelControllerPort(xsc);
		if (port != -1)
		{
			return { { new_control ,port } };
		}
	}
	if (new_xsc.startsWith("#"))
	{
		return GetControllerPortList(controllers, new_xsc);
	}
	return std::nullopt;
}

int xLightsUpdate::GetControllerPort(ControllerData const& control, QString startChannel) const
{
	//#25101:13"
	if (!startChannel.startsWith("#") && !startChannel.contains(":"))
	{
		return -1;
	}
	startChannel = startChannel.remove(0, 1);
	auto channelArray = startChannel.split(":");
	for (auto const& port : control.pixelports)
	{
		ulong unv = channelArray[0].toULong();
		ulong sc = channelArray[1].toULong();
		if (port.startChannel == sc && port.startUniverse == unv)
		{
			return port.port;
		}
	}

	return -1;
}