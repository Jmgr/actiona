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

#pragma once

#include "code/codeclass.h"

#include <QObject>
#include <QScriptValue>
#include <QScriptEngine>
#include <QClipboard>

namespace Code
{
	class Clipboard : public CodeClass
	{
		Q_OBJECT
		Q_ENUMS(Mode)
		Q_ENUMS(DataType)
		Q_PROPERTY(QString text READ text WRITE setText)
		Q_PROPERTY(QScriptValue image READ image WRITE setImage)
	
	public:
		enum Mode
		{
			Standard =		QClipboard::Clipboard,
			Selection =		QClipboard::Selection,
			FindBuffer =	QClipboard::FindBuffer
		};
		enum DataType
		{
			Text,
			Image
		};
		
		static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);
	
		Clipboard();

		QString text() const;
		QScriptValue image() const;
	
	public slots:
		QString toString() const override                                { return QStringLiteral("Clipboard"); }
        bool equals(const QScriptValue &other) const override    { return defaultEqualsImplementation<Clipboard>(other); }
		QScriptValue setMode(Mode mode);
		QScriptValue setText(const QString &value) const;
		QScriptValue setImage(const QScriptValue &data) const;
		DataType dataType() const;
		
	private:
		void setModePrivate(QScriptContext *context, QScriptEngine *engine, Mode mode);
	
		QClipboard::Mode mMode{QClipboard::Clipboard};
	};
}

