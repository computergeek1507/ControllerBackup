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
	int zigZag{0};
	int group{1};
	int brightness{100};
	bool reverse{false};
	int gamma{1};
	QString protocol;
	QString toString() const
	{
		return QString("Port:%1,Pixels:%2,Start Channel:%3,Start Universe:%4,Start Nulls:%5,Group Count:%6,Brightness:%7,Reverse:%8,Gamma:%9")
			.arg(port).arg(pixels).arg(startChannel).arg(startUniverse).arg(startNulls).arg(group).arg(brightness).arg(reverse).arg(gamma);
	}
};

struct ControllerData
{
	QString name;
	QString mode;
	QString ip;
	std::vector<ControllerPort> pixelports;
	QString toString() const
	{
		return QString("Name:%1,IP Address:%2,Mode:%3")
			.arg(name).arg(ip).arg(mode);
	}

	void clear()
	{
		pixelports.clear();
		ip.clear();
		name.clear();
		mode.clear();
	}
};

#endif