/*
	Actionaz
	Copyright (C) 2008-2010 Jonathan Mercier-Ganady

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

#include <QFile>

namespace ActionTools
{
	CodeHighlighter::CodeHighlighter(QTextDocument *parent)
		: QSyntaxHighlighter(parent)
	{
		HighlightingRule rule;

		keywordFormat.setForeground(Qt::darkBlue);
		keywordFormat.setFontWeight(QFont::Bold);
		QStringList keywordPatterns;

		QFile file(":/code/keywords.txt");
		if(file.open(QFile::ReadOnly))
		{
			while(!file.atEnd())
			{
				QByteArray line = file.readLine();
				if(!line.isEmpty())
					keywordPatterns << "\\b" + line.trimmed() + "\\b";
			}
		}

		foreach(const QString &pattern, keywordPatterns)
		{
			rule.pattern = QRegExp(pattern);
			rule.format = keywordFormat;
			highlightingRules.append(rule);
		}
		
		actionFormat.setFontWeight(QFont::Bold);
		
		classFormat.setFontWeight(QFont::Bold);
		classFormat.setForeground(Qt::darkMagenta);
		rule.pattern = QRegExp("\\bQ[A-Za-z]+\\b");
		rule.format = classFormat;
		highlightingRules.append(rule);

		singleLineCommentFormat.setForeground(Qt::red);
		rule.pattern = QRegExp("//[^\n]*");
		rule.format = singleLineCommentFormat;
		highlightingRules.append(rule);

		multiLineCommentFormat.setForeground(Qt::red);

		quotationFormat.setForeground(Qt::darkGreen);
		rule.pattern = QRegExp("\".*\"");
		rule.format = quotationFormat;
		highlightingRules.append(rule);

		functionFormat.setFontItalic(true);
		functionFormat.setForeground(Qt::blue);
		rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
		rule.format = functionFormat;
		highlightingRules.append(rule);

		commentStartExpression = QRegExp("/\\*");
		commentEndExpression = QRegExp("\\*/");
	}
	
	void CodeHighlighter::addAction(const QString &actionName)
	{
		HighlightingRule rule;
		rule.pattern = QRegExp(QString("\\b%1\\b").arg(actionName));
		rule.format = actionFormat;
		highlightingRules.append(rule);
	}

	void CodeHighlighter::highlightBlock(const QString &text)
	{
		foreach(const HighlightingRule &rule, highlightingRules)
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
		startIndex = commentStartExpression.indexIn(text);

		while(startIndex >= 0)
		{
			int endIndex = commentEndExpression.indexIn(text, startIndex);
			int commentLength;
			if (endIndex == -1)
			{
				setCurrentBlockState(1);
				commentLength = text.length() - startIndex;
			}
			else
			{
				commentLength = endIndex - startIndex + commentEndExpression.matchedLength();
			}
			setFormat(startIndex, commentLength, multiLineCommentFormat);
			startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
		}
	}
}
