#include "xlights_networks.h"

#include <QProgressDialog>
#include <QThread>
#include <QFile>
#include <QTextStream>

xLightsNetworks::xLightsNetworks(QObject* parent) :
	QObject(parent), m_logger(spdlog::get("ControllerBackup"))
{
}

bool xLightsNetworks::LoadXML(QString const& xgbNetworks)
{
    QFile file(xgbNetworks);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        m_logger->warn("Failed to Open xLights Networks File");
        return false;
    }
    if (!xgbnetworks_doc.setContent(&file))
    {
        m_logger->warn("Failed to Parse xLights Networks File");
        file.close();
        return false;
    }
    file.close();
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        m_logger->warn("Failed to Open xLights XRB Effects File");
        return false;
    }
    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        xgbnetworks_data.append(line);
    }

    file.close();

    //QDomElement docEle = xgbnetworks_doc.documentElement();
    //QDomNodeList elements = docEle.elementsByTagName("LAMPS");
    return true;
}

bool xLightsNetworks::SaveXML(QString const& xgbNetworks)
{
    QFile outFile(xgbNetworks);
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        m_logger->warn("Failed to open file for writing.");
        return false;
    }

    QTextStream stream(&outFile);
    //stream << xgbnetworks_doc.toString();
    for (auto const& line : xgbnetworks_data)
    {
        stream << line << Qt::endl;
    }
    outFile.flush();
    outFile.close();
    return true;
}

bool xLightsNetworks::SetController(QString const& ip, QString const& vendor, QString const& model, QString const& variant)
{
    for (int i = 0; i < xgbnetworks_data.count(); ++i)
    {//    <model 
        if (xgbnetworks_data[i].startsWith("  <Controller ") && xgbnetworks_data[i].contains("IP=\"" + ip + "\""))
        {
            xgbnetworks_data[i].replace("Vendor=\"\" Model=\"\" Variant=\"\"", "Vendor=\"" + vendor + "\" Model=\"" + model + "\" Variant=\"" + variant + "\"");
            return true;
        }
    }
    return false;
}

QStringList xLightsNetworks::GetControllerIPs() const 
{
    QStringList controllerIPs;
    auto models = xgbnetworks_doc.elementsByTagName("Controller");

    for (int i = 0; i < models.size(); ++i)
    {
        QDomElement domElement = models.at(i).toElement();
        QDomAttr attribute = domElement.attributeNode("IP");
        controllerIPs.append(attribute.value());
    }
    return controllerIPs;
}

QStringList xLightsNetworks::GetControllerNames() const 
{
    QStringList controllerNames;
    auto models = xgbnetworks_doc.elementsByTagName("Controller");

    for (int i = 0; i < models.size(); ++i)
    {
        QDomElement domElement = models.at(i).toElement();
        QDomAttr attribute = domElement.attributeNode("Name");
        controllerNames.append(attribute.value());
    }
    return controllerNames;
}

QMap<QString, QString> xLightsNetworks::GetControllerIPMap() const 
{
    QMap<QString, QString> controllers;
    auto models = xgbnetworks_doc.elementsByTagName("Controller");

    for (int i = 0; i < models.size(); ++i)
    {
        QDomElement domElement = models.at(i).toElement();
        QDomAttr attributeIP = domElement.attributeNode("IP");
        QDomAttr attributeName = domElement.attributeNode("Name");
        controllers.insert(attributeIP.value(), attributeName.value());
    }
    return controllers;
}

