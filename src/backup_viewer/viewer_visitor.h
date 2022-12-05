
#ifndef VIEWERVISITOR_H
#define VIEWERVISITOR_H

#include <QString>

#include <memory>
#include <vector>

struct FalconV3Controller;
struct FalconV4Controller;
struct FPPController;
struct GeniusController;
struct WLEDController;

struct ViewerVisitor
{
    void DisplayOuputs(FalconV3Controller* c);
    void DisplayOuputs(FalconV4Controller* c);
    void DisplayOuputs(FPPController* c);
    void DisplayOuputs(GeniusController* c);
    void DisplayOuputs(WLEDController* c);

    QString fileData;

    void ReadFile(QString const& file);
};

#endif