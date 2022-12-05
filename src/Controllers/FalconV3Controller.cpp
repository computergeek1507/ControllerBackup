#include "FalconV3Controller.h"

QString FalconV3Controller::BackUpConfig(QString const& folder) const
{
	//http://192.168.5.217/settings.xml
	auto json = DownloadData("http://" + IP + "/settings.xml");
	return SaveData(json.toLatin1(), folder);
}

QString FalconV3Controller::GetFileName() const
{
	QString ip = IP;
	ip = ip.replace(".", "_");
	return Name + "_" + ip + ".xml";
}
 