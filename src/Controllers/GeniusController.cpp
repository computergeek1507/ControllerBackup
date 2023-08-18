#include "GeniusController.h"
#include "BackupVisitor.h"
#include "../backup_viewer/viewer_visitor.h"
#include "config_visitor.h"

void GeniusController::accept(BackupVisitor * v)
{
	v->BackUp(this);
}
void GeniusController::accept(ViewerVisitor * v)
{
	v->DisplayOuputs(this);
}

void GeniusController::accept(ConfigVisitor * v)
{
	v->ReadConfig(this);
}