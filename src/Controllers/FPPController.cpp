#include "FPPController.h"
#include "config_visitor.h"

void FPPController::accept(BackupVisitor * v)
{
	v->BackUp(this);
}

void FPPController::accept(ConfigVisitor * v)
{
	v->ReadConfig(this);
}

QString FPPController::DecodeInputType(int type) const
{
	switch (type) {
	case 0: return "E1.31 - Multicast";
	case 1: return "E1.31 - Unicast";
	case 2: return "ArtNet";
	default: break;
	}
	return "Unknown";
}