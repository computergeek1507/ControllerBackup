#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>

#include "Controllers/ControllerManager.h"

#include "spdlog/spdlog.h"
#include "spdlog/common.h"

#include <memory>
#include <filesystem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }

class QListWidgetItem;
class QListWidget;
class QTableWidget;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

public Q_SLOTS:

	void on_actionSetShowFolder_triggered();
	void on_actionClose_triggered();

	void on_actionAbout_triggered();
	void on_actionOpen_Logs_triggered();

	void on_pbBackupFolder_clicked();
	void on_pb_backup_clicked();

	void on_twControllers_cellDoubleClicked(int row, int column);
	void RedrawControllerList();
	void RedrawFolder(QString const& folder);
	void UpdateStatus(QString const& ip, QString const& filePath);
	void LogMessage(QString const& message , spdlog::level::level_enum llvl = spdlog::level::level_enum::debug);

private:
	Ui::MainWindow *m_ui;
	std::shared_ptr<spdlog::logger> m_logger{ nullptr };
	std::unique_ptr<QSettings> m_settings{ nullptr };
	QString m_appdir;

	std::unique_ptr<ControllerManager> m_manager{ nullptr };

};
#endif // MAINWINDOW_H
