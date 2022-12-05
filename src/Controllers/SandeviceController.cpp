#include "WLEDController.h"

QString WLEDController::BackUpConfig(QString const& folder) const
{
	auto json = DownloadData("http://" + IP + GetCfgURL());
	return SaveData(json.toLatin1(), folder);
}
 