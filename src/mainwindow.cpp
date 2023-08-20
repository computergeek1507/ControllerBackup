#include "mainwindow.h"

#include "./ui_mainwindow.h"

#include "config.h"

#include <QMessageBox>
#include <QDesktopServices>
#include <QFileDialog>
#include <QTextStream>
#include <QListWidget>
#include <QListWidgetItem>
#include <QTableWidget>
#include <QThread>
#include <QInputDialog>
#include <QCommandLineParser>
#include <QTimer>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QStandardPaths>
#include <QOperatingSystemVersion>

#include "backup_viewer/backup_viewer.h"

#include "spdlog/spdlog.h"

#include "spdlog/sinks/qt_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"

#include <filesystem>
#include <utility>

enum class ControllerColumn: int { Name=0, IP_Address, Type, Status };

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
{
	QCoreApplication::setApplicationName(PROJECT_NAME);
    QCoreApplication::setApplicationVersion(PROJECT_VER);
    m_ui->setupUi(this);

	auto const log_name{ "log.txt" };

	m_appdir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
	std::filesystem::create_directory(m_appdir.toStdString());
	QString logdir = m_appdir + "/log/";
	std::filesystem::create_directory(logdir.toStdString());

	try
	{
		auto file{ std::string(logdir.toStdString() + log_name) };
		auto rotating = std::make_shared<spdlog::sinks::rotating_file_sink_mt>( file, 1024 * 1024, 5, false);

		m_logger = std::make_shared<spdlog::logger>("ControllerBackup", rotating);
		m_logger->flush_on(spdlog::level::debug);
		m_logger->set_level(spdlog::level::debug);
		m_logger->set_pattern("[%D %H:%M:%S] [%L] %v");
		spdlog::register_logger(m_logger);
	}
	catch (std::exception& /*ex*/)
	{
		QMessageBox::warning(this, "Logger Failed", "Logger Failed To Start.");
	}

	setWindowTitle(windowTitle() + " v" + PROJECT_VER);

	m_settings = std::make_unique< QSettings>(m_appdir + "/settings.ini", QSettings::IniFormat);

	m_manager = std::make_unique<ControllerManager>();
	connect(m_manager.get(), &ControllerManager::ReloadControllers, this, &MainWindow::RedrawControllerList);
	connect(m_manager.get(), &ControllerManager::ReloadSetFolder, this, &MainWindow::RedrawFolder);
	connect(m_manager.get(), &ControllerManager::UpdateControllerStatus, this, &MainWindow::UpdateStatus);

	m_ui->twControllers->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(m_ui->twControllers, &QTableWidget::customContextMenuRequested,	this, &MainWindow::customMenuRequested);

	auto lastfolder{ m_settings->value("last_folder").toString() };
	auto backupfolder{ m_settings->value("backup_folder").toString() };

	if (!lastfolder.isEmpty() && QDir(lastfolder).exists())
	{
		m_manager->LoadControllers(lastfolder);
	}

	if (!backupfolder.isEmpty() && QDir(backupfolder).exists())
	{
		m_ui->leBackupFolder->setText(backupfolder);
	}
}

MainWindow::~MainWindow()
{
	delete m_ui;
}

void MainWindow::on_actionSetShowFolder_triggered()
{
	QFileDialog dialog;
	dialog.setFileMode(QFileDialog::Directory);
	dialog.setOption(QFileDialog::ShowDirsOnly);
	auto lastfolder{ m_settings->value("last_folder",QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)).toString() };

	QString const folder = dialog.getExistingDirectory(this, "Select xLight Show Folder", lastfolder, QFileDialog::ShowDirsOnly);
	if (!folder.isEmpty() && QDir(folder).exists())
	{
		m_manager->LoadControllers(folder);
		m_settings->setValue("last_folder", folder);
		m_settings->sync();
	}
}

void MainWindow::on_actionSetup_xLights_from_Configs_triggered()
{
	auto folder = m_ui->leBackupFolder->text();
	if (!folder.isEmpty() && QDir(folder).exists())
	{
		m_manager->UpdateXLightsController(folder, m_ui->twControllers->currentRow());
	}
	else
	{
		QMessageBox::warning(this, "Backup Folder Doesn't Exist", "Backup Folder Doesn't Exist: '" + folder + "'");
	}
}

void MainWindow::on_actionClose_triggered()
{
	close();
}

void MainWindow::on_actionAbout_triggered()
{
	QString text = QString("Controller Backup v%1<br>QT v%2<br><br>Icons by:")
		.arg(PROJECT_VER, QT_VERSION_STR) +
		QStringLiteral("<br><a href='http://www.famfamfam.com/lab/icons/silk/'>www.famfamfam.com</a>");
		//http://www.famfamfam.com/lab/icons/silk/
	QMessageBox::about( this, "About Controller Backup", text );
}

void MainWindow::on_actionOpen_Logs_triggered()
{
	QDesktopServices::openUrl(QUrl::fromLocalFile(m_appdir + "/log/"));
}

void MainWindow::on_twControllers_cellDoubleClicked(int row, int column)
{
	if (column == std::to_underlying(ControllerColumn::Name))
	{
		BackUpViewer::Load(m_manager->GetController(row), this);
	}
	else if (column == std::to_underlying(ControllerColumn::IP_Address))
	{
		auto ip{ m_ui->twControllers->item(row, std::to_underlying(ControllerColumn::IP_Address))->text() };
		QDesktopServices::openUrl(QUrl("http://" + ip));
	}
	else if (column == std::to_underlying(ControllerColumn::Status))
	{
		auto file = m_ui->twControllers->item(row, std::to_underlying(ControllerColumn::Status))->data(Qt::UserRole).toString();
		if (QFile::exists(file)) 
		{
			QDesktopServices::openUrl(QUrl::fromLocalFile(file));
		}
	}
}

void MainWindow::RedrawControllerList()
{
	m_ui->twControllers->clearContents();
	m_ui->twControllers->setRowCount(0);
	auto SetItem = [&](int row, ControllerColumn col, QString const& text)
	{
		m_ui->twControllers->setItem(row, std::to_underlying(col), new QTableWidgetItem());
		m_ui->twControllers->item(row, std::to_underlying(col))->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		m_ui->twControllers->item(row, std::to_underlying(col))->setText(text);
	};

	m_ui->twControllers->setRowCount(static_cast<int>(m_manager->GetControllerSize()));

	for (size_t idx=0; idx < m_manager->GetControllerSize(); ++idx)
	{
		auto c = m_manager->GetController(idx);
		SetItem(idx, ControllerColumn::Name, c->Name);
		SetItem(idx, ControllerColumn::IP_Address, c->IP);
		SetItem(idx, ControllerColumn::Type, c->GetType());
		SetItem(idx, ControllerColumn::Status, QString());
	}
	
	m_ui->twControllers->resizeColumnsToContents();
}

void MainWindow::on_pbBackupFolder_clicked()
{
	QFileDialog dialog;
	dialog.setFileMode(QFileDialog::Directory);
	dialog.setOption(QFileDialog::ShowDirsOnly);
	auto backupfolder{ m_settings->value("backup_folder",QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)).toString() };

	QString const folder = dialog.getExistingDirectory(this, "Select Backup Folder", backupfolder, QFileDialog::ShowDirsOnly);
	if (!folder.isEmpty() && QDir(folder).exists())
	{
		m_ui->leBackupFolder->setText(folder);
		m_settings->setValue("backup_folder", folder);
		m_settings->sync();
	}
}

void MainWindow::on_pb_backup_clicked()
{
	auto folder = m_ui->leBackupFolder->text();
	if (!folder.isEmpty() && QDir(folder).exists())
	{
		m_manager->BackUpControllerConfigs(folder);
	}
	else
	{
		QMessageBox::warning(this, "Backup Folder Doesn't Exist", "Backup Folder Doesn't Exist: '" + folder + "'");
	}
}

void MainWindow::customMenuRequested(QPoint pos)
{
	QModelIndex index = m_ui->twControllers->indexAt(pos);
	QMenu menu(this);

	QAction* action1 = menu.addAction("Backup Controller");

	QAction* selectedMenuItem = menu.exec(m_ui->twControllers->mapToGlobal(pos));
	if (selectedMenuItem == nullptr)
	{
		return;
	}

	if (selectedMenuItem == action1)
	{
		auto folder = m_ui->leBackupFolder->text();
		if (!folder.isEmpty() && QDir(folder).exists())
		{
			m_manager->BackUpControllerConfig(folder, m_ui->twControllers->currentRow());
		}
		else
		{
			QMessageBox::warning(this, "Backup Folder Doesn't Exist", "Backup Folder Doesn't Exist: '" + folder + "'");
		}
	}
}

void MainWindow::LogMessage(QString const& message, spdlog::level::level_enum llvl)
{
	m_logger->log(llvl, message.toStdString());
}

void MainWindow::RedrawFolder(QString const& folder)
{
	m_ui->lblShowFolder->setText(folder);
}

void MainWindow::UpdateStatus(QString const& ip, QString const& filePath)
{
	m_ui->twControllers->setRowCount(static_cast<int>(m_manager->GetControllerSize()));

	for (size_t idx = 0; idx < m_ui->twControllers->rowCount(); ++idx)
	{
		auto cellip = m_ui->twControllers->item(idx, std::to_underlying(ControllerColumn::IP_Address))->text();
		if(cellip == ip)
		{
			QFileInfo info(filePath);
			m_ui->twControllers->item(idx, std::to_underlying(ControllerColumn::Status))->setText("Saved: " + info.fileName());
			m_ui->twControllers->item(idx, std::to_underlying(ControllerColumn::Status))->setData(Qt::UserRole, filePath);
			m_ui->twControllers->resizeColumnsToContents();
			return;
		}
	}
}
