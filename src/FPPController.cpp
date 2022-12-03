#include "FPPController.h"

QString FPPController::BackUpConfig(QString const& folder) const 
{
	auto stringurl = GetBBBStringsURL();
	if ("Pi Hat" == Model)
	{
		stringurl = GetPIStringsURL();
	}
	///fppjson.php?command=getChannelOutputs&file=co-bbbStrings
	auto json = DownloadData("http://" + IP + stringurl);
	return SaveData(json.toLatin1(), folder);
}
 