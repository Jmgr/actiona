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

#include "actiontools/code/codeclass.hpp"

#include <QJSValue>
#include <QClipboard>

namespace Code
{
	class Clipboard : public CodeClass
	{
		Q_OBJECT
		Q_PROPERTY(QString text READ text WRITE setText)
		Q_PROPERTY(QJSValue image READ image WRITE setImage)
	
	public:
		enum Mode
		{
			Standard =		QClipboard::Clipboard,
			Selection =		QClipboard::Selection,
			FindBuffer =	QClipboard::FindBuffer
		};
        Q_ENUM(Mode)
		enum DataType
		{
			Text,
			Image
		};
        Q_ENUM(DataType)
		
        Q_INVOKABLE Clipboard();
        Q_INVOKABLE explicit Clipboard(Mode mode);

		QString text() const;
		QJSValue image() const;
	
        Q_INVOKABLE QString toString() const override           { return QStringLiteral("Clipboard"); }
        Q_INVOKABLE Clipboard *setMode(Mode mode);
        Q_INVOKABLE Clipboard *setText(const QString &value);
        Q_INVOKABLE Clipboard *setImage(const QJSValue &data);
        Q_INVOKABLE DataType dataType() const;

        static void registerClass(ActionTools::ScriptEngine &scriptEngine);
		
	private:
        void setModePrivate(Mode mode);
	
		QClipboard::Mode mMode{QClipboard::Clipboard};
	};
}

