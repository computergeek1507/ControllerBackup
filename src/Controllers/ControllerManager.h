#ifndef CONTROLLERMANAGER_H
#define CONTROLLERMANAGER_H

#include "BaseController.h"

#include "spdlog/spdlog.h"

#include <QString>
#include <QObject>

#include <memory>
#include <vector>

class ControllerManager: public QObject
{
	Q_OBJECT

public:

	ControllerManager();
	bool LoadControllers(QString const& outputConfig);

	bool BackUpControllerConfigs(QString const& folder);
	bool BackUpControllerConfig(QString const& folder, int index);

	[[nodiscard]] BaseController* GetController(int index) const { return m_controllers[index].get(); };
	[[nodiscard]] size_t GetControllerSize() const { return m_controllers.size(); };

Q_SIGNALS:
	void ReloadControllers();
	void ReloadSetFolder(QString const& folder);
	void UpdateControllerStatus(QString const& ip, QString const& status);

private:
	std::vector<std::unique_ptr<BaseController>> m_controllers;
	std::shared_ptr<spdlog::logger> m_logger{ nullptr };
};

#endif