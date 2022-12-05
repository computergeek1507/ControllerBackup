#include "AlphapixController.h"

QString AlphapixController::BackUpConfig(QString const& folder) const
{
	//_page = GetURL("/");
	auto html = DownloadData("http://" + IP + "/");
	return SaveData(html.toLatin1(), folder);
}
 