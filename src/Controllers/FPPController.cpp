#include "FPPController.h"

void FPPController::accept(BackupVisitor * v)
{
	v->BackUpFPP(this);
}
void FPPController::accept(ViewerVisitor * v)
{
	v->DisplayOuputs(this);
}