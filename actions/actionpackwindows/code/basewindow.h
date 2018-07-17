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

#include <QScriptValue>
#include <QScriptEngine>

#include "code/codeclass.h"

namespace Code
{
	class BaseWindow : public CodeClass
	{
		Q_OBJECT
		Q_PROPERTY(QString title READ title WRITE setTitle)
		Q_PROPERTY(QScriptValue position READ position WRITE setPosition)
		Q_PROPERTY(float opacity READ opacity WRITE setOpacity)
		Q_PROPERTY(bool enabled READ enabled WRITE setEnabled)
		Q_PROPERTY(bool visible READ visible WRITE setVisible)

	public slots:
		QScriptValue setTitle(const QString &title);
		QScriptValue setPosition(const QScriptValue &);
		QScriptValue setOpacity(float opacity);
		QScriptValue setEnabled(bool enabled);
		QScriptValue setVisible(bool visible);
		QScriptValue setWindowIcon(const QScriptValue &windowIcon);
		QScriptValue close();
        bool equals(const QScriptValue &other) const override;

	protected:
		BaseWindow();

		QString title() const;
		QScriptValue position() const;
		float opacity() const;
		bool enabled() const;
		bool visible() const;

		void setWidget(QWidget *widget);
		void setupConstructorParameters(QScriptContext *context, QScriptEngine *engine, const QScriptValue &parameters);

	private:
		QWidget *mWindow{nullptr};
	};
}

