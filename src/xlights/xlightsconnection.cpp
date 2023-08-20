#include "xlightsconnection.h"

#include <QCoreApplication>
#include <QElapsedTimer>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

#include "spdlog/spdlog.h"

constexpr std::string_view BASEURL = "http://localhost:49913/";

xLightsConnection::xLightsConnection(QObject *parent) :
    QObject(parent)
{
}

bool xLightsConnection::Connect()
{
    GetData(RequestType::GetModels);
    GetData(RequestType::GetControllers);
    return m_connected;
}

void xLightsConnection::AddEffect(QString const& effect, QString const& model, 
        int layer, int startMS, int endMS, QString const& palette, QMap<QString,QString> parmeters)
{
    QString settings;
    for(auto const& key: parmeters.keys())
    {
        settings += QString("%1=%2,").arg(key).arg(parmeters.value(key));
    }
    settings = settings.left(settings.length()-1);

    QMap<QString,QString> all;
    all.insert("target", model);
    all.insert("effect", effect);
    all.insert("layer", QString::number(layer));
    all.insert("startTime", QString::number(startMS));
    all.insert("endTime", QString::number(endMS));
    //all.insert("palette", effect);
    all.insert("settings", settings);
    GetData(RequestType::AddEffect, all);
}

void xLightsConnection::SetModelControllerPort(QString const& model, int port) 
{
    QMap<QString, QString> all;
    all.insert("key", "ModelControllerConnectionPort");
    all.insert("model", model);
    all.insert("data", QString::number(port));
    GetData(RequestType::SetModelControllerPort, all);
}

void xLightsConnection::SetModelController(QString const& model, QString const& controller) 
{
    QMap<QString, QString> all;
    all.insert("key", "Controller");
    all.insert("model", model);
    all.insert("data", controller);
    GetData(RequestType::SetModelController, all);
}

void xLightsConnection::CallGetModel(QString const& model) 
{
    QMap<QString, QString> all;
    all.insert("model", model);
    GetData(RequestType::GetModelInfo, all);
}

[[nodiscard]] std::optional< std::reference_wrapper< Controller const > > xLightsConnection::GetController(QString const& ip) const
{
    if (auto const found{ std::find_if(m_controller.cbegin(),m_controller.cend(),
                                            [&ip](auto& c) { return c.ip.compare(ip, Qt::CaseInsensitive) == 0; }) };
        found != m_controller.cend())
    {
        return *found;
    }
    return std::nullopt;
}

void xLightsConnection::DecodeResponce(QByteArray const& data, RequestType type)
{
	QJsonDocument loadDoc(QJsonDocument::fromJson(data));
	
	switch (type)
	{
	    case RequestType::GetSequence:
        {
            QJsonObject xLightObject = loadDoc.object();
		    QString seq = xLightObject["seq"].toString();
            QString media = xLightObject["media"].toString();
            int frames = xLightObject["len"].toInt();
            int frameLen = xLightObject["framems"].toInt();
            m_frameTime = frameLen;
            m_frames = frames;
            m_audioFile = media;
            emit UpdateSequence(seq, media, frames, frameLen);

            break;
        }
	    case RequestType::GetModels:
        {
            QStringList models;
		    QJsonArray modelArray = loadDoc.array();
		    for (auto const& model : modelArray)
		    {
                models.append(model.toString());
		    }
            m_modelList = models;
            emit UpdateModels(models);
            break;
        }
        case RequestType::GetModelInfo:
        {
            QMap<QString, QString> models;
            QJsonObject modelArray = loadDoc.object();
            for(const QString & key: modelArray.keys()) {
                QJsonValue value = modelArray.value(key);
                models.insert(key, value.toString());
            }
            //for (auto const& model : modelArray)
            //{
            //    models.insert(model.key);
            //}
            m_modelSettings = models;
            //emit UpdateModels(models);
            break;
        }
        case RequestType::GetControllers:
        {
            //QMap<QString, QString> controllers;
            QJsonArray controllerArray = loadDoc.array();
            int count = controllerArray.count();
            for (int i = 0; i < count; ++i) 
            {
                QJsonObject controllerObject = controllerArray.at(i).toObject();
                Controller con;
                con.index = i;
                con.ip = controllerObject["ip"].toString();
                con.name = controllerObject["name"].toString();

                m_controller.push_back(con);
            }
            
            //emit UpdateModels(models);
            break;
        }
        case RequestType::AddEffect:
        {
            QJsonObject xLightObject = loadDoc.object();
            break;
        }
        case RequestType::SetModelController:
        {
            QJsonObject xLightObject = loadDoc.object();
            break;
        }
        case RequestType::SetModelControllerPort:
        {
            QJsonObject xLightObject = loadDoc.object();
            break;
        }
        default:
            break;
	}
}

QString xLightsConnection::GenerateUrl(RequestType type, QMap<QString,QString> param) const
{
    QString extra{ "?" };
    for (auto const& key : param.keys())
    {
        extra += QString("%1=%2&").arg(key).arg(param.value(key));
    }
    extra = extra.left(extra.length() - 1);
    switch (type)
    {
        case RequestType::GetSequence:
		    return QString(BASEURL.data()) + "getOpenSequence";
	    case RequestType::GetModels:
		    return QString(BASEURL.data()) + "getModels?groups=false";
        case RequestType::GetModelInfo:
            return QString(BASEURL.data()) + "getModel" + extra;
        case RequestType::AddEffect:
		    return QString(BASEURL.data()) + "addEffect" + extra;
        case RequestType::GetControllers:
            return QString(BASEURL.data()) + "getControllers";
        case RequestType::SetModelController:
        case RequestType::SetModelControllerPort:
            return QString(BASEURL.data()) + "setModelProperty" + extra;
       
    }
    return QString();
    //"getOpenSequence"
}

void xLightsConnection::GetData(RequestType type, QMap<QString,QString> param)
{
    auto url = QUrl(GenerateUrl(type, param));
    QNetworkAccessManager manager;
    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);
    QNetworkReply* response = manager.get(request);

    QElapsedTimer timer;
    timer.start();

    while (!response->isFinished())
    {
        if (timer.elapsed() >= ( 5000))
        {
            response->abort();
            m_connected = false;
            auto logger = spdlog::get("ControllerBackup");
            logger->error("Failed to Connect to xLights");
            return;
        }
        QCoreApplication::processEvents();
    }
    m_connected = true;
    auto content = response->readAll();
    response->deleteLater();
    DecodeResponce(content, type);
}