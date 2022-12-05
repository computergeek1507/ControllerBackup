#include "FalconV3Controller.h"


void FalconV3Controller::accept(BackupVisitor * v)
{
	v->BackUpFalconV3(this);
}

void FalconV3Controller::accept(ViewerVisitor * v)
{
	v->DisplayOuputs(this);
}

QString FalconV3Controller::GetFileName() const
{
	QString ip = IP;
	ip = ip.replace(".", "_");
	return Name + "_" + ip + ".xml";
}
 