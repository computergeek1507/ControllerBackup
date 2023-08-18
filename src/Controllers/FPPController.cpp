#include "FPPController.h"
#include "config_visitor.h"

void FPPController::accept(BackupVisitor * v)
{
	v->BackUp(this);
}
void FPPController::accept(ViewerVisitor * v)
{
	v->DisplayOuputs(this);
}

void FPPController::accept(ConfigVisitor * v)
{
	v->ReadConfig(this);
}