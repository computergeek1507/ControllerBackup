#include "FPPController.h"

void FPPController::BackUpConfig(QString const& folder) const 
{
	///fppjson.php?command=getChannelOutputs&file=co-bbbStrings
	auto json = DownloadJson("http://" + IP + GetBBBStringsURL());
	SaveJson(json, folder);
}
 