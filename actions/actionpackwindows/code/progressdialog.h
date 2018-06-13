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

#include "basewindow.h"

class QProgressDialog;

namespace Code
{
	class ProgressDialog : public BaseWindow
	{
		Q_OBJECT
		Q_PROPERTY(QScriptValue onCanceled READ onCanceled WRITE setOnCanceled)
		Q_PROPERTY(int value READ value WRITE setValue)
		
	public:
		static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);		
		
		ProgressDialog();
		~ProgressDialog() override;
		
		void setOnCanceled(const QScriptValue &onCanceled)					{ mOnCanceled = onCanceled; }
		
		QScriptValue onCanceled() const										{ return mOnCanceled; }
		int value() const;
		
	public slots:
		QString toString() const override					{ return QStringLiteral("ProgressDialog"); }
		QScriptValue setValue(int value);
		QScriptValue setLabelText(const QString &labelText);
		QScriptValue setMinimum(int minimum);
		QScriptValue setMaximum(int maximum);
		QScriptValue setRange(int minimum, int maximum);
		QScriptValue show();
		int showModal();
		
	private slots:
		void canceled();
		
	private:
		QProgressDialog *mProgressDialog;
		QScriptValue mOnCanceled;
	};
}

