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
	
	void addEcmaScriptObjects(QStandardItemModel *model)
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
}
