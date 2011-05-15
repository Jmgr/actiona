/*
	Actionaz
	Copyright (C) 2008-2011 Jonathan Mercier-Ganady

	Actionaz is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Actionaz is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.

	Contact : jmgr@jmgr.info
*/

#include "codelineedit.h"
#include "codeeditordialog.h"
#include "settings.h"
#include "scriptcompleter.h"

#include <QMenu>
#include <QContextMenuEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QStyleOption>
#include <QSettings>
#include <QRegExpValidator>
#include <QDebug>
#include <QToolButton>

namespace ActionTools
{
	CodeLineEdit::CodeLineEdit(QWidget *parent, const QRegExp &regexpValidation)
		: QLineEdit(parent),
		mCode(false),
		mMultiline(false),
		mAllowTextCodeChange(true),
		mShowEditorButton(true),
		mEmbedded(false),
		mSwitchTextCode(new QAction(tr("Set to text/code"), this)),
		mOpenEditor(new QAction(tr("Open editor"), this)),
		mRegExp(regexpValidation),
		mCompletionModel(0),
		mCodeButton(new QToolButton(this)),
		mEditorButton(new QToolButton(this))
	{
		connect(this, SIGNAL(textChanged(const QString &)), this, SLOT(textChanged(const QString &)));
		connect(mSwitchTextCode, SIGNAL(triggered()), this, SLOT(reverseCode()));
		connect(mOpenEditor, SIGNAL(triggered()), this, SLOT(openEditor()));
		connect(mCodeButton, SIGNAL(clicked()), this, SLOT(reverseCode()));
		connect(mEditorButton, SIGNAL(clicked()), this, SLOT(openEditor()));

		QSettings settings;

		mSwitchTextCode->setShortcut(QKeySequence(settings.value("actions/switchTextCode", QKeySequence("Ctrl+Shift+C")).toString()));
		mSwitchTextCode->setShortcutContext(Qt::WidgetShortcut);
		mOpenEditor->setShortcut(QKeySequence(settings.value("actions/openEditorKey", QKeySequence("Ctrl+Shift+V")).toString()));
		mOpenEditor->setShortcutContext(Qt::WidgetShortcut);

		addAction(mSwitchTextCode);
		addAction(mOpenEditor);
		
		mCodeButton->setIcon(QIcon(":/images/code.png"));
		mCodeButton->setMaximumWidth(14);
		mCodeButton->setToolTip(tr("Click here to switch text/code"));
		
		mEditorButton->setIcon(QIcon(":/images/editor.png"));
		mEditorButton->setMaximumWidth(18);
		mEditorButton->setToolTip(tr("Click here to open the editor"));
		
		setMinimumWidth(minimumWidth() + 14 + 18);
		
		setEmbedded(false);
	}
	
	void CodeLineEdit::setCode(bool code)
	{
		if(!mAllowTextCodeChange)
		   return;

		mCode = code;
		
		if(code && mCompletionModel)
			setCompleter(new ScriptCompleter(mCompletionModel, this));
		else
			setCompleter(0);

		if(mRegExp != QRegExp())
		{
			if(code)
			{
				delete validator();
				setValidator(0);
			}
			else
				setValidator(new QRegExpValidator(mRegExp, this));
		}
		
		update();
		
		emit codeChanged(mCode);
	}
	
	void CodeLineEdit::setEmbedded(bool embedded)
	{
		mEmbedded = embedded;
		
		int w = 0;
		
		if(mAllowTextCodeChange)
			w += mCodeButton->maximumWidth();
		if(mShowEditorButton)
			w += mEditorButton->maximumWidth();
		
		if(embedded)
			setStyleSheet(QString("QLineEdit { padding-right: %1px; }").arg(w));
		else
			setStyleSheet(QString("QLineEdit { padding-right: %1px; min-height: %2px; }").arg(w).arg(sizeHint().height()));

		resizeButtons();
		update();
	}

	void CodeLineEdit::setAllowTextCodeChange(bool allowTextCodeChange)
	{
		mAllowTextCodeChange = allowTextCodeChange;
		mSwitchTextCode->setEnabled(mAllowTextCodeChange);
		
		mCodeButton->setVisible(allowTextCodeChange);
		
		setEmbedded(mEmbedded);
		
		resizeButtons();
		update();
	}
	
	void CodeLineEdit::setShowEditorButton(bool showEditorButton)
	{
		mShowEditorButton = showEditorButton;
		mOpenEditor->setEnabled(mShowEditorButton);
		
		mEditorButton->setVisible(showEditorButton);
		
		setEmbedded(mEmbedded);
		
		resizeButtons();
		update();
	}

	void CodeLineEdit::setFromSubParameter(const SubParameter &subParameter)
	{
		if(mAllowTextCodeChange)
			setCode(subParameter.isCode());

		setText(subParameter.value().toString());
	}

	void CodeLineEdit::addShortcuts(QMenu *menu)
	{
		menu->addActions(actions());
	}
	
	void CodeLineEdit::setCompletionModel(QAbstractItemModel *completionModel)
	{
		mCompletionModel = completionModel;
		
		if(mCode)
			setCompleter(new ScriptCompleter(mCompletionModel, this));
	}

	void CodeLineEdit::reverseCode()
	{
		setCode(!isCode());
	}

	void CodeLineEdit::textChanged(const QString &text)
	{
		mMultiline = text.contains(QChar('\n'));
		setReadOnly(mMultiline);
	}

	void CodeLineEdit::openEditor(int line, int column)
	{
		if(!mShowEditorButton)
			return;
		
		CodeEditorDialog *codeEditorDialog = new CodeEditorDialog(mCompletionModel, this);

		codeEditorDialog->setText(text());
		codeEditorDialog->setCode(isCode());
		codeEditorDialog->setCurrentLine(line);
		codeEditorDialog->setCurrentColumn(column);
		codeEditorDialog->setAllowTextCodeChange(mAllowTextCodeChange);

		if(codeEditorDialog->exec() == QDialog::Accepted)
		{
			setText(codeEditorDialog->text());
			setCode(codeEditorDialog->isCode());
		}

		delete codeEditorDialog;
	}

	void CodeLineEdit::contextMenuEvent(QContextMenuEvent *event)
	{
		QMenu *menu = createStandardContextMenu();

		menu->addSeparator();

		addShortcuts(menu);

		menu->exec(event->globalPos());

		delete menu;

		event->accept();
	}
	
	void CodeLineEdit::resizeEvent(QResizeEvent *event)
	{
		resizeButtons();
		
		QLineEdit::resizeEvent(event);
	}
	
	void CodeLineEdit::resizeButtons()
	{
		QRect codeButtonGeometry;
		QRect editorButtonGeometry;
		
		codeButtonGeometry.setX(rect().right() - mCodeButton->maximumWidth() + (mEmbedded ? 1 : 0));
		codeButtonGeometry.setY(rect().top() + (mEmbedded ? -1 : 0));
		codeButtonGeometry.setWidth(mCodeButton->maximumWidth());
		codeButtonGeometry.setHeight(height() + (mEmbedded ? 2 : 0));
		
		mCodeButton->setGeometry(codeButtonGeometry);
		
		editorButtonGeometry.setX(rect().right()
								  - (mShowEditorButton ? mEditorButton->maximumWidth() : 0)
								  - (mAllowTextCodeChange ? codeButtonGeometry.width() : 0)
								  + (mEmbedded ? 2 : 1));
		editorButtonGeometry.setY(rect().top() + (mEmbedded ? -1 : 0));
		editorButtonGeometry.setWidth(mEditorButton->maximumWidth());
		editorButtonGeometry.setHeight(height() + (mEmbedded ? 2 : 0));
		
		mEditorButton->setGeometry(editorButtonGeometry);
	}

	void CodeLineEdit::mouseMoveEvent(QMouseEvent *event)
	{
		if(!mMultiline)
			QLineEdit::mouseMoveEvent(event);
		else
			event->ignore();
	}

	void CodeLineEdit::mouseDoubleClickEvent(QMouseEvent *event)
	{
		if(mMultiline)
			emit openEditor();
			
		QLineEdit::mouseDoubleClickEvent(event);
	}

	void CodeLineEdit::paintEvent(QPaintEvent *event)
	{
		if(!mMultiline)
			QLineEdit::paintEvent(event);

		if(mMultiline || mCode)
		{
			QPainter painter(this);

			if(mMultiline)
			{
				QStyleOptionFrameV3 panel;
				panel.initFrom(this);
				
				if(!mEmbedded)
				{
					panel.lineWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth, &panel, this);
					panel.midLineWidth = 0;
					panel.state |= QStyle::State_Sunken;
	
					style()->drawPrimitive(QStyle::PE_PanelLineEdit, &panel, &painter, this);
				}

				painter.setBrush(panel.palette.text());
				QFont italicFont = font();
				italicFont.setStyle(QFont::StyleItalic);
				painter.setFont(italicFont);

				QPalette pal = palette();
				pal.setCurrentColorGroup(QPalette::Disabled);

				style()->drawItemText(&painter, rect(), Qt::AlignCenter, pal, false, tr("Double-click to edit"), QPalette::Text);
			}

			if(mCode)
			{
				QPolygon polygon;
				QColor color;
		
				if(isEnabled())
					color = QColor(255, 0, 0, 200);
				else
					color = QColor(100, 0, 0, 200);
		
				painter.setPen(Qt::NoPen);
				
				int offset = (mEmbedded ? 0 : 4);
		
				polygon << QPoint(offset, offset)
						<< QPoint(6 + offset, offset)
						<< QPoint(offset, 6 + offset);
		
				painter.setBrush(QBrush(color));
				painter.drawPolygon(polygon);
			}
		}
	}
}
