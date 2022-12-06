#include "FPPController.h"

void FPPController::accept(BackupVisitor * v)
{
	v->BackUp(this);
}
void FPPController::accept(ViewerVisitor * v)
{
	v->DisplayOuputs(this);
}