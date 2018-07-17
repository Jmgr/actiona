/*
	Actiona
	Copyright (C) 2005 Jonathan Mercier-Ganady

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

#pragma once

#include "actiontools_global.h"
#include "subparameter.h"
#include "abstractcodeeditor.h"

#include <QLineEdit>
#include <QRegExp>

class QMenu;
class QAbstractItemModel;

namespace ActionTools
{
    class ParameterContainer;
    class CodeLineEditButton;

	class ACTIONTOOLSSHARED_EXPORT CodeLineEdit : public QLineEdit, public AbstractCodeEditor
	{
		Q_OBJECT
		Q_PROPERTY(bool code READ isCode WRITE setCode)

	public:
        CodeLineEdit(QWidget *parent, const QRegExp &regexpValidation = QRegExp());
        ~CodeLineEdit() override ;

		bool isMultiline() const										{ return mMultiline; }
		bool isCode() const												{ return mCode; }
		bool isEmbedded() const											{ return mEmbedded; }
		
        CodeLineEditButton *codeButton() const							{ return mCodeButton; }
        CodeLineEditButton *editorButton() const						{ return mEditorButton; }

		void setCode(bool code);
		void setEmbedded(bool embedded);

		void setAllowTextCodeChange(bool allowTextCodeChange);
		void setShowEditorButton(bool showEditorButton);
		void setFromSubParameter(const SubParameter &subParameter);
        void setRegexpValidation(const QRegExp &regexpValidation)       { mRegExp = regexpValidation; }

		void addShortcuts(QMenu *menu);
		
        void setCompletionModel(QAbstractItemModel *completionModel) override;
        void setParameterContainer(const ParameterContainer *parameterContainer) override;
        QSet<QString> findVariables() const override;

	public slots:
		void reverseCode();
        void onTextChanged(const QString &text);
        void openEditor(int line = -1, int column = -1) override;

	signals:
		void codeChanged(bool code);

	protected:
        void contextMenuEvent(QContextMenuEvent *event) override;
        void resizeEvent(QResizeEvent *event) override;
        virtual void insertVariable(const QString &variable);

    private slots:
        void showVariableMenuAsPopup();
        void insertVariable(QAction *action);

    protected:
        virtual QMenu *createVariablesMenu(QMenu *parentMenu, bool ignoreMultiline = false);
        virtual QMenu *createResourcesMenu(QMenu *parentMenu, bool ignoreMultiline = false);

    private:
		void resizeButtons();
        void addVariablesAndResourcesMenus(QMenu *menu);
		
        void mouseMoveEvent(QMouseEvent *event) override;
		void multilineCheck(const QString &text);

        void mouseDoubleClickEvent(QMouseEvent *event) override;
        void paintEvent(QPaintEvent *event) override;

        const ActionTools::ParameterContainer *mParameterContainer;
		bool mCode;
		bool mMultiline;
		bool mAllowTextCodeChange;
		bool mShowEditorButton;
		bool mEmbedded;
		QAction *mSwitchTextCode;
		QAction *mOpenEditor;
		QRegExp mRegExp;
		QAbstractItemModel *mCompletionModel;
        CodeLineEditButton *mCodeButton;
        CodeLineEditButton *mEditorButton;
        CodeLineEditButton *mInsertButton;

		Q_DISABLE_COPY(CodeLineEdit)
	};
}

