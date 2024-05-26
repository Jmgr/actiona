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

#include "basewindow.hpp"

class QProgressDialog;

namespace Code
{
	class ProgressDialog : public BaseWindow
	{
		Q_OBJECT
		Q_PROPERTY(QJSValue onCanceled READ onCanceled WRITE setOnCanceled)
		Q_PROPERTY(int value READ value WRITE setValue)
		
	public:
        Q_INVOKABLE ProgressDialog();
        Q_INVOKABLE ProgressDialog(const QJSValue &parameters);
		~ProgressDialog() override;
		
		void setOnCanceled(const QJSValue &onCanceled)					{ mOnCanceled = onCanceled; }
		
		QJSValue onCanceled() const										{ return mOnCanceled; }
		int value() const;
		
        Q_INVOKABLE QString toString() const override					{ return QStringLiteral("ProgressDialog"); }
        Q_INVOKABLE ProgressDialog *setValue(int value);
        Q_INVOKABLE ProgressDialog *setLabelText(const QString &labelText);
        Q_INVOKABLE ProgressDialog *setMinimum(int minimum);
        Q_INVOKABLE ProgressDialog *setMaximum(int maximum);
        Q_INVOKABLE ProgressDialog *setRange(int minimum, int maximum);
        Q_INVOKABLE ProgressDialog *show();
        Q_INVOKABLE int showModal();

        static void registerClass(ActionTools::ScriptEngine &scriptEngine);
		
	private slots:
		void canceled();
		
	private:
		QProgressDialog *mProgressDialog;
		QJSValue mOnCanceled;
	};
}

