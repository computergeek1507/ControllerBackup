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

    void BackUp(FalconV3Controller* c);
    
    void BackUp(FalconV4Controller* c);

    void BackUp(FPPController* c);

    void BackUp(GeniusController* c);

    void BackUp(WLEDController* c);

    [[nodiscard]] QString DownloadData(QString const& url) const;
	[[nodiscard]] QString DownloadData(QString const& url, QString const& post) const;
	void SaveData(QByteArray const& json, QString const& backupfolder, QString const& fileName);
};

#endif