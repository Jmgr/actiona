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

#include "codeedit.h"
#include "codehighlighter.h"
#include "linenumberarea.h"
#include "scriptcompleter.h"
#include "keywords.h"
#include "code/codetools.h"

#include <QAbstractItemView>
#include <QScrollBar>
#include <QApplication>
#include <QPainter>
#include <QScriptEngine>
#include <QDebug>
#include <QStandardItemModel>

namespace ActionTools
{
	CodeEdit::CodeEdit(QWidget *parent)
		: QPlainTextEdit(parent),
		mCompleter(new ScriptCompleter(this)),
		mHighlighter(new CodeHighlighter(document())),
		mLineNumberArea(new LineNumberArea(this))
		
	{
		mCompleter->setWidget(this);
		mCompleter->setCompletionMode(QCompleter::PopupCompletion);
		mCompleter->setCaseSensitivity(Qt::CaseSensitive);

		mCompleter->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
		mCompleter->setWrapAround(false);

        connect(this, &CodeEdit::blockCountChanged, this, &CodeEdit::updateLineNumberAreaWidth);
        connect(this, &CodeEdit::updateRequest, this, &CodeEdit::updateLineNumberArea);
        connect(this, &CodeEdit::cursorPositionChanged, this, &CodeEdit::highlightCurrentLine);

		updateLineNumberAreaWidth(0);
		highlightCurrentLine();
		setTabStopWidth(30);

		QFont font;
		font.setFamily(QStringLiteral("Arial"));
		font.setFixedPitch(true);
		font.setPointSize(10);
		setFont(font);

        connect(mCompleter, static_cast<void (ScriptCompleter::*)(const QString &)>(&ScriptCompleter::activated), this, &CodeEdit::insertCompletion);
	}

	void CodeEdit::setCode(bool code)
	{
		mCode = code;

		mHighlighter->setDocument(code ? document() : nullptr);
	}

	int CodeEdit::lineNumberAreaWidth() const
	{
		int digits = 1;
		int max = qMax(1, blockCount());
		while(max >= 10)
		{
			max /= 10;
			++digits;
		}

		int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

		return space;
	}

	void CodeEdit::setCurrentLine(int line)
	{
		QTextCursor cursor = textCursor();
		cursor.movePosition(QTextCursor::Start);
		cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, line - 1);
		setTextCursor(cursor);
	}

	void CodeEdit::setCurrentColumn(int column)
	{
		QTextCursor cursor = textCursor();
		cursor.movePosition(QTextCursor::StartOfLine);
		cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, column - 1);
		setTextCursor(cursor);
	}
	
	void CodeEdit::setCompletionModel(QAbstractItemModel *completionModel)
	{
		mCompleter->setModel(completionModel);
		
		if(!mCompletionModelSet)
		{
			mCompletionModelSet = true;
			
			auto standardItemCompletionModel = qobject_cast<QStandardItemModel *>(completionModel);
			if(!standardItemCompletionModel)
				return;
			
			for(int actionIndex = 0; actionIndex < standardItemCompletionModel->rowCount(); ++actionIndex)
			{
				QStandardItem *item = standardItemCompletionModel->item(actionIndex, 0);
				if(!item)
					continue;

				mHighlighter->addCodeObject(item->text());
			}
		}
	}

	bool CodeEdit::checkSyntax()
	{
		if(mCode)
		{
			QScriptSyntaxCheckResult result = QScriptEngine::checkSyntax(toPlainText());
			if(result.state() != QScriptSyntaxCheckResult::Valid)
			{
				setCurrentLine(result.errorLineNumber());
				setCurrentColumn(result.errorColumnNumber());

				mLastSyntaxError = result.errorMessage();

				return false;
			}
		}

		return true;
	}

	void CodeEdit::insertCompletion(const QString &completion)
	{
		if(mCompleter->widget() != this)
			return;

		QTextCursor cursor = textCursor();
		int extra = completion.length() - mCompleter->completionPrefix().length();
		cursor.insertText(completion.right(extra));
		setTextCursor(cursor);
	}

	void CodeEdit::updateLineNumberAreaWidth(int newBlockCount)
	{
		Q_UNUSED(newBlockCount)

		setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
	}

	void CodeEdit::highlightCurrentLine()
	{
		QList<QTextEdit::ExtraSelection> extraSelections;

		if(!isReadOnly())
		{
			QTextEdit::ExtraSelection selection;

			QColor lineColor = QColor(Qt::yellow).lighter(160);

			selection.format.setBackground(lineColor);
			selection.format.setProperty(QTextFormat::FullWidthSelection, true);
			selection.cursor = textCursor();
			selection.cursor.clearSelection();
			extraSelections.append(selection);
		}

		setExtraSelections(extraSelections);
	}

	void CodeEdit::updateLineNumberArea(const QRect &rect, int dy)
	{
		if(dy)
			mLineNumberArea->scroll(0, dy);
		else
			mLineNumberArea->update(0, rect.y(), mLineNumberArea->width(), rect.height());

		if(rect.contains(viewport()->rect()))
			updateLineNumberAreaWidth(0);
	}

	int spacesLeftFromPosition(const QString &text, int position)
	{
		int i = position;

		while(i > 0)
		{
			if(!text.at(i-1).isSpace())
				break;
			--i;
		}

		return position - i;
	}

	int columnAt(const QString &text, int position)
	{
		int column = 0;

		for(int i = 0; i < position; ++i)
		{
			if(text.at(i) == QLatin1Char('\t'))
				column = column - (column % 30) + 30;
			else
				++column;
		}

		return column;
	}

	int indentedColumn(int column, bool doIndent)
	{
		int aligned = (column / 30) * 30;

		if(doIndent)
			return aligned + 30;
		if(aligned < column)
			return aligned;

		return qMax(0, aligned - 30);
	}

	QString indentationString(int startColumn, int targetColumn)
	{
		targetColumn = qMax(startColumn, targetColumn);

		QString s;
		int alignedStart = startColumn - (startColumn % 30) + 30;

		if(alignedStart > startColumn && alignedStart <= targetColumn)
		{
			s += QLatin1Char('\t');
			startColumn = alignedStart;
		}
		if(int columns = targetColumn - startColumn)
		{
			int tabs = columns / 30;
			s += QString(tabs, QLatin1Char('\t'));
			s += QString(columns - tabs * 30, QLatin1Char(' '));
		}

		return s;
	}

	int lineIndentPosition(const QString &text)
	{
		int i = 0;

		while(i < text.size())
		{
			if(!text.at(i).isSpace())
				break;
			++i;
		}

		int column = columnAt(text, i);

		return i - (column % 30);
	}

	int firstNonSpace(const QString &text)
	{
		int i = 0;

		while(i < text.size())
		{
			if(!text.at(i).isSpace())
				return i;
			++i;
		}

		return i;
	}

	void CodeEdit::indentOrUnindent(bool doIndent)
	{
		QTextCursor cursor = textCursor();

		cursor.beginEditBlock();

		int pos = cursor.position();
		QTextDocument *doc = document();

		if(!cursor.hasSelection() || (doc->findBlock(cursor.selectionStart()) == doc->findBlock(cursor.selectionEnd()) ))
		{
			cursor.removeSelectedText();
			QTextBlock block = cursor.block();
			QString text = block.text();
			int indentPosition = (cursor.position() - block.position());
			int spaces = spacesLeftFromPosition(text, indentPosition);
			int startColumn = columnAt(text, indentPosition - spaces);
			int targetColumn = indentedColumn(columnAt(text, indentPosition), doIndent);

			cursor.setPosition(block.position() + indentPosition);
			cursor.setPosition(block.position() + indentPosition - spaces, QTextCursor::KeepAnchor);
			cursor.removeSelectedText();
			cursor.insertText(indentationString(startColumn, targetColumn));
		}
		else
		{
			int anchor = cursor.anchor();
			int start = qMin(anchor, pos);
			int end = qMax(anchor, pos);

			QTextBlock startBlock = doc->findBlock(start);
			QTextBlock endBlock = doc->findBlock(end-1).next();

			for(QTextBlock block = startBlock; block != endBlock; block = block.next())
			{
				QString text = block.text();
				int indentPosition = lineIndentPosition(text);
				if(!doIndent && !indentPosition)
				indentPosition = firstNonSpace(text);
				int targetColumn = indentedColumn(columnAt(text, indentPosition), doIndent);
				cursor.setPosition(block.position() + indentPosition);
				cursor.insertText(indentationString(0, targetColumn));
				cursor.setPosition(block.position());
				cursor.setPosition(block.position() + indentPosition, QTextCursor::KeepAnchor);
				cursor.removeSelectedText();
			}
		}

		cursor.endEditBlock();
	}

	void CodeEdit::lineNumberAreaPaintEvent(QPaintEvent *event)
	{
		QPainter painter(mLineNumberArea);
		painter.fillRect(event->rect(), Qt::lightGray);
		QTextBlock block = firstVisibleBlock();
		int blockNumber = block.blockNumber();
		int top = static_cast<int>(blockBoundingGeometry(block).translated(contentOffset()).top());
		int bottom = top + static_cast<int>(blockBoundingRect(block).height());
		while (block.isValid() && top <= event->rect().bottom())
		{
			if (block.isVisible() && bottom >= event->rect().top())
			{
				QString number = QString::number(blockNumber + 1);
				painter.setPen(Qt::black);
				painter.drawText(0, top, mLineNumberArea->width(), fontMetrics().height(), Qt::AlignRight, number);
			}

			block = block.next();
			top = bottom;
			bottom = top + static_cast<int>(blockBoundingRect(block).height());
			++blockNumber;
		}
	}

	void CodeEdit::resizeEvent(QResizeEvent *e)
	{
		QPlainTextEdit::resizeEvent(e);

		QRect cr = contentsRect();
		mLineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
	}

	QString CodeEdit::textUnderCursor() const
	{
		QTextCursor cursor = textCursor();
	
		int curpos = cursor.position();
		QString text = cursor.block().text().left(curpos);
	
		QStringList wordList = text.split(QRegExp(QStringLiteral("[^\\w\\.]")));
	
		if (wordList.isEmpty())
			return QString();
	
		return wordList.last();
	}

	void CodeEdit::autoComplete()
	{
		mCompleter->complete();
	}

	void CodeEdit::focusInEvent(QFocusEvent *event)
	{
		if(mCompleter)
			mCompleter->setWidget(this);

		QPlainTextEdit::focusInEvent(event);
	}

	void CodeEdit::keyPressEvent(QKeyEvent *event)
	{
		if(!mCode)
		{
			QPlainTextEdit::keyPressEvent(event);

			return;
		}

		if(mCompleter->popup()->isVisible())
		{
			// The following keys are forwarded by the completer to the widget
			switch(event->key())
			{
			 case Qt::Key_Enter:
			 case Qt::Key_Return:
			 case Qt::Key_Escape:
			 case Qt::Key_Tab:
			 case Qt::Key_Backtab:
				event->ignore();
				return; // let the completer do default behavior
			 default:
				break;
			}
		}

		bool isShortcut = ((event->modifiers() & Qt::ControlModifier) && event->key() == Qt::Key_Space); // CTRL+Space
		if(!mCompleter || !isShortcut) // dont process the shortcut when we have a completer
		{
			switch(event->key())
			{
			case Qt::Key_Tab:
				indentOrUnindent(true);
				return;
			case Qt::Key_Backtab:
				indentOrUnindent(false);
				return;
			default:
				break;
			}

			QPlainTextEdit::keyPressEvent(event);
		}

		const bool ctrlOrShift = event->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);

		if(!mCompleter || (ctrlOrShift && event->text().isEmpty()))
			return;

		static QString endOfWord(QStringLiteral("~!@#$%^&*()_+{}|:\"<>?,/;'[]\\-="));
		bool hasModifier = (event->modifiers() != Qt::NoModifier) && !ctrlOrShift;
		QString completionPrefix = textUnderCursor();

		if(!isShortcut && (hasModifier || event->text().isEmpty() || completionPrefix.length() < 1 || endOfWord.contains(event->text().right(1))))
		{
			mCompleter->popup()->hide();
			return;
		}
		
		if(completionPrefix != mCompleter->completionPrefix())
		{
			mCompleter->setCompletionPrefix(completionPrefix);
			mCompleter->popup()->setCurrentIndex(mCompleter->completionModel()->index(0, 0));
		}

		QRect cr = cursorRect();
		cr.setWidth(mCompleter->popup()->sizeHintForColumn(0) + mCompleter->popup()->verticalScrollBar()->sizeHint().width());
		mCompleter->complete(cr); // popup it up!
	}

	void CodeEdit::keyReleaseEvent(QKeyEvent *event)
	{
		if(event->key() == Qt::Key_Return && event->modifiers() & Qt::ControlModifier)
			emit acceptDialog();
		else
			QPlainTextEdit::keyReleaseEvent(event);
	}

	bool CodeEdit::event(QEvent *event)
	{
		//Block the backtab key from changing the focus, since we use it to unindent
		if (event->type() == QEvent::KeyPress)
		{
			auto keyEvent = static_cast<QKeyEvent *>(event);
			if (keyEvent->key() == Qt::Key_Backtab)
			{
				keyPressEvent(keyEvent);

				return true;
			}
		}

		return QPlainTextEdit::event(event);
	}
}
