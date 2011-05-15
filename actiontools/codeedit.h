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

#ifndef CODEEDIT_H
#define CODEEDIT_H

#include "actiontools_global.h"

#include <QPlainTextEdit>

class QAbstractItemModel;

namespace ActionTools
{
	class CodeHighlighter;
	class ScriptCompleter;

	class ACTIONTOOLSSHARED_EXPORT CodeEdit : public QPlainTextEdit
	{
		Q_OBJECT

		friend class LineNumberArea;

	public:
		explicit CodeEdit(QWidget *parent = 0);

		void setCode(bool code);
		bool isCode() const							{ return mCode; }
		int lineNumberAreaWidth() const;
		void setCurrentLine(int line);
		void setCurrentColumn(int column);
		void setCompletionModel(QAbstractItemModel *completionModel);

		bool checkSyntax();
		QString lastSyntaxError() const				{ return mLastSyntaxError; }

	private slots:
		void insertCompletion(const QString &completion);
		void updateLineNumberAreaWidth(int newBlockCount);
		void highlightCurrentLine();
		void updateLineNumberArea(const QRect &rect, int dy);

	private:
		void indentOrUnindent(bool doIndent);
		void lineNumberAreaPaintEvent(QPaintEvent *event);
		void resizeEvent(QResizeEvent *event);
		QString textUnderCursor() const;
		void focusInEvent(QFocusEvent *event);
		void autoComplete();
		void keyPressEvent(QKeyEvent *event);

		ScriptCompleter *mCompleter;
		CodeHighlighter *mHighlighter;
		QWidget *mLineNumberArea;
		bool mCode;
		QString mLastSyntaxError;
		bool mCompletionModelSet;

		Q_DISABLE_COPY(CodeEdit)
	};
}

#endif // CODEEDIT_H
