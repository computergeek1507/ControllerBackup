
#include "viewer_visitor.h"

#include "../Controllers/FalconV3Controller.h"
#include "../Controllers/FalconV4Controller.h"
#include "../Controllers/FPPController.h"
#include "../Controllers/GeniusController.h"
#include "../Controllers/WLEDController.h"

#include <QFile>
#include <QTextStream>

void ViewerVisitor::DisplayOuputs(FalconV3Controller* c)
{
    ReadFile(c->FilePath);
}

void ViewerVisitor::DisplayOuputs(FalconV4Controller* c)
{
    ReadFile(c->FilePath);
}

void ViewerVisitor::DisplayOuputs(FPPController* c)
{
    ReadFile(c->FilePath);
}

void ViewerVisitor::DisplayOuputs(GeniusController* c)
{
    ReadFile(c->FilePath);
}

void ViewerVisitor::DisplayOuputs(WLEDController* c)
{
    ReadFile(c->FilePath);
}

void ViewerVisitor::ReadFile(QString const& file)
{
    QFile f(file);

    if (!f.open(QFile::ReadOnly | QFile::Text))
    { 
        return;
    }
    QTextStream in(&f);
    fileData = in.readAll();
}