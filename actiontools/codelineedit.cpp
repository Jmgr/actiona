/*
	Actiona
    Copyright (C) 2005-2016 Jonathan Mercier-Ganady

	Actiona is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Actiona is distributed in the hope that it will be useful,
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
#include "parametercontainer.h"
#include "codelineeditbutton.h"
#include "script.h"
#include "actioninstance.h"

#include <QMenu>
#include <QContextMenuEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QStyleOption>
#include <QSettings>
#include <QRegExpValidator>
#include <QDebug>
#include <QToolButton>
#include <QCursor>
#include <QSet>
#include <QMessageBox>

namespace ActionTools
{
    CodeLineEdit::CodeLineEdit(QWidget *parent, const QRegExp &regexpValidation)
        : QLineEdit(parent),
        mParameterContainer(0),
		mCode(false),
		mMultiline(false),
		mAllowTextCodeChange(true),
		mShowEditorButton(true),
		mEmbedded(false),
        mSwitchTextCode(new QAction(QIcon(":/images/code.png"), tr("Set to text/code"), this)),
        mOpenEditor(new QAction(QIcon(":/images/editor.png"), tr("Open editor"), this)),
		mRegExp(regexpValidation),
		mCompletionModel(0),
        mCodeButton(new CodeLineEditButton(this)),
        mEditorButton(new CodeLineEditButton(this)),
        mInsertButton(new CodeLineEditButton(this))
	{
		connect(this, SIGNAL(textChanged(const QString &)), this, SLOT(textChanged(const QString &)));
		connect(mSwitchTextCode, SIGNAL(triggered()), this, SLOT(reverseCode()));
		connect(mOpenEditor, SIGNAL(triggered()), this, SLOT(openEditor()));
		connect(mCodeButton, SIGNAL(clicked()), this, SLOT(reverseCode()));
		connect(mEditorButton, SIGNAL(clicked()), this, SLOT(openEditor()));
        connect(mInsertButton, SIGNAL(clicked()), this, SLOT(showVariableMenuAsPopup()));

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

        mInsertButton->setIcon(QIcon(":/images/insert.png"));
        mInsertButton->setMaximumWidth(18);
        mInsertButton->setToolTip(tr("Click here to insert a variable or a resource"));
		
        setMinimumWidth(minimumWidth() + mCodeButton->maximumWidth() + mEditorButton->maximumWidth() + mInsertButton->maximumWidth());

        setEmbedded(false);
    }

    CodeLineEdit::~CodeLineEdit()
    {
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

        w += mInsertButton->maximumWidth();
		
		setStyleSheet(QString("QLineEdit { padding-right: %1px; }").arg(w));

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

        QString t = subParameter.value().toString();

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

    void CodeLineEdit::setParameterContainer(const ParameterContainer *parameterContainer)
    {
        mParameterContainer = parameterContainer;
    }

    QSet<QString> CodeLineEdit::findVariables() const
    {
        return ActionTools::ActionInstance::findVariables(text(), isCode());
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
		
        CodeEditorDialog codeEditorDialog(mCompletionModel, createVariablesMenu(0, true), createResourcesMenu(0, true), this);

        codeEditorDialog.setWindowFlags(codeEditorDialog.windowFlags() | Qt::WindowContextHelpButtonHint);

		codeEditorDialog.setText(text());
		codeEditorDialog.setCode(isCode());
		codeEditorDialog.setCurrentLine(line);
		codeEditorDialog.setCurrentColumn(column);
		codeEditorDialog.setAllowTextCodeChange(mAllowTextCodeChange);

		if(codeEditorDialog.exec() == QDialog::Accepted)
		{
			setText(codeEditorDialog.text());
			setCode(codeEditorDialog.isCode());
        }
    }

	void CodeLineEdit::contextMenuEvent(QContextMenuEvent *event)
	{
		QMenu *menu = createStandardContextMenu();

		menu->addSeparator();

		addShortcuts(menu);

        menu->addSeparator();

        addVariablesAndResourcesMenus(menu);

		menu->exec(event->globalPos());

		delete menu;

		event->accept();
	}
	
	void CodeLineEdit::resizeEvent(QResizeEvent *event)
	{
		resizeButtons();
		
        QLineEdit::resizeEvent(event);
    }

    void CodeLineEdit::insertVariable(const QString &variable)
    {
        //If a validator is set this means that the insertion will fail
        //In this case, reset the content and set the code mode
        if(validator())
        {
            if(!text().isEmpty() && QMessageBox::question(this, tr("Insert variable/resource"), tr("Inserting a variable or a resource will replace the current parameter value.\nAre you sure?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) != QMessageBox::Yes)
                return;

            setCode(true);
            setText(QString());
        }

        //Temporarily remove the completer so that we don't get a popup
        QCompleter *currentCompleter = completer();
        if(currentCompleter)
        {
            currentCompleter->setParent(0);
            setCompleter(0);
        }

        if(isCode())
            insert(variable);
        else
            insert("$" + variable);

        if(currentCompleter)
        {
            currentCompleter->setParent(this);
            setCompleter(currentCompleter);
        }
    }

    void CodeLineEdit::insertVariable(QAction *action)
    {
        insertVariable(action->text());
    }

    QMenu *CodeLineEdit::createVariablesMenu(QMenu *parentMenu, bool ignoreMultiline)
    {
        QMenu *variablesMenu = 0;

        if(!ignoreMultiline && isMultiline())
        {
            variablesMenu = new QMenu(tr("Cannot insert in a multiline parameter"), parentMenu);
            variablesMenu->setEnabled(false);
        }
        else
        {
            Q_ASSERT(mParameterContainer);
            variablesMenu = mParameterContainer->createVariablesMenu(parentMenu);
            if(variablesMenu)
            {
                variablesMenu->setTitle(tr("Insert variable"));
            }
            else
            {
                variablesMenu = new QMenu(tr("No variables to insert"), parentMenu);
                variablesMenu->setEnabled(false);
            }
        }

        variablesMenu->setIcon(QIcon(":/images/variable.png"));

        return variablesMenu;
    }

    QMenu *CodeLineEdit::createResourcesMenu(QMenu *parentMenu, bool ignoreMultiline)
    {
        QMenu *resourceMenu = 0;

        if(!ignoreMultiline && isMultiline())
        {
            resourceMenu = new QMenu(tr("Cannot insert in a multiline parameter"), parentMenu);
            resourceMenu->setEnabled(false);
        }
        else
        {
            Q_ASSERT(mParameterContainer);
            resourceMenu = mParameterContainer->createResourcesMenu(parentMenu);
            if(resourceMenu)
                resourceMenu->setTitle(tr("Insert resource"));
            else
            {
                resourceMenu = new QMenu(tr("No resources to insert"), parentMenu);
                resourceMenu->setEnabled(false);
            }
        }

        resourceMenu->setIcon(QIcon(":/images/resource.png"));

        return resourceMenu;
    }

    void CodeLineEdit::showVariableMenuAsPopup()
    {
        QMenu *menu = new QMenu;

        addVariablesAndResourcesMenus(menu);

        menu->exec(QCursor::pos());

        delete menu;
    }
	
	void CodeLineEdit::resizeButtons()
	{
		QRect codeButtonGeometry;
        QRect editorButtonGeometry;
        QRect insertButtonGeometry;
		
		codeButtonGeometry.setX(rect().right() - mCodeButton->maximumWidth() + (mEmbedded ? 1 : 0));
		codeButtonGeometry.setY(rect().top() + (mEmbedded ? -1 : 0));
		codeButtonGeometry.setWidth(mCodeButton->maximumWidth());
		codeButtonGeometry.setHeight(height() + (mEmbedded ? 2 : 0));
		
		mCodeButton->setGeometry(codeButtonGeometry);

        insertButtonGeometry.setX(rect().right()
                                  - (mShowEditorButton ? mEditorButton->maximumWidth() : 0)
                                  - (mAllowTextCodeChange ? codeButtonGeometry.width() : 0)
                                  + (mEmbedded ? 2 : 1));
        insertButtonGeometry.setY(rect().top() + (mEmbedded ? -1 : 0));
        insertButtonGeometry.setWidth(mInsertButton->maximumWidth());
        insertButtonGeometry.setHeight(height() + (mEmbedded ? 2 : 0));

        mInsertButton->setGeometry(insertButtonGeometry);

        editorButtonGeometry.setX(rect().right()
                                  - (mShowEditorButton ? mEditorButton->maximumWidth() : 0)
                                  - (mAllowTextCodeChange ? codeButtonGeometry.width() : 0)
                                  - insertButtonGeometry.width()
                                  + (mEmbedded ? 2 : 1));
        editorButtonGeometry.setY(rect().top() + (mEmbedded ? -1 : 0));
        editorButtonGeometry.setWidth(mEditorButton->maximumWidth());
        editorButtonGeometry.setHeight(height() + (mEmbedded ? 2 : 0));

        mEditorButton->setGeometry(editorButtonGeometry);
    }

    void CodeLineEdit::addVariablesAndResourcesMenus(QMenu *menu)
    {
        QMenu *variablesMenu = createVariablesMenu(menu);
        connect(variablesMenu, SIGNAL(triggered(QAction*)), this, SLOT(insertVariable(QAction*)));
        menu->addMenu(variablesMenu);

        QMenu *resourcesMenu = createResourcesMenu(menu);
        connect(resourcesMenu, SIGNAL(triggered(QAction*)), this, SLOT(insertVariable(QAction*)));
        menu->addMenu(resourcesMenu);
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

                style()->drawItemText(&painter, rect(), Qt::AlignCenter, pal, false, tr("Multiline, double-click to edit"), QPalette::Text);
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
