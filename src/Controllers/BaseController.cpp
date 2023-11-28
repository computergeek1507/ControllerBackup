#include "BaseController.h"

#include <QFileInfo>

QString BaseController::GetFileName(BackupType type) const
{
	QString postFix;

    switch (type)
    {
        case BackupType::Input:
            postFix = "_inputs";
            break;
        case BackupType::OuputAll:
            postFix = "_outputs";
            break;
        case BackupType::OuputPixels:
            postFix = "_out_pixels";
            break;
        case BackupType::OuputSerials:
            postFix = "_out_serials";
            break;
        default:
            break;
    }

	QString ip = IP;
	ip = ip.replace(".", "_");
	return Name + "_" + ip + postFix + GetFileExtension();
}

QString BaseController::GetBackupFile(BackupType type) const
{
    //if (auto const found{ std::find_if(FilePaths.cbegin(),FilePaths.cend(),
    //                                        [&type](auto const f) { return f.Type = type; }) };
    //                                        found != FilePaths.cend())
    //{
    //    return (*found).Path;
    //}
    for (auto const& [ttype,path]: FilePaths)
    {
        if (ttype == type)
        {
            return path;
        }
    }
    return QString();
}

QStringList BaseController::GetBackupFilePaths() const
{
    QStringList paths;
    for (auto const& [_,path]: FilePaths)
    {
        paths.append(path);
    }
    return paths;
}

QStringList BaseController::GetBackupFileNames() const
{
    QStringList names;
    for (auto const& [_, path] : FilePaths)
    {
        QFileInfo info(path);
        names.append(info.fileName());
    }
    return names;
}

QString BaseController::GetFileExtension() const
{
    return ".json";
}
