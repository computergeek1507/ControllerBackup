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
#include <QDomDocument>

#include <cmath>

void ConfigVisitor::ReadConfig(FalconV3Controller* c)
{
    ControllerData configData;
    auto data = ReadFile(c->FilePath);

    //load the file
    QDomDocument doc;
    doc.setContent(data);
    QDomElement docElem = doc.documentElement();

    QDomElement universeElement = docElem.firstChildElement("universes");
    if (!universeElement.isNull()) {
        QDomNode node = universeElement.firstChild();
        while (!node.isNull()) {
            QDomElement element = node.toElement();
            if (!element.isNull()) {
                //const QString tagName(element.tagName());
                QString universe = element.attribute("u");
                QString lenth = element.attribute("l");
                QString sc = element.attribute("s");
                ControllerInput input;
                input.channels = lenth.toInt();
                input.startUniverse = universe.toInt();
                input.universeCount = 1;
                input.startChannel = sc.toInt();
                //input.type = stringsObj.value("p").toString();
                configData.inputs.push_back(input);
            }
            node = node.nextSibling();
        }
    }

    QDomElement stringsElement = docElem.firstChildElement("strings");
    if (!stringsElement.isNull()) {
        QDomNode node = stringsElement.firstChild();
        while (!node.isNull()) {
            QDomElement element = node.toElement();
            if (!element.isNull()) {
                //const QString tagName(element.tagName());
                QString name = element.attribute("y");
                QString port = element.attribute("p");
                QString uni = element.attribute("u");
                QString pixels = element.attribute("c");
                QString us = element.attribute("us");
                QString start = element.attribute("s");
                 ControllerPort pixelport;
                pixelport.port = port.toInt() + 1;
                pixelport.pixels = pixels.toInt();
                pixelport.name = name;
                pixelport.startNulls = element.attribute("n").toInt();
                pixelport.reverse = (element.attribute("d").toInt());
                pixelport.gamma = c->DecodeGamma(element.attribute("ga").toInt());
                pixelport.brightness = c->DecodeBrightness(element.attribute("b").toInt());
                pixelport.colorOrder = c->DecodeColorOrder(element.attribute("o").toInt());
                pixelport.protocol = c->DecodePixelProtocol(element.attribute("t").toInt());
                //pixelport.universeCount = 1;
                if (us == "0") {
                    pixelport.startChannel = start.toInt();
                } else {
                    pixelport.startUniverse = uni.toInt();
                    pixelport.startChannel = us.toInt();
                }
                //input.type = stringsObj.value("p").toString();
                configData.pixelports.push_back(pixelport);
            }
            node = node.nextSibling();
        }
    }
    
    //QDomElement nameElement = headerElement.firstChildElement("un");
    
    //QString universe = nameElement.attribute("u");
    configData.ip = c->IP;
    //ReadIPFromFile(c->FilePath);
    controllers.push_back(configData);
}

void ConfigVisitor::ReadConfig(FalconV4Controller* c)
{
    ControllerData configData;
    auto data = ReadFile(c->FilePath);
	QJsonDocument configJson = QJsonDocument::fromJson(data);

	QJsonObject rootObj = configJson.object();

    QJsonValue controller = rootObj.value("C");
    QJsonValue mode = controller.toObject().value("O");
    int iMode = mode.toInt();

    configData.mode = c->DecodeMode(iMode);

    QJsonValue name = controller.toObject().value("N");
    configData.name = name.toString();

    //QJsonValue ip = controller.toObject().value("I");
    configData.ip = c->IP;

    QJsonValue version = controller.toObject().value("V");
    configData.firmware = version.toString();

    QJsonValue topString = rootObj.value("S");
    QJsonValue strings = topString.toObject().value("A");
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
                port.startChannel = stringsObj.value("sc").toInt()+1;
                port.pixels = stringsObj.value("n").toInt();
                port.startNulls = stringsObj.value("ns").toInt();
                port.endNulls = stringsObj.value("ne").toInt();
                port.group = stringsObj.value("gp").toInt();
                port.zigZag = stringsObj.value("z").toInt();
                port.reverse = stringsObj.value("v").toInt();
                port.name = stringsObj.value("nm").toString();
                port.gamma = stringsObj.value("g").toInt()/10.0;
                port.colorOrder = c->DecodeColorOrder(stringsObj.value("o").toInt());
                port.protocol = c->DecodePixelProtocol(stringsObj.value("l").toInt());
                configData.pixelports.push_back(port);
            }
        }
    }

    QJsonValue topInputs = rootObj.value("I");
    QJsonValue inputs = topInputs.toObject().value("A");
    if (inputs.type() == QJsonValue::Array)
    {
        QJsonArray inputsArray = inputs.toArray();
        for (int i = 0; i < inputsArray.count(); i++)
        {
            QJsonValue inputChild = inputsArray.at(i);
            if (inputChild.type() == QJsonValue::Object)
            {
                QJsonObject stringsObj = inputChild.toObject();
                ControllerInput input;
                input.channels = stringsObj.value("c").toInt();
                input.startUniverse = stringsObj.value("u").toInt();
                input.universeCount = stringsObj.value("uc").toInt();
                input.type = stringsObj.value("p").toString();
                configData.inputs.push_back(input);
            }
        }
    }
    if (!configData.ip.isEmpty()) 
    {
        controllers.push_back(configData);
    }
    else 
    {
        qDebug() << c->FilePath << "was invalid";
    }
}

void ConfigVisitor::ReadConfig(FPPController* c)
{
    ControllerData configData;
    auto data = ReadFile(c->FilePath);
    QJsonDocument configJson = QJsonDocument::fromJson(data);

    QJsonObject rootObj = configJson.object();
}

void ConfigVisitor::ReadConfig(GeniusController* c)
{
    ControllerData configData;
    auto data = ReadFile(c->FilePath);
    QJsonDocument configJson = QJsonDocument::fromJson(data);

    QJsonObject rootObj = configJson.object();

    QJsonValue system = rootObj.value("system");
    QJsonValue name = system.toObject().value("friendly_name");
    configData.name = name.toString();
    QJsonValue mode = system.toObject().value("operating_mode");
    configData.mode = mode.toString();


    QJsonValue strings = rootObj.value("outputs");
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
                port.port = i + 1;
                port.brightness = 30;
                //port.startUniverse = stringsObj.value("u").toInt();
                //port.startChannel = stringsObj.value("sc").toInt() + 1;
                //port.pixels = stringsObj.value("n").toInt();
                //port.startNulls = stringsObj.value("ns").toInt();
                //port.endNulls = stringsObj.value("ne").toInt();
                //port.group = stringsObj.value("gp").toInt();
                //port.zigZag = stringsObj.value("z").toInt();
                //port.reverse = stringsObj.value("v").toInt();
                //port.name = stringsObj.value("nm").toString();
                //port.gamma = stringsObj.value("g").toInt() / 10.0;
                QJsonValue vitualString = stringsObj.value("virtual_strings");
                if (vitualString.type() == QJsonValue::Array)
                {
                    QJsonArray vsArray = vitualString.toArray();
                    
                    for (int i = 0; i < vsArray.count(); i++)
                    {
                        QJsonValue vsChild = vsArray.at(i);
                        QJsonObject vsObj = vsChild.toObject();
                        //auto y = vsChild.toString();
                        if (i == 0)
                        {
                            port.startChannel = vsObj.value("sc").toInt();

                            if (vsObj.contains("n"))
                            {
                                port.name = vsObj.value("n").toString();
                            }
                            if (vsObj.contains("su"))
                            {
                                port.startUniverse = vsObj.value("su").toInt();
                            }
                            if (vsObj.contains("sn"))
                            {
                                port.startNulls = vsObj.value("sn").toInt();
                            }
                            if (vsObj.contains("en"))
                            {
                                port.endNulls = vsObj.value("en").toInt();
                            }
                            if (vsObj.contains("r"))
                            {
                                port.reverse = vsObj.value("r").toBool();
                            }
                            if (vsObj.contains("b"))
                            {
                                port.brightness = vsObj.value("b").toInt();
                            }
                            if (vsObj.contains("g"))
                            {
                                port.gamma = vsObj.value("g").toDouble();
                            }
                            if (vsObj.contains("st"))
                            {
                                port.colorOrder = vsObj.value("st").toString();
                            }
                        }

                        port.pixels += vsObj.value("ec").toInt();
                    }
                }
                configData.pixelports.push_back(port);
            }
        }
    }


    QJsonValue inputs = rootObj.value("inputs");
    if (inputs.type() == QJsonValue::Array)
    {
        QJsonArray inputsArray = inputs.toArray();
        for (int i = 0; i < inputsArray.count(); i++)
        {
            QJsonValue inputChild = inputsArray.at(i);
            if (inputChild.type() == QJsonValue::Object)
            {
                QJsonObject stringsObj = inputChild.toObject();
                ControllerInput input;
                input.channels = stringsObj.value("channels_per_universe").toInt();
                input.startUniverse = stringsObj.value("start_universe").toInt();
                input.universeCount = stringsObj.value("number_of_universes").toInt();
                //input.type = stringsObj.value("p").toString();
                configData.inputs.push_back(input);
            }
        }
    }
    configData.ip = c->IP;
    //ReadIPFromFile(c->FilePath);
    if (!configData.ip.isEmpty())
    {
        controllers.push_back(configData);
    }
    else
    {
        qDebug() << c->FilePath << "was invalid";
    }
}

void ConfigVisitor::ReadConfig(WLEDController* c)
{
    ControllerData configData;
    auto data = ReadFile(c->FilePath);
    QJsonDocument configJson = QJsonDocument::fromJson(data);

    QJsonObject rootObj = configJson.object();

    QJsonValue hw = rootObj.value("hw");

    QJsonValue led = hw.toObject().value("led");

    QJsonValue ins = led.toObject().value("ins");
    int pixels{0};
    if (ins.type() == QJsonValue::Array)
    {
        QJsonArray stringsArray = ins.toArray();
        for (int i = 0; i < stringsArray.count(); i++)
        {
            QJsonValue stringsChild = stringsArray.at(i);
            if (stringsChild.type() == QJsonValue::Object)
            {
                QJsonObject stringsObj = stringsChild.toObject();
                ControllerPort port;
                port.port = i + 1;
                //port.startUniverse = stringsObj.value("u").toInt();
                port.startChannel = (stringsObj.value("start").toInt() * 3) + 1;
                port.pixels = stringsObj.value("len").toInt();
                port.startNulls = stringsObj.value("skip").toInt();
                pixels += port.pixels;
                //port.endNulls = stringsObj.value("ne").toInt();
                //port.group = stringsObj.value("gp").toInt();
                //port.zigZag = stringsObj.value("z").toInt();
                port.reverse = stringsObj.value("rev").toInt();
                //port.name = stringsObj.value("nm").toString();
                //port.gamma = stringsObj.value("g").toInt() / 10.0;
                int order = stringsObj.value("order").toInt();
                port.colorOrder = c->DecodeColorOrder(order);
                int pixeltype = stringsObj.value("type").toInt();
                port.protocol = c->DecodePixelProtocol(pixeltype);
                configData.pixelports.push_back(port);
            }
        }
    }


    QJsonValue inputf = rootObj.value("if");

    QJsonValue live = inputf.toObject().value("live");

    QJsonValue dmx = live.toObject().value("dmx");

    ControllerInput input;
    input.channels = 510;
    input.startUniverse = dmx.toObject().value("uni").toInt();
    input.universeCount = std::ceil((pixels*3)/510);

    input.type = QString::number(dmx.toObject().value("mode").toInt());
    configData.inputs.push_back(input);

    configData.firmware = QString::number(rootObj.value("vid").toInt());
    QJsonValue id = rootObj.value("id");
    QJsonValue name = id.toObject().value("name");
    configData.name = name.toString();

    configData.ip = c->IP;
    //ReadIPFromFile(c->FilePath);
    controllers.push_back(configData);
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

QString ConfigVisitor::ReadIPFromFile(QString const& file) const
{
    return "";
}