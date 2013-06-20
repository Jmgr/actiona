/*
	Actionaz
	Copyright (C) 2008-2013 Jonathan Mercier-Ganady

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

#ifndef CODELINEEDIT_H
#define CODELINEEDIT_H

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
        virtual ~CodeLineEdit();

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
		
		void setCompletionModel(QAbstractItemModel *completionModel);
        void setParameterContainer(const ParameterContainer *parameterContainer);
        QSet<QString> findVariables() const;

	public slots:
		void reverseCode();
		void textChanged(const QString &text);
		void openEditor(int line = -1, int column = -1);

	signals:
		void codeChanged(bool code);

	protected:
		void contextMenuEvent(QContextMenuEvent *event);
		void resizeEvent(QResizeEvent *event);
        virtual void insertVariable(const QString &variable);

    private slots:
        void showVariableMenuAsPopup();
        void insertVariable(QAction *action);

	private:
        QMenu *createVariablesMenu(QMenu *parentMenu);
		void resizeButtons();
		
		void mouseMoveEvent(QMouseEvent *event);
		void multilineCheck(const QString &text);

		void mouseDoubleClickEvent(QMouseEvent *event);
		void paintEvent(QPaintEvent *event);

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

#endif // CODELINEEDIT_H
