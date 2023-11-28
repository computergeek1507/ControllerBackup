#include "GeniusController.h"
#include "BackupVisitor.h"
#include "config_visitor.h"

void GeniusController::accept(BackupVisitor * v)
{
	v->BackUp(this);
}

void GeniusController::accept(ConfigVisitor * v)
{
	v->ReadConfig(this);
}