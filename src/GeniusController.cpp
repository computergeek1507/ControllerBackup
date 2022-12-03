#include "GeniusController.h"

QString GeniusController::BackUpConfig(QString const& folder) const
{
	auto json = DownloadData("http://" + IP + GetConfigURL());
	return SaveData(json.toLatin1(), folder);
}
 