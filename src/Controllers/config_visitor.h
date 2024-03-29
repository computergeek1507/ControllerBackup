#ifndef CONFIGVISITOR_H
#define CONFIGVISITOR_H

#include <QString>
#include <QObject>

#include "controller_data.h"

#include <memory>
#include <vector>

struct FalconV3Controller;
struct FalconV4Controller;
struct FPPController;
struct GeniusController;
struct WLEDController;
//struct ControllerData;

struct ConfigVisitor
{
    //QString Folder;
    //QString File;
    std::vector<ControllerData> controllers;

    void ReadConfig(FalconV3Controller* c);

    void ReadConfig(FalconV4Controller* c);

    void ReadConfig(FPPController* c);

    void ReadConfig(GeniusController* c);

    void ReadConfig(WLEDController* c);

    QByteArray ReadFile(QString const& file) const;
};

#endif