#include "xlights_update.h"

#include <QProgressDialog>
#include <QThread>

xLightsUpdate::xLightsUpdate(QObject* parent) :
	QObject(parent), m_logger(spdlog::get("ControllerBackup"))
{
	m_xLightsConn = std::make_unique< xLightsConnection>();
}

bool xLightsUpdate::UpdateXlightsModels(std::vector<ControllerData> controllers) 
{
	if (!m_xLightsConn->Connect())
	{
		return false;
	}

	QProgressDialog progress("Reading Models...", "Abort", 0, m_xLightsConn->GetModels().length(), nullptr);
	progress.setWindowModality(Qt::WindowModal);

	int i = 0;
	for (auto const& model : m_xLightsConn->GetModels())
	{
		progress.setValue(i);
		progress.setLabelText("Looking for " + model +" data");
		if (progress.wasCanceled())
		{
			return false;
		}
		//Use Start Channel
		m_xLightsConn->CallGetModel(model);
		auto settings = m_xLightsConn->GetModelSettings();
		QString con = settings.value("controller");
		if (!con.isEmpty())
		{
			continue;
		}
		QString xsc = settings.value("StartChannel");
		if (!xsc.startsWith("#")) 
		{
			continue;
		}
		progress.setLabelText("Looking for " + model + " " + xsc);
		for (auto const& control : controllers)
		{
			auto xcont = m_xLightsConn->GetController(control.ip);
			if (!xcont)
			{
				continue;
			}
			int port = GetControllerPort(control, xsc);
			if (port == -1)
			{
				continue;
			}
			m_xLightsConn->SetModelControllerPort(model, port);
			QThread::msleep(100);
			//m_xLightsConn->SetModelController(model, xcont->get().name);
			//m_xLightsConn->SetModelController(model, QString::number(xcont->get().index + 2));
			break;
		}
		++i;
	}

	return true;
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