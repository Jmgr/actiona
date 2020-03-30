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

#include <QWidget>

#include "codeedit.hpp"
#include "actiontools_global.hpp"

namespace ActionTools
{
	class ACTIONTOOLSSHARED_EXPORT LineNumberArea : public QWidget
	{
	public:
		explicit LineNumberArea(CodeEdit *editor) : QWidget(editor)
		{
			codeEditor = editor;
		}

		QSize sizeHint() const override
		{
			return {codeEditor->lineNumberAreaWidth(), 0};
		}

	protected:
		void paintEvent(QPaintEvent *event) override
		{
			codeEditor->lineNumberAreaPaintEvent(event);
		}

	private:
		CodeEdit *codeEditor;

		Q_DISABLE_COPY(LineNumberArea)
	};
}

