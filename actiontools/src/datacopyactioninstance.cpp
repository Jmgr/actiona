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

#include "actiontools/datacopyactioninstance.hpp"
#include "actiontools/devicecopythread.hpp"

#include <QIODevice>

namespace ActionTools
{
	DataCopyActionInstance::DataCopyActionInstance(const ActionDefinition *definition, QObject *parent)
		: ActionInstance(definition, parent)
		
	{
        connect(&mProgressTimer, &QTimer::timeout, this, &DataCopyActionInstance::updateProgress);
		
		mProgressTimer.setSingleShot(false);
		mProgressTimer.setInterval(50);
	}
	
	void DataCopyActionInstance::stopExecution()
	{
		clean();
	}
	
	bool DataCopyActionInstance::startCopy(QIODevice *input, QIODevice *output)
	{
		if(!input->isOpen() && !input->open(QIODevice::ReadOnly))
			return false;
		
		if(!output->isOpen() && !output->open(QIODevice::WriteOnly))
		{
			input->close();
			return false;
		}
		
		mInput = input;
		mOutput = output;
		mTotalSize = input->size();
		mDeviceCopyThread = new ActionTools::DeviceCopyThread(input, output);
		
        connect(mDeviceCopyThread, &ActionTools::DeviceCopyThread::finished, this, &DataCopyActionInstance::done);
		
		mProgressTimer.start();
		mDeviceCopyThread->start();
		
		return true;
	}
	
	void DataCopyActionInstance::clean()
	{
		emit hideProgressDialog();
		
		mProgressTimer.stop();
		
		mDeviceCopyThread->stop();
		mDeviceCopyThread->wait();
		delete mDeviceCopyThread;
		
		mInput->close();
		mOutput->close();
	}
	
	void DataCopyActionInstance::updateProgress()
	{
		emit updateProgressDialog((mDeviceCopyThread->copiedData() * 100) / mTotalSize);
	}
	
	void DataCopyActionInstance::done()
	{
		clean();
	
		executionEnded();
	}
}
