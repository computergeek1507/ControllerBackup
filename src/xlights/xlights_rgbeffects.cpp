#include "xlights_rgbeffects.h"

#include <QProgressDialog>
#include <QThread>
#include <QFile>
#include <QTextStream>

xLightsRGBEffects::xLightsRGBEffects(QObject* parent) :
	QObject(parent), m_logger(spdlog::get("ControllerBackup"))
{
}

bool xLightsRGBEffects::LoadXML(QString const& xgbEffects)
{
    QFile file(xgbEffects);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        m_logger->warn("Failed to Open xLights XRB Effects File");
        return false;
    }
    if (!rgbeffects_doc.setContent(&file))
    {
        m_logger->warn("Failed to Parse xLights XRB Effects File");
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
                if (rgbeffects_data[j].startsWith("    </model>"))
                {
                    j--;
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
            return false;
        }
    }
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
            rgbeffects_data[i].replace(">", " ModelChain=\">" + chainModel + "\">");
            return true;
        }
    }
    return false;
}

int xLightsRGBEffects::GetModelControllerPort(QString const& model) const
{
    auto models = rgbeffects_doc.elementsByTagName("model");
    for (int i = 0; i < models.size(); ++i)
    {
        QDomElement domElement = models.at(i).toElement();
        QDomAttr attribute = domElement.attributeNode("name");
        if (attribute.value() == model) {
            //if (!domElement.firstChild().hasAttributes())
            {
                //domElement.firstChildElement();
    
                auto name = domElement.firstChildElement("ControllerConnection").tagName();
                //nodeTag.setTagName("new_amazing_tag_name");
                QString sport = domElement.firstChildElement("ControllerConnection").attribute("Port", "-1");
                return sport.toInt();
            }
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
    return false;
}

QStringList xLightsRGBEffects::GetModels() const
{
    QStringList modelNames;
    auto models = rgbeffects_doc.elementsByTagName("model");

    for (int i = 0; i < models.size(); ++i)
    {
        QDomElement domElement = models.at(i).toElement();
        QDomAttr attribute = domElement.attributeNode("name");
        modelNames.append(attribute.value());
    }
    return modelNames;
}

QMap<QString, QString> xLightsRGBEffects::GetModelSettings(QString const& model) const
{
    auto models = rgbeffects_doc.elementsByTagName("model");
    QMap<QString, QString> modelSettings;
    for (int i = 0; i < models.size(); ++i)
    {
        QDomElement domElement = models.at(i).toElement();
        QDomAttr attribute = domElement.attributeNode("name");
        if (attribute.value() == model) {
            //domElement.attributes("Controller", controller);
            for (int i = 0; i < domElement.attributes().length(); ++i) {
                auto inode = domElement.attributes().item(i);
                auto attr = inode.toAttr();
                //qDebug() << "Name: " << attr.name();
                //qDebug() << "Value: " << attr.value();
                modelSettings.insert(attr.name(), attr.value());
            }
        }
    }
    return modelSettings;
}