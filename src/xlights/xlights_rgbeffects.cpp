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
        m_logger->warn("Failed to open file");
        return 0;
    }
    if (!rgbeffects_doc.setContent(&file))
    {
        m_logger->warn("failed to parse file");
        file.close();
        return 0;
    }

    file.close();

    QDomElement docEle = rgbeffects_doc.documentElement();
    QDomNodeList elements = docEle.elementsByTagName("LAMPS");

    QFile outFile(xgbEffects);
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        m_logger->warn("Failed to open file for writing.");
        return 0;
    }

    QTextStream stream(&outFile);
    stream << rgbeffects_doc.toString();

    outFile.close();
    return 0;
}

void xLightsRGBEffects::SetModelControllerPort(QString const& model, int port)
{
   
}

void xLightsRGBEffects::SetModelController(QString const& model, QString const& controller)
{
    
}