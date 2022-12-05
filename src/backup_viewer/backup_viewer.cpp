#include "backup_viewer.h"

#include "../controllers/BaseController.h"
#include "viewer_visitor.h"

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
	std::unique_ptr<ViewerVisitor> visitor = std::make_unique< ViewerVisitor>();

	controller->accept(visitor.get());
	m_ui.textEdit->setPlainText(visitor->fileData);
}
