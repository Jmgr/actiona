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
// This file uses some code lines from the Ofi Labs X2 project
// Copyright (C) 2010 Ariya Hidayat <ariya.hidayat@gmail.com>
// Licensed under GNU/GPLv3

#pragma once

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QSet>

#include "actiontools_global.hpp"

class QTextDocument;

namespace ActionTools
{
	class ACTIONTOOLSSHARED_EXPORT CodeHighlighter : public QSyntaxHighlighter
	{
		Q_OBJECT

	public:
		enum Format
		{
			NormalFormat,
			CommentFormat,
			NumberFormat,
			StringFormat,
			OperatorFormat,
			IdentifierFormat,
			KeywordFormat,
			ReservedFormat,
			CodeObjectsFormat,

			FormatCount
		};

		CodeHighlighter(QTextDocument *parent = nullptr);
		void addCodeObject(const QString &name);

	protected:
		void highlightBlock(const QString &text) override;

	private:
		QSet<QString> mUsedKeywords;
		QSet<QString> mReservedKeywords;
		QSet<QString> mCodeObjects;
		QTextCharFormat mFormats[FormatCount];

		Q_DISABLE_COPY(CodeHighlighter)
	};
}

