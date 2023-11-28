#ifndef CONTROLLERDATA_H
#define CONTROLLERDATA_H

#include <QString>
#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>

#include <memory>
#include <vector>

struct ControllerPort
{
	int port;
	int pixels{0};
	QString name;
	uint64_t startChannel{0};
	uint64_t startUniverse{0};
	QString colorOrder;
	int startNulls{0};
	int endNulls{ 0 };
	int zigZag{0};
	int group{1};
	int brightness{100};
	bool reverse{false};
	double gamma{1.0};
	QString protocol;
	QString toString() const
	{
		return QString("Port:%1,Pixels:%2,Start Channel:%3,Start Universe:%4,Start Nulls:%5,Group Count:%6,Brightness:%7,Reverse:%8,Gamma:%9")
			.arg(port).arg(pixels).arg(startChannel).arg(startUniverse).arg(startNulls).arg(group).arg(brightness).arg(reverse).arg(gamma);
	}
};

struct ControllerInput
{
	QString type;
	uint64_t channels{ 0 };
	uint64_t startUniverse{ 0 };
	uint64_t universeCount{ 0 };
	uint64_t startChannel{ 1 };
	
	QString toString() const
	{
		return QString("Type:%1, Channel Size:%2, Start Universe:%3, Universe Count:%4, Start Channel:%5")
			.arg(type).arg(channels).arg(startUniverse).arg(universeCount).arg(startChannel);
	}
};

struct ControllerData
{
	QString name;
	QString mode;
	QString ip;
	QString firmware;
	bool absoluteStartAddress{false};
	std::vector<ControllerPort> pixelports;
	std::vector<ControllerPort> serialports;
	std::vector<ControllerInput> inputs;
	QString toString() const
	{
		return QString("Name:%1,IP Address:%2,Mode:%3,Firmware:%4")
			.arg(name).arg(ip).arg(mode).arg(firmware);
	}

	void clear()
	{
		pixelports.clear();
		serialports.clear();
		inputs.clear();
		ip.clear();
		name.clear();
		mode.clear();
		firmware.clear();
		absoluteStartAddress = false;
	}
};

#endif