#include "backup_viewer.h"

#include "../Controllers/BaseController.h"
#include "../Controllers/config_visitor.h"
#include "../Controllers/controller_data.h"


enum class StringPortColumn : int { Port = 0, Desp, Pixels, StartChan, Protocol, ColorOrder, StartNulls, EndNulls, Brightness, Gamma, Group, Reverse};

bool BackUpViewer::Load(BaseController * controller, QWidget* parent)
{
    BackUpViewer bc(controller, parent);

	if (bc.exec())
	{
		return true;
	}
	return false;
}

BackUpViewer::BackUpViewer(BaseController * controller, QWidget* parent ) :
    QDialog( parent )
{
    m_ui.setupUi( this );
	connect(m_ui.buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

	m_ui.splitter->setStretchFactor(0, 1);
	m_ui.splitter->setStretchFactor(1, 3);
	//m_ui.twStrings->hideColumn(std::to_underlying(StringPortColumn::EndChan));

	std::unique_ptr<ConfigVisitor> visitor = std::make_unique< ConfigVisitor>();

	if (!controller->GetFileName().isEmpty())
	{
		controller->accept(visitor.get());
		if (visitor->controllers.size() > 0) {
			auto cd = visitor->controllers.front();
			LoadData(cd);
		}
	}
}

void BackUpViewer::LoadData(ControllerData const& controller)
{
	m_ui.lsInfo->clear();
	m_ui.lsInfo->addItem("Name: " + controller.name);
	m_ui.lsInfo->addItem("IP: " + controller.ip);
	m_ui.lsInfo->addItem("Mode: " + controller.mode);
	m_ui.lsInfo->addItem("Firmware: " + controller.firmware);

	m_ui.lsInfo->addItem("Inputs" );
	for (auto const& in : controller.inputs)
	{
		m_ui.lsInfo->addItem(in.toString());
	}

	m_ui.twStrings->clearContents();
	m_ui.twStrings->setRowCount(0);
	auto SetItem = [&](int row, StringPortColumn col, QString const& text)
	{
		m_ui.twStrings->setItem(row, std::to_underlying(col), new QTableWidgetItem());
		m_ui.twStrings->item(row, std::to_underlying(col))->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		m_ui.twStrings->item(row, std::to_underlying(col))->setText(text);
	};

	auto SetItemInt = [&](int row, StringPortColumn col, int val)
	{
		SetItem(row, col, QString::number(val));
	};
	auto SetItemDouble = [&](int row, StringPortColumn col, double val)
	{
		SetItem(row, col, QString::number(val, 'f', 2));
	};

	auto SetItemStartChannel = [&](int row, StringPortColumn col, uint64_t startUniverse, uint64_t startChan)
	{
		if (startUniverse == 0) {
			SetItem(row, col, QString::number(startChan));
		}
		else {
			SetItem(row, col, QString("#%1:%2").arg(startUniverse).arg(startChan));
		}
	};

	m_ui.twStrings->setRowCount(static_cast<int>(controller.pixelports.size()));

	int idx{ 0 };
	for (auto const& c : controller.pixelports)
	{
		SetItemInt(idx, StringPortColumn::Port, c.port);
		SetItem(idx, StringPortColumn::Desp, c.name );
		SetItemInt(idx, StringPortColumn::Pixels, c.pixels);
		SetItemStartChannel(idx, StringPortColumn::StartChan, c.startUniverse, c.startChannel);
		SetItem(idx, StringPortColumn::Protocol, c.protocol);
		SetItem(idx, StringPortColumn::ColorOrder, c.colorOrder);
		SetItemInt(idx, StringPortColumn::StartNulls, c.startNulls);
		SetItemInt(idx, StringPortColumn::EndNulls, c.endNulls);
		SetItemInt(idx, StringPortColumn::Brightness, c.brightness);
		SetItemDouble(idx, StringPortColumn::Gamma, c.gamma);
		SetItemInt(idx, StringPortColumn::Group, c.group);
		SetItem(idx, StringPortColumn::Reverse, c.reverse ? "true" : "false");
		++idx;
	}

	m_ui.twStrings->resizeColumnsToContents();
}
