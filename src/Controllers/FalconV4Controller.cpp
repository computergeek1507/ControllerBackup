#include "FalconV4Controller.h"
#include "config_visitor.h"

#include <QJsonDocument>
#include <QJsonObject>

void FalconV4Controller::accept(BackupVisitor * v)
{
	v->BackUp(this);
}

void FalconV4Controller::accept(ConfigVisitor * v)
{
	v->ReadConfig(this);
}

QString FalconV4Controller::BuildParm(QString const& type, QString const& method) const
{
	return QString("{\"T\":\"%1\",\"M\":\"%2\",\"B\":%3,\"E\":%4,\"I\":%5,\"P\":{}}").arg(type).arg(method).arg(0).arg(0).arg(0);
}

QJsonObject FalconV4Controller::GetReturnParm(QString const& json) const
{
	QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
	if (!doc.isNull())
	{
		if (doc.isObject())
		{
			if( doc.object().contains("P")) 
			{
				return doc.object().value("P").toObject();
			}
		}
	}
	
	return QJsonObject();
}

QString FalconV4Controller::DecodeMode(int mode) const
{
	switch (mode) {
	case 0: return "E1.31/ArtNET";
	case 1: return "ZCPP";
	case 2: return "DDP";
	case 3: return "Remote";
	case 4: return "Master";
	case 5: return "Player";
	default: break;
	}
	return "Unknown";
}

QString FalconV4Controller::DecodePixelProtocol(int protocol) const
{
	switch (protocol) {
	case 0: return "apa102";
	case 1: return "apa109";
	case 2: return "dmx512p";
	case 3: return "lpd6803";
	case 4: return "lpd8806";
	case 5: return "sm16716";
	case 6: return "tls3001";
	case 7: return "tm1814";
	case 8: return "tm1829";
	case 9: return "ucs8903";
	case 10: return "ucs8903 16 bit";
	case 11: return "ucs8904";
	case 12: return "ucs8904 16 bit";
	case 13: return "ws2801";
	case 14: return "ws2811";
	case 15: return "ws2811 slow";
	case 16: return "sj1221";
	case 17: return "dmx512p-4";
	}
	return "";
}

QString FalconV4Controller::DecodeColorOrder(int color) const
{
	switch (color) {
	case 0: return "RGB";
	case 1: return "RBG";
	case 2: return "GRB";
	case 3: return "GBR";
	case 4: return "BRG";
	case 5: return "BGR";
	default: break;
	}
	return "RGB";
}

QString FalconV4Controller::DecodeInputType(QString const& type) const
{
	if (type == "e")
	{
		return "E1.31";
	}
	if (type == "a")
	{
		return "ArtNet";
	}
	
	return "Unknown";
}
 