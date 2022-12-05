#ifndef FALCONV3CONTROLLER_H
#define FALCONV3CONTROLLER_H

#include "BaseController.h"
#include "BackupVisitor.h"
#include "../backup_viewer/viewer_visitor.h"

#include <QString>
#include <memory>

struct FalconV3Controller : BaseController
{
	FalconV3Controller(QString name, QString  ip): BaseController(std::move(name), std::move(ip)) {}
	QString GetType() const override{ return "FalconV3"; };
	QString GetFileName() const override;

	void accept(BackupVisitor * v) override;
	void accept(ViewerVisitor * v) override;
};

#endif
