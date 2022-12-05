#ifndef BACKUPVIEWER_H
#define BACKUPVIEWER_H

#include <QDialog>

#include <ui_backupviewer.h>

struct BaseController;

class BackUpViewer : public QDialog
{
public:
    static bool Load(BaseController * controller, QWidget* parent);

protected:
	BackUpViewer(BaseController * controller, QWidget* parent );

public Q_SLOTS:

private:
	Ui::BackUpViewerDialog m_ui;
};

#endif 