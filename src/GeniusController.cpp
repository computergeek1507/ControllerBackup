#include "GeniusController.h"

void GeniusController::BackUpConfig(QString const& folder) const 
{
	auto json = DownloadJson("http://" + IP + GetConfigURL());
	SaveJson(json, folder);
}
 