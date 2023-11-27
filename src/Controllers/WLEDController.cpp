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

QString WLEDController::DecodePixelProtocol(int protocol) const
{
	switch (protocol) {
	case 22: return "ws2811";
	case 25: return "tm1829";
	case 26: return "ucs8903";
	case 29: return "ucs8904";
	case 30: return "sk6812rgbw";
	//case 30: return "apa109";
	case 31: return "tm1814";

	case 50: return "ws2801";
	case 51: return "apa102";
	case 52: return "lpd8806";
	case 53: return "p9813";
	case 54: return "lpd6803";

	}
	return "";
}

QString WLEDController::DecodeColorOrder(int color) const
{
    switch (color)
    {
        case 0: return "GRB";
        case 1: return "RGB";
        case 2: return "BRG";
        case 3: return "RBG";
        case 4: return "BGR";
        case 5: return "GBR";
    }
    return QString();
}