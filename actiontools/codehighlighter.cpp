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

#include "codehighlighter.h"
#include "keywords.h"

#include <QFile>
#include <QDebug>

namespace ActionTools
{
	CodeHighlighter::CodeHighlighter(QTextDocument *parent)
		: QSyntaxHighlighter(parent)
	{
		HighlightingRule rule;
		QTextCharFormat format;

		//Used keywords
		format.setForeground(Qt::darkBlue);
		format.setFontWeight(QFont::Bold);
		rule.format = format;
	
		foreach(const QString &keyword, usedKeywords)
		{
			rule.pattern = QRegExp("\\b" + keyword + "\\b");
			mHighlightingRules.append(rule);
		}
		
		//Reserved keywords
		format.setForeground(Qt::red);
		format.setFontWeight(QFont::Bold);
		format.setFontStrikeOut(true);
		rule.format = format;

		foreach(const QString &keyword, reservedKeywords)
		{
			rule.pattern = QRegExp("\\b" + keyword + "\\b");
			mHighlightingRules.append(rule);
		}
	
		format = QTextCharFormat();
		
		//Actions
		mCodeClassFormat.setForeground(Qt::darkBlue);
		mCodeClassFormat.setFontWeight(QFont::Bold);
		
		//Operators
		format.setForeground(Qt::red);
		rule.pattern = QRegExp("[\\-\\+\\*/=%\\^!~<>\\?:,&\\|]");
		rule.format = format;
		mHighlightingRules.append(rule);
		
		//Numbers
		format.setForeground(Qt::magenta);
		rule.pattern = QRegExp("\\b[\\d]+\\.?[\\d]*\\b");
		rule.format = format;
		mHighlightingRules.append(rule);
		
		//Multi line comments
		mMultiLineCommentFormat.setForeground(Qt::darkGreen);

		//Quotes
		format.setForeground(Qt::darkRed);
		rule.pattern = QRegExp("\".*\"");
		rule.format = format;
		mHighlightingRules.append(rule);

		//Single line comments
		format.setForeground(Qt::darkGreen);
		rule.pattern = QRegExp("//[^\n]*");
		rule.format = format;
		mHighlightingRules.append(rule);

		mCommentStartExpression = QRegExp("/\\*");
		mCommentEndExpression = QRegExp("\\*/");
	}
	
	void CodeHighlighter::addAction(const QString &actionName)
	{
		HighlightingRule rule;
		rule.pattern = QRegExp(QString("\\b%1\\b").arg(actionName));
		rule.format = mCodeClassFormat;
		mHighlightingRules.prepend(rule);
	}

	void CodeHighlighter::highlightBlock(const QString &text)
	{
		foreach(const HighlightingRule &rule, mHighlightingRules)
		{
			QRegExp expression(rule.pattern);
			int index = expression.indexIn(text);
			while(index >= 0)
			{
				int length = expression.matchedLength();
				setFormat(index, length, rule.format);
				index = expression.indexIn(text, index + length);
			}
		}
		setCurrentBlockState(0);

		int startIndex = 0;
		if(previousBlockState() != 1)
			startIndex = mCommentStartExpression.indexIn(text);

		while(startIndex >= 0)
		{
			int endIndex = mCommentEndExpression.indexIn(text, startIndex);
			int commentLength;
			
			if(endIndex == -1)
			{
				setCurrentBlockState(1);
				commentLength = text.length() - startIndex;
			}
			else
				commentLength = endIndex - startIndex + mCommentEndExpression.matchedLength();

			setFormat(startIndex, commentLength, mMultiLineCommentFormat);
			startIndex = mCommentStartExpression.indexIn(text, startIndex + commentLength);
		}
	}
}
