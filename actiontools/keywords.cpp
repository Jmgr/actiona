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

#include "keywords.h"

#include <QStandardItemModel>
#include <QMetaMethod>
#include <QMetaEnum>

namespace ActionTools
{
	const QStringList usedKeywords = QStringList()
									 << "break"
									 << "for"
									 << "throw"
									 << "case"
									 << "function"
									 << "try"
									 << "catch"
									 << "if"
									 << "typeof"
									 << "continue"
									 << "in"
									 << "var"
									 << "default"
									 << "instanceof"
									 << "void"
									 << "delete"
									 << "new"
									 << "undefined"
									 << "do"
									 << "return"
									 << "while"
									 << "else"
									 << "switch"
									 << "with"
									 << "finally"
									 << "this";
	
	const QStringList reservedKeywords = QStringList()
									 << "abstract"
									 << "final"
									 << "protected"
									 << "boolean"
									 << "float"
									 << "public"
									 << "byte"
									 << "goto"
									 << "short"
									 << "char"
									 << "implements"
									 << "static"
									 << "class"
									 << "import"
									 << "super"
									 << "const"
									 << "int"
									 << "synchronized"
									 << "debugger"
									 << "interface"
									 << "throws"
									 << "double"
									 << "long"
									 << "transient"
									 << "enum"
									 << "native"
									 << "volatile"
									 << "export"
									 << "package"
									 << "extends"
									 << "private";
	
	void addEcmaScriptObjectsKeywords(QStandardItemModel *model)
	{
		const QStringList globalKeywords = QStringList()
										 //Constants
										 << "NaN" << "Infinity" << "undefined"
										 //Functions
										 << "eval(x)" << "parseInt(string, radix)" << "parseInt(string)" << "parseFloat(string)" << "isNaN(number)" << "isFinite(number)"
										 << "decodeURI(encodedURI)" << "decodeURIComponent(encodedURIComponent)" << "encodeURI(uri)" << "encodeURIComponent(uriComponent)";
		
		const QStringList mathKeywords = QStringList()
										 //Constants
										 << "E" << "LN10" << "LN2" << "LOG2E" << "LOG10E" << "PI" << "SQRT1_2" << "SQRT2"
										 //Functions
										 << "abs(x)" << "acos(x)" << "asin(x)" << "atan(x)" << "atan2(x, y)" << "ceil(x)" << "cos(x)" << "exp(x)" << "floor(x)"
										 << "log(x)" << "max(x, y, ...)" << "min(x, y, ...)" << "pow(x, y)" << "random()" << "round(x)" << "sin(x)" << "sqrt(x)"
										 << "tan(x)";

		foreach(const QString &keyword, usedKeywords)
			model->appendRow(new QStandardItem(QIcon(":/icons/keywords.png"), keyword));
		foreach(const QString &keyword, reservedKeywords)
			model->appendRow(new QStandardItem(QIcon(":/icons/no.png"), keyword));
		
		foreach(const QString &keyword, globalKeywords)
		{
			QStandardItem *globalItem = new QStandardItem(QIcon(":/icons/keywords.png"), keyword);//TODO : Find a nice icon
			globalItem->setData(static_cast<int>(ActionTools::ScriptElementAction));
			model->appendRow(globalItem);
		}
		
		QStandardItem *mathItem = new QStandardItem(QIcon(":/icons/keywords.png"), "Math");//TODO : Find a nice icon
		foreach(const QString &keyword, mathKeywords)
			mathItem->appendRow(new QStandardItem(QIcon(":/icons/keywords.png"), keyword));//TODO : Find a nice icon
		mathItem->setData(static_cast<int>(ActionTools::ScriptElementAction));
		model->appendRow(mathItem);
	}
	
	void addClassKeywords(const QMetaObject *metaObject, const QString &name, const QIcon &icon, QStandardItemModel *model, const QStringList &ignoreList)
	{
		QStandardItem *currentItem = 0;
		
		//Add all methods
		for(int methodIndex = 0; methodIndex < metaObject->methodCount(); ++methodIndex)
		{
			const QMetaMethod &method = metaObject->method(methodIndex);
			
			if(method.access() != QMetaMethod::Public || method.methodType() != QMetaMethod::Slot)
				continue;
			
			const QString &signature(method.signature());
			bool shouldIgnore = false;
			foreach(const QString &ignore, ignoreList)
			{
				if(signature.startsWith(ignore))
				{
					shouldIgnore = true;
					break;
				}
			}
			
			if(shouldIgnore)
				continue;
			
			QString methodName(signature.left(signature.indexOf("(")));
			methodName += "(";
			
			bool firstParameter = true;
			foreach(const QByteArray &parameter, method.parameterNames())
			{
				if(firstParameter)
					firstParameter = false;
				else
					methodName += ", ";
				
				methodName += parameter;
			}
			
			methodName += ")";

			if(!currentItem)
				currentItem = new QStandardItem(icon, name);
			
			QStandardItem *methodItem = new QStandardItem(icon, methodName);
			currentItem->appendRow(methodItem);
		}
		
		//Add all enums
		if(currentItem)//Only add enums if a valid method has been added
		{
			for(int enumIndex = 0; enumIndex < metaObject->enumeratorCount(); ++enumIndex)
			{
				const QMetaEnum &enumerator = metaObject->enumerator(enumIndex);
				
				QStandardItem *enumNameItem = new QStandardItem(icon, enumerator.name());
				
				for(int keyIndex = 0; keyIndex < enumerator.keyCount(); ++keyIndex)
				{
					QStandardItem *enumItem = new QStandardItem(icon, enumerator.key(keyIndex));
					enumNameItem->appendRow(enumItem);
				}

				currentItem->appendRow(enumNameItem);
			}
			
			currentItem->setData(static_cast<int>(ActionTools::ScriptElementAction));
			model->appendRow(currentItem);
		}
	}
}
