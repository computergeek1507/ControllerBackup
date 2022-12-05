#ifndef BACKUPVISITOR_H
#define BACKUPVISITOR_H



#include <QString>
#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>

#include <memory>
#include <vector>

struct FalconV3Controller;
struct FalconV4Controller;
struct FPPController;
struct GeniusController;
struct WLEDController;

struct BackupVisitor
{
    QString Folder;
    //QString File;
    QString BackUpPath;

    BackupVisitor(QString folder) : Folder(folder)
    {}

    void BackUpFalconV3(FalconV3Controller* c);
    
    void BackUpFalconV4(FalconV4Controller* c);

    void BackUpFPP(FPPController* c);

    void BackUpGenius(GeniusController* c);

    void BackUpWLED(WLEDController* c);

    [[nodiscard]] QString DownloadData(QString const& url) const;
	[[nodiscard]] QString DownloadData(QString const& url, QString const& post) const;
	void SaveData(QByteArray const& json, QString const& backupfolder, QString const& fileName);
};

#endif