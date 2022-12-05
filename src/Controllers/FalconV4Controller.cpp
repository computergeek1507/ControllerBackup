#include "FalconV4Controller.h"

#include <QJsonDocument>
#include <QJsonObject>

QString FalconV4Controller::BackUpConfig(QString const& folder) const
{
	QJsonObject controllerObject;
	//"Q", "SP", batch, 0, 0,

	QString url = "http://" + IP + GetAPI();

	controllerObject.insert("C", GetReturnParm(DownloadData(url, BuildParm("Q", "ST"))));
	controllerObject.insert("I", GetReturnParm(DownloadData(url, BuildParm("Q", "IN"))));
	controllerObject.insert("S", GetReturnParm(DownloadData(url, BuildParm("Q", "SP"))));

	controllerObject.insert("SC", GetReturnParm(DownloadData(url, BuildParm("Q", "SC"))));
	controllerObject.insert("P", GetReturnParm(DownloadData(url, BuildParm("Q", "IP"))));

	//{"T":"Q","M":"IN","B":0,"E":0,"I":0,"P":{}}
	QJsonDocument doc(controllerObject);
	//qDebug() << doc.toJson();
	return SaveData(doc.toJson(), folder);
}

QString FalconV4Controller::BuildParm(QString const& type, QString const& method) const
{
	return QString("{\"T\":\"%1\",\"M\":\"%2\",\"B\":%3,\"E\":%4,\"I\":%5,\"P\":{}}").arg(type).arg(method).arg(0).arg(0).arg(0);
}

QJsonObject FalconV4Controller::GetReturnParm(QString const& json) const
{
	QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
	if (!doc.isNull())
	{
		if (doc.isObject())
		{
			if( doc.object().contains("P")) 
			{
				return doc.object().value("P").toObject();
			}
		}
	}
	
	return QJsonObject();
}
 