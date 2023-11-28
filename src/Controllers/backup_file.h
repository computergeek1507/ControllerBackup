#ifndef BACKUPFILE_H
#define BACKUPFILE_H

#include <QString>
#include <array>

enum class BackupType : int
{
	All=0,
	Input,
	OuputAll,
	OuputPixels,
	OuputSerials
};

constexpr std::array<BackupType, 5> BACKUP_TYPES{ BackupType::All, 
									BackupType::Input, BackupType::OuputAll,
									BackupType::OuputPixels, BackupType::OuputSerials }; // constexpr is important here

struct BackupFile
{
	BackupFile() {}
	BackupFile(BackupType type, QString path) :Type(type), Path(path) {}
	BackupType Type{ BackupType::All};
	QString Path;
};

#endif