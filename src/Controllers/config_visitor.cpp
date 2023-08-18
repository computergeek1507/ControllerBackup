#include "config_visitor.h"

#include "FalconV3Controller.h"
#include "FalconV4Controller.h"
#include "FPPController.h"
#include "GeniusController.h"
#include "WLEDController.h"


#include <QFile>
#include <QDir>
#include <QElapsedTimer>
#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

void ConfigVisitor::ReadConfig(FalconV3Controller* c)
{
    ReadFile(c->FilePath);
}

void ConfigVisitor::ReadConfig(FalconV4Controller* c)
{
    auto data = ReadFile(c->FilePath);
	QJsonDocument configJson = QJsonDocument::fromJson(data);

	QJsonObject rootObj = configJson.object();

    QJsonValue controller = rootObj.value("c");
    QJsonValue mode = controller.toObject().value("O");
    int iMode = mode.toInt();

    QJsonValue ip = controller.toObject().value("I");
    configData.ip = ip.toString();

    QJsonValue topString = rootObj.value("S");
    QJsonValue strings = controller.toObject().value("A");
    if (strings.type() == QJsonValue::Array)
    {
        QJsonArray stringsArray = strings.toArray();
        for (int i = 0; i < stringsArray.count(); i++) 
        {
            QJsonValue stringsChild = stringsArray.at(i);
            if (stringsChild.type() == QJsonValue::Object) 
            {
                QJsonObject stringsObj = stringsChild.toObject();
                ControllerPort port;
                port.port = stringsObj.value("p").toInt() + 1;
                port.startUniverse = stringsObj.value("u").toInt();
                port.startChannel = stringsObj.value("sc").toInt();
                port.pixels = stringsObj.value("n").toInt();
                port.startNulls = stringsObj.value("ns").toInt();
                port.group = stringsObj.value("gp").toInt();
                port.zigZag = stringsObj.value("z").toInt();
                port.reverse = stringsObj.value("v").toInt();
                configData.pixelports.push_back(port);
            }
        }
    }
}

void ConfigVisitor::ReadConfig(FPPController* c)
{
    ReadFile(c->FilePath);
}

void ConfigVisitor::ReadConfig(GeniusController* c)
{
    ReadFile(c->FilePath);
}

void ConfigVisitor::ReadConfig(WLEDController* c)
{
    ReadFile(c->FilePath);
}

QByteArray ConfigVisitor::ReadFile(QString const& file) const
{
	QFile f(file);

	if (!f.open(QFile::ReadOnly | QFile::Text))
	{
		return QByteArray();
	}

	return f.readAll();
}

void ConfigVisitor::ExportData(QString const& file) const
{

}