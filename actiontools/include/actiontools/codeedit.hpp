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

	Contact: jmgr@jmgr.info
*/

#pragma once

#include "actiontools_global.hpp"

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
		explicit CodeEdit(QWidget *parent = nullptr);

		void setCode(bool code);
		bool isCode() const							{ return mCode; }
		int lineNumberAreaWidth() const;
		void setCurrentLine(int line);
		void setCurrentColumn(int column);
		void setCompletionModel(QAbstractItemModel *completionModel);

		bool checkSyntax();
		QString lastSyntaxError() const				{ return mLastSyntaxError; }

	signals:
		void acceptDialog();

	private slots:
		void insertCompletion(const QString &completion);
		void updateLineNumberAreaWidth(int newBlockCount);
		void updateLineNumberArea(const QRect &rect, int dy);

	private:
		void indentOrUnindent(bool doIndent);
		void lineNumberAreaPaintEvent(QPaintEvent *event);
		void resizeEvent(QResizeEvent *event) override;
		QString textUnderCursor() const;
		void focusInEvent(QFocusEvent *event) override;
		void autoComplete();
		void keyPressEvent(QKeyEvent *event) override;
		void keyReleaseEvent(QKeyEvent *event) override;
		bool event(QEvent *event) override;

		ScriptCompleter *mCompleter;
		CodeHighlighter *mHighlighter;
		QWidget *mLineNumberArea;
		bool mCode{true};
		QString mLastSyntaxError;
		bool mCompletionModelSet{false};

		Q_DISABLE_COPY(CodeEdit)
	};
}

