#include "xlights_rgbeffects.h"

#include <QProgressDialog>
#include <QThread>
#include <QFile>
#include <QTextStream>
#include <QDebug>

xLightsRGBEffects::xLightsRGBEffects(QObject* parent) :
	QObject(parent), m_logger(spdlog::get("ControllerBackup"))
{
}

bool xLightsRGBEffects::LoadXML(QString const& xgbEffects)
{
    //	//xlights_rgbeffects_pre2026.04.xml
    QFile file(xgbEffects);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        m_logger->warn("Failed to Open xLights XRB Effects File");
        return false;
    }
    QByteArray xmlData = file.readAll();
    pugi::xml_parse_result result = rgbeffects_doc.load_buffer(xmlData.constData(), xmlData.size());
    if (!result)
    {
        m_logger->warn("Failed to Parse xLights XRB Effects File");
        m_logger->warn("Offset: {} Error: {}", result.offset, result.description());
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
        rgbeffects_data.append(line);
    }

    file.close();

    //QDomElement docEle = rgbeffects_doc.documentElement();
    //QDomNodeList elements = docEle.elementsByTagName("LAMPS");
    return true;
}

bool xLightsRGBEffects::SaveXML(QString const& xgbEffects)
{
    QFile outFile(xgbEffects);
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        m_logger->warn("Failed to open file for writing.");
        return false;
    }

    QTextStream stream(&outFile);
    //stream << rgbeffects_doc.toString();

    for (auto const& line : rgbeffects_data)
    {
        stream << line << Qt::endl;
    }
    outFile.flush();
    outFile.close();
    return true;
}

bool xLightsRGBEffects::SetModelControllerPort(QString const& model, int port)
{
    //auto models = rgbeffects_doc.elementsByTagName("model");
    //for (int i = 0; i < models.size(); ++i)
    //{
    //    QDomElement domElement = models.at(i).toElement();
    //    QDomAttr attribute = domElement.attributeNode("name");
    //    if (attribute.value() == model) {
    //        //if (!domElement.firstChild().hasAttributes())
    //        {
    //            //domElement.firstChildElement();
    //
    //            //qDebug() << domElement.firstChildElement("ControllerConnection").tagName();
    //            //nodeTag.setTagName("new_amazing_tag_name");
    //            domElement.firstChildElement("ControllerConnection").setAttribute("Protocol", "ws2811");
    //            domElement.firstChildElement("ControllerConnection").setAttribute("Port", QString::number(port));
    //        }
    //    }
    //}

    for (int i = 0; i < rgbeffects_data.count(); ++i)
    {//    <model 
        if (rgbeffects_data[i].startsWith("    <model ") && rgbeffects_data[i].contains("name=\"" + model + "\""))
        {
            int j = i;
            for (;j < rgbeffects_data.count(); ++j) 
            {
                if (rgbeffects_data[j].startsWith("      <ControllerConnection"))
                {
                    //j--;
                    break;
                }
            }
            if (rgbeffects_data[j].contains("Port=\""))
            {
                return false;
            }
            if (rgbeffects_data[j].contains("<ControllerConnection/>"))
            {
                rgbeffects_data[j].replace("<ControllerConnection/>", "<ControllerConnection Port=\"" + QString::number(port) + "\" Protocol=\"ws2811\"/>");
                return true;
            }
            if (rgbeffects_data[j].contains("Protocol=\"ws2811\""))
            {
                rgbeffects_data[j].replace("Protocol=\"ws2811\"", "Port=\"" + QString::number(port) + "\" Protocol=\"ws2811\"");
                return true;
            }
            m_logger->warn("Failed to set controller port: {} \"{}\"", model.toStdString(), rgbeffects_data[j].toStdString());
            qDebug() << "Failed to set controller port: " << model << " \"" << rgbeffects_data[j] << "\"";
            return false;
        }
    }
    m_logger->warn("Failed to set controller port: {}", model.toStdString());
    qDebug() << "Failed to set controller port: " << model;
    return false;
}

bool xLightsRGBEffects::SetModelControllerChain(QString const& model, QString const& chainModel)
{
    for (int i = 0; i < rgbeffects_data.count(); ++i)
    {//    <model 
        if (rgbeffects_data[i].startsWith("    <model ") && rgbeffects_data[i].contains("name=\"" + model + "\""))
        {
            if (rgbeffects_data[i].contains("ModelChain=\""))
            {
                return true;
            }
            rgbeffects_data[i].replace(">", " ModelChain=\"&gt;" + chainModel + "\">");
            return true;
        }
    }
    m_logger->warn("Failed to set model chain: {}", model.toStdString());
    qDebug() << "Failed to set model chain: " << model;
    return false;
}

int xLightsRGBEffects::GetModelControllerPort(QString const& model) const
{
    pugi::xml_node root = rgbeffects_doc.document_element();
    
    for (pugi::xml_node modelNode : root.children("model"))
    {
        pugi::xml_attribute nameAttr = modelNode.attribute("name");
        if (nameAttr && QString::fromUtf8(nameAttr.value()) == model) {
            pugi::xml_node connNode = modelNode.child("ControllerConnection");
            if (connNode) {
                pugi::xml_attribute portAttr = connNode.attribute("Port");
                if (portAttr) {
                    return QString::fromUtf8(portAttr.value()).toInt();
                }
            }
            return -1;
        }
    }
    return -1;
}

bool xLightsRGBEffects::SetModelController(QString const& model, QString const& controller)
{
    //auto models = rgbeffects_doc.elementsByTagName("model");
    //
    //for (int i = 0; i < models.size(); ++i)
    //{
    //    QDomElement domElement = models.at(i).toElement();
    //    QDomAttr attribute = domElement.attributeNode("name");
    //    if (attribute.value() == model) {
    //        domElement.setAttribute("Controller", controller);
    //    }
    //}

    for(int i=0; i< rgbeffects_data.count();++i)
    {//    <model 
        if (rgbeffects_data[i].startsWith("    <model ") && rgbeffects_data[i].contains("name=\"" + model + "\""))
        {
            if (rgbeffects_data[i].contains("Controller=\"" ))
            {
                return true;
            }
            rgbeffects_data[i].replace(">", " Controller=\"" + controller + "\">");
            return true;
        }
    }
    m_logger->warn("Failed to set model Controller: {}", model.toStdString());
    qDebug() << "Failed to set model Controller: " << model;
    return false;
}

QStringList xLightsRGBEffects::GetModels() const
{
    QStringList modelNames;
    pugi::xml_node root = rgbeffects_doc.document_element();
    
    for (pugi::xml_node model : root.children("model"))
    {
        pugi::xml_attribute nameAttr = model.attribute("name");
        if (nameAttr)
        {
            modelNames.append(QString::fromUtf8(nameAttr.value()));
        }
    }
    return modelNames;
}

QMap<QString, QString> xLightsRGBEffects::GetModelSettings(QString const& model) const
{
    QMap<QString, QString> modelSettings;
    pugi::xml_node root = rgbeffects_doc.document_element();
    
    for (pugi::xml_node modelNode : root.children("model"))
    {
        pugi::xml_attribute nameAttr = modelNode.attribute("name");
        if (nameAttr && QString::fromUtf8(nameAttr.value()) == model) {
            for (pugi::xml_attribute attr : modelNode.attributes())
            {
                modelSettings.insert(QString::fromUtf8(attr.name()), QString::fromUtf8(attr.value()));
            }
            break;
        }
    }
    return modelSettings;
}