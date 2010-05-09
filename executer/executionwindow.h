#ifndef EXECUTIONWINDOW_H
#define EXECUTIONWINDOW_H

#include <QWidget>
#include <QKeySequence>

namespace Ui
{
	class ExecutionWindow;
}

class ExecutionWindow : public QWidget
{
	Q_OBJECT

public:
	ExecutionWindow(QWidget *parent = 0);
	~ExecutionWindow();

	void setStopExecutionShortcut(const QKeySequence &keySequence);
	void setCurrentActionName(const QString &actionName);
	void setCurrentActionColor(const QColor &actionColor);

signals:
	void canceled();

private:
	void changeEvent(QEvent *e);

	Ui::ExecutionWindow *ui;
};

#endif // EXECUTIONWINDOW_H
