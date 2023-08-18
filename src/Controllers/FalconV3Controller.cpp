#include "FalconV3Controller.h"
#include "config_visitor.h"

void FalconV3Controller::accept(BackupVisitor * v)
{
	v->BackUp(this);
}

void FalconV3Controller::accept(ViewerVisitor * v)
{
	v->DisplayOuputs(this);
}

void FalconV3Controller::accept(ConfigVisitor * v)
{
	v->ReadConfig(this);
}

QString FalconV3Controller::GetFileName() const
{
	QString ip = IP;
	ip = ip.replace(".", "_");
	return Name + "_" + ip + ".xml";
}
 