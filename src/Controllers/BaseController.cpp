#include "BaseController.h"

QString BaseController::GetFileName() const
{
	QString ip = IP;
	ip = ip.replace(".", "_");
	return Name + "_" + ip + ".json";
}
