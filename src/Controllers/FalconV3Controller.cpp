#include "FalconV3Controller.h"
#include "config_visitor.h"

void FalconV3Controller::accept(BackupVisitor * v)
{
	v->BackUp(this);
}

void FalconV3Controller::accept(ConfigVisitor * v)
{
	v->ReadConfig(this);
}

QString FalconV3Controller::GetFileExtension() const
{
	return ".xml";
}

QString FalconV3Controller::DecodeMode(int mode) const
{
    switch (mode) {
    case 0: return "E131/ArtNET";
    case 2: return "Player";
    case 4: return "Remote";
    case 8: return "Master";
    case 16: return "ZCPP";
    case 64: return "DDP";
    }
    return QString();
}

QString FalconV3Controller::DecodePixelProtocol(int protocol) const
{
	switch (protocol) {
	case 0: return "ws2811";
	case 1: return "tm18xx";
	case 2: return "lx1203";
	case 3: return "ws2801";
	case 4: return "tls3001";
	case 5: return "lpd6803";
	case 6: return "gece";
	case 7: return "lpd8806";
	case 8: return "apa102";
	case 9: return "ucs1903";
	case 10: return "dm412";
	case 11: return "p9813";
	case 12: return "ucs2903";
	case 14: return "tm1814";
	}
	return "";
}

QString FalconV3Controller::DecodeSerialProtocol(int protocol) const {

	switch (protocol) {
	case 0: return "dmx";
	case 1: return "pixelnet";
	case 2: return "renard";
	}
	return QString();
}

QString FalconV3Controller::DecodeColorOrder(int color) const
{
	switch (color) {
	case 0: return "RGB";
	case 1: return "RBG";
	case 2: return "GRB";
	case 3: return "GBR";
	case 4: return "BRG";
	case 5: return "BGR";
	default: break;
	}
	return "RGB";
}

int FalconV3Controller::DecodeBrightness(int brightnessCode) const {

	switch (brightnessCode) {
	case 0: return 100;
	case 1: return 95;
	case 2: return 90;
	case 3: return 85;
	case 4: return 80;
	case 5: return 75;
	case 6: return 70;
	case 7: return 65;
	case 8: return 60;
	case 9: return 50;
	case 10: return 40;
	case 11: return 30;
	case 12: return 20;
	case 13: return 10;
	default: break;
	}
	return 100;
}

float FalconV3Controller::DecodeGamma(int gammaCode) const {

	switch (gammaCode) {
	case 0: return 1.0f;
	case 1: return 2.0f;
	case 2: return 2.3f;
	case 3: return 2.5f;
	case 4: return 2.8f;
	case 5: return 3.0f;
	default: break;
	}
	return 1.0f;
}

