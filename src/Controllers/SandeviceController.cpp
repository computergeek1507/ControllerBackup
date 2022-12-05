#include "SandeviceController.h"

QString SandeviceController::BackUpConfig(QString const& folder) const
{
	//page = SDGetURL("/");
	auto html = DownloadData("http://" + IP + "/");
	return SaveData(html.toLatin1(), folder);
}
 