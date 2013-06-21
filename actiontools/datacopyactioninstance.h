/*
	Actionaz
	Copyright (C) 2008-2013 Jonathan Mercier-Ganady

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

#ifndef DATACOPYACTIONINSTANCE_H
#define DATACOPYACTIONINSTANCE_H

#include "actiontools_global.h"
#include "actioninstance.h"

#include <QTimer>

namespace ActionTools
{
	class DeviceCopyThread;
	
	class ACTIONTOOLSSHARED_EXPORT DataCopyActionInstance : public ActionInstance
	{
		Q_OBJECT
		
	public:
		DataCopyActionInstance(const ActionDefinition *definition = 0, QObject *parent = 0);
		DataCopyActionInstance(const ActionInstance &other) : ActionInstance(other)			{}
		virtual ~DataCopyActionInstance()													{}
		
		virtual void stopExecution();
		
	protected:
		bool startCopy(QIODevice *input, QIODevice *output);
		virtual void clean();
		
	private slots:
		void updateProgress();
		
	protected slots:
		virtual void done();
		
	private:
		QTimer mProgressTimer;
		DeviceCopyThread *mDeviceCopyThread;
		QIODevice *mInput;
		QIODevice *mOutput;
		qint64 mTotalSize;
	};
}

#endif // DATACOPYACTIONINSTANCE_H
