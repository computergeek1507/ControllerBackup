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
    auto data = ReadFile(c->GetBackupFile());

    //load the file
    QDomDocument doc;
    doc.setContent(data);
    QDomElement docElem = doc.documentElement();

    QDomElement universeElement = docElem.firstChildElement("universes");
    if (!universeElement.isNull()) {
        QString absolute = universeElement.attribute("a");
        configData.absoluteStartAddress = absolute.toInt();
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
                input.type = c->DecodeInputType(element.attribute("t").toInt());
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
                pixelport.group = element.attribute("g").toInt();
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
    controllers.push_back(configData);
}

void ConfigVisitor::ReadConfig(FalconV4Controller* c)
{
    ControllerData configData;
    auto data = ReadFile(c->GetBackupFile());
	QJsonDocument configJson = QJsonDocument::fromJson(data);

	QJsonObject rootObj = configJson.object();

    QJsonValue controller = rootObj.value("C");
    QJsonValue mode = controller.toObject().value("O");
    int iMode = mode.toInt();
    configData.mode = c->DecodeMode(iMode);

    QJsonValue name = controller.toObject().value("N");
    configData.name = name.toString();
    configData.ip = c->IP;

    QJsonValue version = controller.toObject().value("V");
    configData.firmware = version.toString();

    QJsonValue absolute = controller.toObject().value("A");
    configData.absoluteStartAddress = absolute.toInt();

    QJsonValue topString = rootObj.value("S");
    QJsonValue strings = topString.toObject().value("A");
    if (strings.type() == QJsonValue::Array)
    {
        QString lastprotocol = "ws2811";
        QJsonArray stringsArray = strings.toArray();
        for (int i = 0; i < stringsArray.count(); i++) 
        {
            QJsonValue stringsChild = stringsArray.at(i);
            if (stringsChild.type() == QJsonValue::Object)
            {
                QJsonObject stringsObj = stringsChild.toObject();
                if ((i + 1) % 16 == 1)//only read protocol at 1, 17, 33
                {
                    lastprotocol = c->DecodePixelProtocol(stringsObj.value("l").toInt());
                }
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
                port.protocol = lastprotocol;
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
                input.type = c->DecodeInputType(stringsObj.value("p").toString());
                configData.inputs.push_back(input);
            }
        }
    }
    controllers.push_back(configData);
}

void ConfigVisitor::ReadConfig(FPPController* c)
{
    ControllerData configData;
    auto data = ReadFile(c->GetBackupFile(BackupType::Input));
    QJsonDocument input_Json = QJsonDocument::fromJson(data);

    QJsonObject root_in_Obj = input_Json.object();
    QJsonValue channelInputs = root_in_Obj.value("channelInputs");
    if (channelInputs.type() == QJsonValue::Array)
    {
        QJsonArray inputArray = channelInputs.toArray();
        for (int j = 0; j < inputArray.count(); j++)
        {
            QJsonValue inputChild = inputArray.at(j);
            if (inputChild.type() == QJsonValue::Object)
            {
                QJsonObject inputObj = inputChild.toObject();
                QString in_type = inputObj.value("type").toString();
                if (in_type.compare("universes") == 0)
                {
                    auto uni = inputObj.value("universes");
                    if (uni.type() == QJsonValue::Array)
                    {
                        QJsonArray uniArray = uni.toArray();
                        for (int i = 0; i < uniArray.count(); i++)
                        {
                            QJsonValue uniChild = uniArray.at(i);
                            QJsonObject uniObj = uniChild.toObject();
                            ControllerInput input;
                            input.channels = uniObj.value("channelCount").toInt();
                            input.startUniverse = uniObj.value("id").toInt();
                            input.universeCount = uniObj.value("universeCount").toInt();
                            input.startChannel = uniObj.value("startChannel").toInt();
                            input.type = c->DecodeInputType(uniObj.value("type").toInt());
                            configData.inputs.push_back(input);
                        }
                    }
                }
            }
        }
    }

    data = ReadFile(c->GetBackupFile(BackupType::OuputPixels));
    QJsonDocument strings_Json = QJsonDocument::fromJson(data);

    QJsonObject root_co_Obj = strings_Json.object();
    QJsonValue channelOuputs = root_co_Obj.value("channelOutputs");
    if (channelOuputs.type() == QJsonValue::Array)
    {
        QJsonArray outputArray = channelOuputs.toArray();
        for (int j = 0; j < outputArray.count(); j++)
        {
            QString protocol = "ws281x";
            QJsonValue outputChild = outputArray.at(j);
            if (outputChild.type() == QJsonValue::Object)
            {
                QJsonObject outputObj = outputChild.toObject();
                if (outputObj.contains("pixelTiming"))
                {
                    if (outputObj.value("pixelTiming").toInt() == 1)
                    {
                        protocol = "1903";
                    }
                }
                QJsonValue outputs = outputObj.value("outputs");
                if (outputs.type() == QJsonValue::Array)
                {
                    QJsonArray outputArray = outputs.toArray();
                    for (int k = 0; k < outputArray.count(); k++)
                    {
                        QJsonValue stringChild = outputArray.at(k);
                        QJsonObject stringsObj = stringChild.toObject();
                        ControllerPort port;
                        port.brightness = 30;
                        port.protocol = protocol;
                        port.port = stringsObj.value("portNumber").toInt() + 1;
                        QJsonValue vitualString = stringsObj.value("virtualStrings");
                        if (vitualString.type() == QJsonValue::Array)
                        {
                            QJsonArray vsArray = vitualString.toArray();
                            for (int i = 0; i < vsArray.count(); i++)
                            {
                                QJsonValue vsChild = vsArray.at(i);
                                QJsonObject vsObj = vsChild.toObject();
                                if (i == 0)
                                {
                                    port.startChannel = vsObj.value("startChannel").toInt() + 1;
                                    port.name = vsObj.value("description").toString();
                                    //port.startUniverse = vsObj.value("su").toInt();
                                    port.startNulls = vsObj.value("nullNodes").toInt();
                                    port.endNulls = vsObj.value("endNulls").toInt();
                                    port.reverse = vsObj.value("reverse").toBool();
                                    port.brightness = vsObj.value("brightness").toInt();
                                    port.gamma = vsObj.value("gamma").toString().toDouble();
                                    port.colorOrder = vsObj.value("colorOrder").toString();
                                    port.group = vsObj.value("groupCount").toInt();
                                }
                                port.pixels += vsObj.value("pixelCount").toInt();
                            }
                        }
                        configData.pixelports.push_back(port);
                    }
                }
            }
        }
    }

    configData.ip = c->IP;
    controllers.push_back(configData);
}

void ConfigVisitor::ReadConfig(GeniusController* c)
{
    ControllerData configData;
    auto data = ReadFile(c->GetBackupFile());
    QJsonDocument configJson = QJsonDocument::fromJson(data);

    QJsonObject rootObj = configJson.object();

    QJsonValue system = rootObj.value("system");
    QJsonValue name = system.toObject().value("friendly_name");
    configData.name = name.toString();
    QJsonValue mode = system.toObject().value("operating_mode");
    configData.mode = mode.toString();

    if (system.toObject().contains("firmware_version"))
    {
        configData.firmware = system.toObject().value("firmware_version").toString();
    }

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
                port.protocol = "ws2811";
                port.colorOrder = "RGB";
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
    bool use_universes = system.toObject().value("manage_outputs_by_universe").toBool();
    if (use_universes) {
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
                    input.startChannel = stringsObj.value("start_channel").toInt();
                    input.universeCount = stringsObj.value("number_of_universes").toInt();
                    input.type = configData.mode;
                    configData.inputs.push_back(input);
                }
            }
        }
    }
    else 
    {
        ControllerInput input;
        input.startChannel = system.toObject().value("start_channel").toInt();
        input.type = configData.mode;
        configData.inputs.push_back(input);
    }
    configData.ip = c->IP;
    controllers.push_back(configData);
}

void ConfigVisitor::ReadConfig(WLEDController* c)
{
    ControllerData configData;
    auto data = ReadFile(c->GetBackupFile());
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
