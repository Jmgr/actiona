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

#ifndef ACTIONPIXELCOLORINSTANCE_H
#define ACTIONPIXELCOLORINSTANCE_H

#include "actioninstanceexecutionhelper.h"
#include "actioninstance.h"
#include "script.h"
#include "ifactionvalue.h"

#include <QPoint>
#include <QPixmap>
#include <QApplication>
#include <QDesktopWidget>

class ActionPixelColorInstance : public ActionTools::ActionInstance
{
	Q_OBJECT

public:
	ActionPixelColorInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0)
		: ActionTools::ActionInstance(definition, parent)											{}

	void startExecution()
	{
		ActionTools::ActionInstanceExecutionHelper actionInstanceExecutionHelper(this, script(), scriptEngine());
		
		QPoint pixelPosition;
		QColor pixelColorValue;
		ActionTools::IfActionValue ifEqual;
		ActionTools::IfActionValue ifDifferent;
		QString variable;
		
		if(!actionInstanceExecutionHelper.evaluatePoint(pixelPosition, "pixel", "position") ||
		   !actionInstanceExecutionHelper.evaluateColor(pixelColorValue, "pixel", "color") ||
		   !actionInstanceExecutionHelper.evaluateIfAction(ifEqual, "ifEqual") ||
		   !actionInstanceExecutionHelper.evaluateIfAction(ifDifferent, "ifDifferent") ||
		   !actionInstanceExecutionHelper.evaluateVariable(variable, "variable"))
			return;
		
		QString action;
		QString line;
		QColor color = pixelColor(pixelPosition.x(), pixelPosition.y());
		
		if(!variable.isEmpty())
			actionInstanceExecutionHelper.setVariable(variable, color);
		
		if(color == pixelColorValue)
		{
			action = ifEqual.action();
			line = ifEqual.line();
			actionInstanceExecutionHelper.setCurrentParameter("ifEqual", "line");
		}
		else
		{
			action = ifDifferent.action();
			line = ifDifferent.line();
			actionInstanceExecutionHelper.setCurrentParameter("ifDifferent", "line");
		}
		
		if(action == ActionTools::IfActionValue::GOTO)
			actionInstanceExecutionHelper.setNextLine(line);

		emit executionEnded();
	}
	
public slots:
	QColor pixelColor(int positionX, int positionY) const
	{
		QPixmap pixel = QPixmap::grabWindow(QApplication::desktop()->winId(), positionX, positionY, 1, 1);
		
		return pixel.toImage().pixel(0, 0);
	}
	
private:
	Q_DISABLE_COPY(ActionPixelColorInstance)
};

#endif // ACTIONPIXELCOLORINSTANCE_H
