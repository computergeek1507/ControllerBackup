#include "FalconV4Controller.h"
#include "config_visitor.h"

#include <QJsonDocument>
#include <QJsonObject>

void FalconV4Controller::accept(BackupVisitor * v)
{
	v->BackUp(this);
}
void FalconV4Controller::accept(ViewerVisitor * v)
{
	v->DisplayOuputs(this);
}

void FalconV4Controller::accept(ConfigVisitor * v)
{
	v->ReadConfig(this);
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
 