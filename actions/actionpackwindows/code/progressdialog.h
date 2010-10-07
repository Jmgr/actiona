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

#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include "basewindow.h"

class QProgressDialog;

namespace Code
{
	class ProgressDialog : public BaseWindow
	{
		Q_OBJECT
		
	public:
		static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);		
		
		ProgressDialog();
		~ProgressDialog();
		
	public slots:
		QScriptValue setValue(int value);
		QScriptValue setLabelText(const QString &labelText);
		QScriptValue setMinimum(int minimum);
		QScriptValue setMaximum(int maximum);
		QScriptValue setRange(int minimum, int maximum);
		QScriptValue show();
		int showModal();
		int value() const;
		
	private slots:
		void canceled();
		
	private:
		QProgressDialog *mProgressDialog;
		QScriptValue mOnCanceled;
		QScriptValue mThisObject;
	};
}

#endif // PROGRESSDIALOG_H
