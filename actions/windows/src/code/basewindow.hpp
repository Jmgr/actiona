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

#include <QJSValue>

#include "actiontools/code/codeclass.hpp"
#include "actiontools/code/point.hpp"

namespace Code
{
	class BaseWindow : public CodeClass
	{
		Q_OBJECT
		Q_PROPERTY(QString title READ title WRITE setTitle)
        Q_PROPERTY(const Code::Point *position READ position WRITE setPosition)
		Q_PROPERTY(float opacity READ opacity WRITE setOpacity)
		Q_PROPERTY(bool enabled READ enabled WRITE setEnabled)
		Q_PROPERTY(bool visible READ visible WRITE setVisible)

    public:
        Q_INVOKABLE BaseWindow *setTitle(const QString &title);
        Q_INVOKABLE BaseWindow *setPosition(const Code::Point *position);
        Q_INVOKABLE BaseWindow *setOpacity(float opacity);
        Q_INVOKABLE BaseWindow *setEnabled(bool enabled);
        Q_INVOKABLE BaseWindow *setVisible(bool visible);
        Q_INVOKABLE BaseWindow *setWindowIcon(const QJSValue &windowIcon);
        Q_INVOKABLE BaseWindow *close();
        Q_INVOKABLE bool equals(const QJSValue &other) const;

	protected:
		BaseWindow();

		QString title() const;
        const Code::Point *position() const;
		float opacity() const;
		bool enabled() const;
		bool visible() const;

		void setWidget(QWidget *widget);
        void setupConstructorParameters(const QJSValue &parameters);

	private:
		QWidget *mWindow{nullptr};
	};
}

