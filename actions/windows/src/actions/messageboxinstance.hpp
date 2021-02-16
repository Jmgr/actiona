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
#include "actiontools/ifactionvalue.hpp"

#include <QMessageBox>

namespace Actions
{
	class MessageBoxInstance : public ActionTools::ActionInstance
	{
		Q_OBJECT

	public:
		enum Icon
		{
			None,
			Information,
			Question,
			Warning,
			Error
		};
		Q_ENUM(Icon)
		enum Buttons
		{
			OkButton,
			YesNoButtons
		};
		Q_ENUM(Buttons)
		enum Result
		{
			Ok,
			Yes,
			No
		};
		Q_ENUM(Result)
		enum TextMode
		{
			AutoTextMode,
			HtmlTextMode,
			PlainTextMode
		};
		Q_ENUM(TextMode)

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

