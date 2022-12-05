#include "HinkspixController.h"

QString HinkspixController::BackUpConfig(QString const& folder) const
{
	auto json = DownloadData("http://" + IP + GetJSONInfoURL());
	return SaveData(json.toLatin1(), folder);
}
 