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
		
		if(!actionInstanceExecutionHelper.evaluatePoint(mPixelPosition, "pixel", "position") ||
		   !actionInstanceExecutionHelper.evaluateColor(mPixelColor, "pixel", "color") ||
		   !actionInstanceExecutionHelper.evaluateString(mIfEqualAction, "ifEqual", "action") ||
		   !actionInstanceExecutionHelper.evaluateString(mIfEqualLine, "ifEqual", "line") ||
		   !actionInstanceExecutionHelper.evaluateString(mIfDifferentAction, "ifDifferent", "action") ||
		   !actionInstanceExecutionHelper.evaluateString(mIfDifferentLine, "ifDifferent", "line") ||
		   !actionInstanceExecutionHelper.evaluateVariable(mVariable, "variable"))
			return;
		
		QString action;
		QString line;
		QColor color = pixelColor(mPixelPosition.x(), mPixelPosition.y());
		
		if(!mVariable.isEmpty())
			actionInstanceExecutionHelper.setVariable(mVariable, color);
		
		if(color == mPixelColor)
		{
			action = mIfEqualAction;
			line = mIfEqualLine;
			actionInstanceExecutionHelper.setCurrentParameter("ifEqual", "line");
		}
		else
		{
			action = mIfDifferentAction;
			line = mIfDifferentLine;
			actionInstanceExecutionHelper.setCurrentParameter("ifDifferent", "line");
		}
		
		if(action == "goto")
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
	QPoint mPixelPosition;
	QColor mPixelColor;
	QString mIfEqualAction;
	QString mIfEqualLine;
	QString mIfDifferentAction;
	QString mIfDifferentLine;
	QString mVariable;
	
	Q_DISABLE_COPY(ActionPixelColorInstance)
};

#endif // ACTIONPIXELCOLORINSTANCE_H
