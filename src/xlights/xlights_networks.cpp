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
        m_logger->warn("Failed to open file");
        return 0;
    }
    if (!xgbnetworks_doc.setContent(&file))
    {
        m_logger->warn("failed to parse file");
        file.close();
        return 0;
    }

    file.close();

    QDomElement docEle = xgbnetworks_doc.documentElement();
    QDomNodeList elements = docEle.elementsByTagName("LAMPS");

    QFile outFile(xgbNetworks);
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        m_logger->warn("Failed to open file for writing.");
        return 0;
    }

    QTextStream stream(&outFile);
    stream << xgbnetworks_doc.toString();

    outFile.close();
    return 0;
}

void xLightsNetworks::SetController(QString const& ip, QString const& vendor, QString const& model, QString const& variant)
{
   
}

