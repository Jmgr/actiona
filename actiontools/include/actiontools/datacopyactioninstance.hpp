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

#include "actiontools_global.hpp"
#include "actiontools/actioninstance.hpp"

#include <QTimer>

class QIODevice;

namespace ActionTools
{
	class DeviceCopyThread;
	
	class ACTIONTOOLSSHARED_EXPORT DataCopyActionInstance : public ActionInstance
	{
		Q_OBJECT
		
	public:
		DataCopyActionInstance(const ActionDefinition *definition = nullptr, QObject *parent = nullptr);
		DataCopyActionInstance(const ActionInstance &other) : ActionInstance(other)			{}
		~DataCopyActionInstance()													override = default;
		
		void stopExecution() override ;
		
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
		qint64 mTotalSize{0};
	};
}

