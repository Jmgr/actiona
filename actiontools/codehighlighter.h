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

#ifndef CODEHIGHLIGHTER_H
#define CODEHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QHash>
#include <QTextCharFormat>

#include "actiontools_global.h"

class QTextDocument;

namespace ActionTools
{
	class ACTIONTOOLSSHARED_EXPORT CodeHighlighter : public QSyntaxHighlighter
	{
		Q_OBJECT

	public:
		explicit CodeHighlighter(QTextDocument *parent = 0);
		
		void addAction(const QString &actionName);

	private:
		void highlightBlock(const QString &text);
		struct HighlightingRule
		{
			QRegExp pattern;
			QTextCharFormat format;
		};
		QVector<HighlightingRule> highlightingRules;

		QRegExp commentStartExpression;
		QRegExp commentEndExpression;

		QTextCharFormat keywordFormat;
		QTextCharFormat classFormat;
		QTextCharFormat singleLineCommentFormat;
		QTextCharFormat multiLineCommentFormat;
		QTextCharFormat quotationFormat;
		QTextCharFormat functionFormat;
		QTextCharFormat actionFormat;

		Q_DISABLE_COPY(CodeHighlighter)
	};
}

#endif // CODEHIGHLIGHTER_H
