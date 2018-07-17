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

#include "actioninstance.h"
#include "ifactionvalue.h"

#include <QMessageBox>

namespace Actions
{
	class MessageBoxInstance : public ActionTools::ActionInstance
	{
		Q_OBJECT
		Q_ENUMS(Icon)
		Q_ENUMS(Buttons)
		Q_ENUMS(Result)
		Q_ENUMS(TextMode)

	public:
		enum Icon
		{
			None,
			Information,
			Question,
			Warning,
			Error
		};
		enum Buttons
		{
			OkButton,
			YesNoButtons
		};
		enum Result
		{
			Ok,
			Yes,
			No
		};
		enum TextMode
		{
			AutoTextMode,
			HtmlTextMode,
			PlainTextMode
		};

		MessageBoxInstance(const ActionTools::ActionDefinition *definition, QObject *parent = nullptr);

        static Tools::StringListPair icons;
        static Tools::StringListPair buttons;
        static Tools::StringListPair textmodes;

		void startExecution() override;
		void stopExecution() override;

	private:
		QMessageBox::Icon messageBoxIcon(Icon icon) const;

	private slots:
		void buttonClicked();

	private:
		void closeAndDelete();

		QMessageBox *mMessageBox;
		ActionTools::IfActionValue mIfYes;
		ActionTools::IfActionValue mIfNo;

		Q_DISABLE_COPY(MessageBoxInstance)
	};
}

