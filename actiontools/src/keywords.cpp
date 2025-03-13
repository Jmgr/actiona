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

#include "actiontools/keywords.hpp"

#include <QStandardItemModel>
#include <QMetaMethod>
#include <QMetaEnum>

namespace ActionTools
{
    const QStringList usedKeywords =
    {
        QStringLiteral("break"),
        QStringLiteral("for"),
        QStringLiteral("throw"),
        QStringLiteral("case"),
        QStringLiteral("function"),
        QStringLiteral("try"),
        QStringLiteral("catch"),
        QStringLiteral("if"),
        QStringLiteral("typeof"),
        QStringLiteral("continue"),
        QStringLiteral("in"),
        QStringLiteral("var"),
        QStringLiteral("default"),
        QStringLiteral("instanceof"),
        QStringLiteral("void"),
        QStringLiteral("delete"),
        QStringLiteral("new"),
        QStringLiteral("undefined"),
        QStringLiteral("do"),
        QStringLiteral("return"),
        QStringLiteral("while"),
        QStringLiteral("else"),
        QStringLiteral("switch"),
        QStringLiteral("with"),
        QStringLiteral("finally"),
        QStringLiteral("this"),
        QStringLiteral("let"),
        QStringLiteral("const"),
        QStringLiteral("yield"),
        QStringLiteral("async"),
        QStringLiteral("await"),
        QStringLiteral("null"),
        QStringLiteral("true"),
        QStringLiteral("false"),
        QStringLiteral("class"),
        QStringLiteral("import"),
        QStringLiteral("export"),
        QStringLiteral("super"),
        QStringLiteral("static"),
        QStringLiteral("debugger"),
    };
	
    const QStringList reservedKeywords =
    {
        QStringLiteral("abstract"),
        QStringLiteral("final"),
        QStringLiteral("protected"),
        QStringLiteral("boolean"),
        QStringLiteral("float"),
        QStringLiteral("public"),
        QStringLiteral("byte"),
        QStringLiteral("goto"),
        QStringLiteral("short"),
        QStringLiteral("char"),
        QStringLiteral("implements"),
        QStringLiteral("int"),
        QStringLiteral("synchronized"),
        QStringLiteral("interface"),
        QStringLiteral("throws"),
        QStringLiteral("double"),
        QStringLiteral("long"),
        QStringLiteral("transient"),
        QStringLiteral("enum"),
        QStringLiteral("native"),
        QStringLiteral("volatile"),
        QStringLiteral("package"),
        QStringLiteral("extends"),
        QStringLiteral("private"),
    };
	
	void addEcmaScriptObjectsKeywords(QStandardItemModel *model)
	{
        const QStringList globalKeywords =
        {
            //Constants
            QStringLiteral("NaN"),
            QStringLiteral("Infinity"),
            QStringLiteral("undefined"),
            //Functions
            QStringLiteral("eval(x)"),
            QStringLiteral("parseInt(string, radix)"),
            QStringLiteral("parseInt(string)"),
            QStringLiteral("parseFloat(string)"),
            QStringLiteral("isNaN(number)"),
            QStringLiteral("isFinite(number)"),
            QStringLiteral("decodeURI(encodedURI)"),
            QStringLiteral("decodeURIComponent(encodedURIComponent)"),
            QStringLiteral("encodeURI(uri)"),
            QStringLiteral("encodeURIComponent(uriComponent)")
        };
		
        const QStringList mathKeywords =
        {
            //Constants
            QStringLiteral("E"),
            QStringLiteral("LN10"),
            QStringLiteral("LN2"),
            QStringLiteral("LOG2E"),
            QStringLiteral("LOG10E"),
            QStringLiteral("PI"),
            QStringLiteral("SQRT1_2"),
            QStringLiteral("SQRT2"),
            //Functions
            QStringLiteral("abs(x)"),
            QStringLiteral("acos(x)"),
            QStringLiteral("asin(x)"),
            QStringLiteral("atan(x)"),
            QStringLiteral("atan2(x, y)"),
            QStringLiteral("ceil(x)"),
            QStringLiteral("cos(x)"),
            QStringLiteral("exp(x)"),
            QStringLiteral("floor(x)"),
            QStringLiteral("log(x)"),
            QStringLiteral("max(x, y, ...)"),
            QStringLiteral("min(x, y, ...)"),
            QStringLiteral("pow(x, y)"),
            QStringLiteral("random()"),
            QStringLiteral("round(x)"),
            QStringLiteral("sin(x)"),
            QStringLiteral("sqrt(x)"),
            QStringLiteral("tan(x)")
        };

		for(const QString &keyword: usedKeywords)
			model->appendRow(new QStandardItem(QIcon(QStringLiteral(":/icons/keywords.png")), keyword));
		for(const QString &keyword: reservedKeywords)
			model->appendRow(new QStandardItem(QIcon(QStringLiteral(":/icons/no.png")), keyword));
		
		for(const QString &keyword: globalKeywords)
			model->appendRow(new QStandardItem(QIcon(QStringLiteral(":/icons/native.png")), keyword));
		
		QStandardItem *mathItem = new QStandardItem(QIcon(QStringLiteral(":/icons/calc.png")), QStringLiteral("Math"));
		for(const QString &keyword: mathKeywords)
			mathItem->appendRow(new QStandardItem(QIcon(QStringLiteral(":/icons/calc.png")), keyword));
		model->appendRow(mathItem);
	}
}
