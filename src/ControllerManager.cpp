#include "ControllerManager.h"

#include "ArtNetOutput.h"
#include "DDPOutput.h"
#include "E131Output.h"

#include <QtXml>
#include <QFile>

ControllerManager::ControllerManager():
		m_logger(spdlog::get("ControllerBackup"))
{

}

bool ControllerManager::OpenOutputs()
{
	for (auto const& o : m_outputs)
	{
		o->Open();
	}
	return true;
}

void ControllerManager::CloseOutputs()
{
	for (auto const& o : m_outputs)
	{
		o->Close();
	}
}

void ControllerManager::OutputData(uint8_t* data)
{
	//TODO: multithread
	for (auto const& o : m_outputs)
	{
		o->OutputFrame(data);
	}
}

bool ControllerManager::LoadOutputs(QString const& outputConfig)
{
	QDomDocument xmlNetworks;
	QFile f(outputConfig);
	if (!f.open(QIODevice::ReadOnly))
	{
		return false;
	}
	xmlNetworks.setContent(&f);
	f.close();

	QDomElement rootXML = xmlNetworks.documentElement();

	uint64_t startChannel{ 1 };
	QString const Type = rootXML.tagName();
	QString const proxy = rootXML.attribute("GlobalFPPProxy", "");
	
	for (QDomElement controllerXML = rootXML.firstChildElement("Controller"); !controllerXML.isNull(); controllerXML = controllerXML.nextSiblingElement("Controller"))
	{
		bool const active = controllerXML.attribute("ActiveState", "Active") == "Active";
		for (QDomElement networkXML = controllerXML.firstChildElement("network"); !networkXML.isNull(); networkXML = networkXML.nextSiblingElement("network"))
		{
			QString const nType = networkXML.attribute("NetworkType", "");
			QString const sChannels = networkXML.attribute("MaxChannels", "0");
			QString const ipAddress = networkXML.attribute("ComPort", "");
			QString const universe = networkXML.attribute("BaudRate", "");
			uint64_t iChannels =  sChannels.toULong();
			if ("DDP" == nType)
			{
				QString const sKeepChannels = networkXML.attribute("KeepChannelNumbers", "1");
				QString const sPacketSize = networkXML.attribute("ChannelsPerPacket", "1440");

				auto ddp = std::make_unique<DDPOutput>();
				ddp->IP = ipAddress;
				ddp->PacketSize = sPacketSize.toUInt();
				ddp->KeepChannels = sPacketSize.toInt();
				ddp->StartChannel = startChannel;
				ddp->Channels = iChannels;
				ddp->Enabled = active;
				m_outputs.push_back(std::move(ddp));
				emit AddController(active, nType, ipAddress, sChannels);
			}
			else if ("E131" == nType)
			{
				QString const sPacketSize = networkXML.attribute("MaxChannels", "510");
				auto e131 = std::make_unique<E131Output>();
				e131->IP = ipAddress;
				e131->PacketSize = sPacketSize.toUInt();
				e131->Universe = universe.toUInt();
				e131->StartChannel = startChannel;
				e131->Channels = iChannels;//todo fix
				e131->Enabled = active;
				m_outputs.push_back(std::move(e131));
				emit AddController(active, nType, ipAddress, sChannels);
			}
			else if ("ArtNet" == nType)
			{
				QString const sPacketSize = networkXML.attribute("MaxChannels", "510");
				auto artnet = std::make_unique<ArtNetOutput>();
				artnet->IP = ipAddress;
				artnet->PacketSize = sPacketSize.toUInt();
				artnet->Universe = universe.toUInt();
				artnet->StartChannel = startChannel;
				artnet->Channels = iChannels;//todo fix
				artnet->Enabled = active;
				m_outputs.push_back(std::move(artnet));
				emit AddController(active, nType, ipAddress, sChannels);
			}
			else
			{
				m_logger->warn("Unsupported output type: {}", nType.toStdString());
				//unsupported type
			}
			startChannel += iChannels;
		}
	}
	emit SetChannelCount(startChannel - 1);
	return true;
}