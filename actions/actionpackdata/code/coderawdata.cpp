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

#include "coderawdata.h"

QScriptValue CodeRawData::constructor(QScriptContext *context, QScriptEngine *engine)
{
	Q_UNUSED(context)
	
	return engine->newQObject(new CodeRawData, QScriptEngine::ScriptOwnership);
}

QScriptValue CodeRawData::constructor(const CodeRawData &other, QScriptContext *context, QScriptEngine *engine)
{
	Q_UNUSED(context)
	
	return engine->newQObject(new CodeRawData(other), QScriptEngine::ScriptOwnership);
}

QScriptValue CodeRawData::constructor(const QByteArray &byteArray, QScriptContext *context, QScriptEngine *engine)
{
	Q_UNUSED(context)
	
	return engine->newQObject(new CodeRawData(byteArray), QScriptEngine::ScriptOwnership);
}

CodeRawData::CodeRawData()
	: QObject(),
	QScriptable()
{
}

CodeRawData::CodeRawData(const CodeRawData &other)
	: QObject(),
	QScriptable(),
	mByteArray(other.mByteArray)
{
}

CodeRawData::CodeRawData(const QByteArray &byteArray)
	: QObject(),
	QScriptable(),
	mByteArray(byteArray)
{
}

CodeRawData &CodeRawData::operator=(CodeRawData other)
{
	swap(other);
	
	return *this;
}

CodeRawData &CodeRawData::operator=(QByteArray byteArray)
{
	swap(byteArray);
	
	return *this;
}

void CodeRawData::swap(CodeRawData &other)
{
	std::swap(mByteArray, other.mByteArray);
}

void CodeRawData::swap(QByteArray &byteArray)
{
	std::swap(mByteArray, byteArray);
}

QByteArray CodeRawData::byteArray() const
{
	return mByteArray;
}

QScriptValue CodeRawData::append(const QVariant &data)
{
	mByteArray.append(data.toByteArray());
	
	return context()->thisObject();
}

QScriptValue CodeRawData::chop(int n)
{
	mByteArray.chop(n);
	
	return context()->thisObject();
}

QScriptValue CodeRawData::clear()
{
	mByteArray.clear();
	
	return context()->thisObject();
}

bool CodeRawData::contains(const QVariant &data)
{
	return mByteArray.contains(data.toByteArray());
}

int CodeRawData::count(const QVariant &data) const
{
	return mByteArray.count(data.toByteArray());
}

int CodeRawData::count() const
{
	return mByteArray.count();
}

bool CodeRawData::endWith(const QVariant &data) const
{
	return mByteArray.endsWith(data.toByteArray());
}

int CodeRawData::indexOf(const QVariant &data, int from) const
{
	return mByteArray.indexOf(data.toByteArray(), from);
}

bool CodeRawData::isEmpty() const
{
	return mByteArray.isEmpty();
}

int CodeRawData::lastIndexOf(const QVariant &data) const
{
	return mByteArray.lastIndexOf(data.toByteArray());
}

QScriptValue CodeRawData::left(int len) const
{
	mByteArray.left(len);
	
	return context()->thisObject();
}

int CodeRawData::length() const
{
	return mByteArray.length();
}

QScriptValue CodeRawData::mid(int pos, int len) const
{
	mByteArray.mid(pos, len);
	
	return context()->thisObject();
}

QScriptValue CodeRawData::prepend(const QVariant &data)
{
	mByteArray.prepend(data.toByteArray());
	
	return context()->thisObject();
}

QScriptValue CodeRawData::remove(int pos, int len)
{
	mByteArray.remove(pos, len);
	
	return context()->thisObject();
}

QScriptValue CodeRawData::replace(const QString &before, const QString &after)
{
	mByteArray.replace(before.toLatin1(), after.toLatin1());
	
	return context()->thisObject();
}

QScriptValue CodeRawData::resize(int size)
{
	mByteArray.resize(size);
	
	return context()->thisObject();
}

QScriptValue CodeRawData::right(int len) const
{
	mByteArray.right(len);
	
	return context()->thisObject();
}

QScriptValue CodeRawData::setData(const QVariant &data)
{
	mByteArray = data.toByteArray();
	
	return context()->thisObject();
}

QScriptValue CodeRawData::simplified() const
{
	mByteArray.simplified();
	
	return context()->thisObject();
}

int CodeRawData::size() const
{
	return mByteArray.size();
}

bool CodeRawData::startsWith(const QVariant &data)
{
	return mByteArray.startsWith(data.toByteArray());
}

double CodeRawData::toNumber() const
{
	return mByteArray.toDouble();
}

QString CodeRawData::toString(Code::Encoding encoding) const
{
	return Code::fromEncoding(mByteArray, encoding);
}

QScriptValue CodeRawData::trimmed() const
{
	mByteArray.trimmed();
	
	return context()->thisObject();
}

QScriptValue CodeRawData::truncate(int pos)
{
	mByteArray.truncate(pos);
	
	return context()->thisObject();
}
