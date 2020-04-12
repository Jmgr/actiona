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

#include "actiontools/actioninstance.hpp"
#include "../keyboarddevice.hpp"
#include "tools/stringlistpair.hpp"

class QTimer;

namespace Actions
{
	class KeyInstance : public ActionTools::ActionInstance
	{
		Q_OBJECT

	public:
		enum Action
		{
			PressReleaseAction,
			PressAction,
			ReleaseAction
		};
		enum Type
		{
			Win32Type,
			DirectXType
		};
		enum Exceptions
		{
			FailedToSendInputException = ActionTools::ActionException::UserException,
			InvalidActionException
		};

		KeyInstance(const ActionTools::ActionDefinition *definition, QObject *parent = nullptr);
		
        static Tools::StringListPair actions;
        static Tools::StringListPair types;

		void startExecution() override;
		void stopExecution() override;
		void stopLongTermExecution() override;

	private slots:
		void sendRelease();
		void sendPressKey();

	private:
		void pressOrReleaseModifiers(bool press);

		KeyboardDevice mKeyboardDevice;
		QString mKey;
		bool mCtrl;
		bool mAlt;
		bool mShift;
		bool mMeta;
		int  mAmount;
		int  mPause;
		QTimer *mTimer;

		Q_DISABLE_COPY(KeyInstance)
	};
}

