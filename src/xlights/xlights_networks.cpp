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
    pugi::xml_document xmlNetworks;
    if (!QFile::exists(xgbNetworks))
    {
        m_logger->warn("xlights_networks.xml not found in {}", xgbNetworks.toStdString());
        return false;
    }
    try {
        QFile file(xgbNetworks);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            m_logger->warn("Failed to Open xLights Networks File");
            return false;
        }
        QByteArray xmlData = file.readAll();
        pugi::xml_parse_result result = xgbnetworks_doc.load_buffer(xmlData.constData(), xmlData.size());
        if (!result)
        {
            m_logger->warn("Failed to Parse xLights Networks File");
            m_logger->warn("Offset: {} Error: {}", result.offset, result.description());
            file.close();
            return false;
        }
        file.close();
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            m_logger->warn("Failed to Open xLights Networks File");
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
    catch(const std::exception& e)
    {
        m_logger->warn("Exception caught while loading xLights Networks File: {}", e.what());
        return false;
    }
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
    pugi::xml_node root = xgbnetworks_doc.document_element();
    
    for (pugi::xml_node controller : root.children("Controller"))
    {
        pugi::xml_attribute ipAttr = controller.attribute("IP");
        if (ipAttr)
        {
            controllerIPs.append(QString::fromUtf8(ipAttr.value()));
        }
    }
    return controllerIPs;
}

QStringList xLightsNetworks::GetControllerNames() const 
{
    QStringList controllerNames;
    pugi::xml_node root = xgbnetworks_doc.document_element();
    
    for (pugi::xml_node controller : root.children("Controller"))
    {
        pugi::xml_attribute nameAttr = controller.attribute("Name");
        if (nameAttr)
        {
            controllerNames.append(QString::fromUtf8(nameAttr.value()));
        }
    }
    return controllerNames;
}

QMap<QString, QString> xLightsNetworks::GetControllerIPMap() const 
{
    QMap<QString, QString> controllers;
    pugi::xml_node root = xgbnetworks_doc.document_element();
    
    for (pugi::xml_node controller : root.children("Controller"))
    {
        pugi::xml_attribute ipAttr = controller.attribute("IP");
        pugi::xml_attribute nameAttr = controller.attribute("Name");
        if (ipAttr && nameAttr)
        {
            controllers.insert(QString::fromUtf8(ipAttr.value()), QString::fromUtf8(nameAttr.value()));
        }
    }
    return controllers;
}

