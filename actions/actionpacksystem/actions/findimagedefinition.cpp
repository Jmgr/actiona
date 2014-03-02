/*
    Actionaz
    Copyright (C) 2008-2014 Jonathan Mercier-Ganady

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

#include "findimagedefinition.h"
#include "actioninstance.h"
#include "ifactionparameterdefinition.h"

void Actions::FindImageDefinition::updateAction(ActionTools::ActionInstance *actionInstance, const Tools::Version &version) const
{
    //In 1.1.0 we have removed the ImageNotFound exception and added ifActions instead
    if(version < Tools::Version(1, 1, 0))
    {
        const ActionTools::ExceptionActionInstancesHash &exceptions = actionInstance->exceptionActionInstances();

        if(exceptions.contains(static_cast<ActionTools::ActionException::Exception>(FindImageInstance::CannotFindTheImageException)))
        {
            const ActionTools::ActionException::ExceptionActionInstance &exceptionAction = exceptions.value(static_cast<ActionTools::ActionException::Exception>(FindImageInstance::CannotFindTheImageException));
            ActionTools::Parameter ifNotFoundParameter = actionInstance->parameter("ifNotFound");

            switch(exceptionAction.action())
            {
            case ActionTools::ActionException::StopExecutionExceptionAction:
                ifNotFoundParameter.setSubParameter("action", false, ActionTools::IfActionParameterDefinition::actions.first[ActionTools::IfActionParameterDefinition::RunCode]);
                ifNotFoundParameter.setSubParameter("line", true, "Console.printError(\"Script line \" + Script.line + \": Cannot find the image\");\nExecution.stop();");
                break;
            case ActionTools::ActionException::SkipExceptionAction:
                ifNotFoundParameter.setSubParameter("action", false, ActionTools::IfActionParameterDefinition::actions.first[ActionTools::IfActionParameterDefinition::DoNothing]);
                break;
            case ActionTools::ActionException::GotoLineExceptionAction:
                ifNotFoundParameter.setSubParameter("action", false, ActionTools::IfActionParameterDefinition::actions.first[ActionTools::IfActionParameterDefinition::Goto]);
                ifNotFoundParameter.setSubParameter("line", false, exceptionAction.line());
                break;
            default:
                break;
            }

            actionInstance->setParameter("ifNotFound", ifNotFoundParameter);
        }
    }
}
