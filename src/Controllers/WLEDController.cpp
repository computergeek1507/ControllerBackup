#include "WLEDController.h"

#include "BackupVisitor.h"
#include "../backup_viewer/viewer_visitor.h"
#include "config_visitor.h"

void WLEDController::accept(BackupVisitor * v)
{
	v->BackUp(this);
}
void WLEDController::accept(ViewerVisitor * v)
{
	v->DisplayOuputs(this);
}

void WLEDController::accept(ConfigVisitor * v)
{
	v->ReadConfig(this);
}