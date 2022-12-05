#include "GeniusController.h"
#include "BackupVisitor.h"
#include "../backup_viewer/viewer_visitor.h"

void GeniusController::accept(BackupVisitor * v)
{
	v->BackUpGenius(this);
}
void GeniusController::accept(ViewerVisitor * v)
{
	v->DisplayOuputs(this);
}