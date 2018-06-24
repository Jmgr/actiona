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

#include "rawdata.h"

namespace Code
{
	QScriptValue RawData::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		RawData *rawData = nullptr;
		
		switch(context->argumentCount())
		{
		case 0:
			rawData = new RawData;
			break;
		case 1:
			{
				QObject *object = context->argument(0).toQObject();
				if(auto codeRawData = qobject_cast<RawData*>(object))
					rawData = new RawData(*codeRawData);
				else
					throwError(context, engine, QStringLiteral("ParameterTypeError"), tr("Incorrect parameter type"));
			}
			break;
		default:
			throwError(context, engine, QStringLiteral("ParameterCountError"), tr("Incorrect parameter count"));
			break;
		}
		
		if(!rawData)
			return engine->undefinedValue();
		
		return CodeClass::constructor(rawData, context, engine);
	}
	
	QScriptValue RawData::constructor(const RawData &other, QScriptEngine *engine)
	{
		return CodeClass::constructor(new RawData(other), engine);
	}
	
	QScriptValue RawData::constructor(const QByteArray &byteArray, QScriptEngine *engine)
	{
		return CodeClass::constructor(new RawData(byteArray), engine);
	}

	void RawData::registerClass(QScriptEngine *scriptEngine)
	{
		CodeTools::addClassToScriptEngine<RawData>(scriptEngine);
	}
	
	RawData::RawData()
		: CodeClass()
	{
	}
	
	RawData::RawData(const RawData &other)
		: CodeClass(),
		mByteArray(other.mByteArray)
	{
	}
	
	RawData::RawData(const QByteArray &byteArray)
		: CodeClass(),
		mByteArray(byteArray)
	{
	}
	
	RawData &RawData::operator=(RawData other)
	{
		swap(other);
		
		return *this;
	}
	
	RawData &RawData::operator=(QByteArray byteArray)
	{
		swap(byteArray);
		
		return *this;
	}
	
	void RawData::swap(RawData &other)
	{
		std::swap(mByteArray, other.mByteArray);
	}
	
	void RawData::swap(QByteArray &byteArray)
	{
		std::swap(mByteArray, byteArray);
	}
	
	const QByteArray &RawData::byteArray() const
	{
		return mByteArray;
	}
	
	QScriptValue RawData::clone() const
	{
		return constructor(mByteArray, engine());
	}
	
	bool RawData::equals(const QScriptValue &other) const
	{
		if(other.isUndefined() || other.isNull())
			return false;
		
		QObject *object = other.toQObject();
		if(auto otherRawData = qobject_cast<RawData*>(object))
			return (otherRawData == this || otherRawData->mByteArray == mByteArray);
			
		return false;
	}

	QString RawData::toString() const
	{
		return QStringLiteral("RawData {size: %1}").arg(size());
	}
	
	QScriptValue RawData::append(const QVariant &data)
	{
		mByteArray.append(data.toByteArray());
		
		return thisObject();
	}
	
	QScriptValue RawData::chop(int n)
	{
		mByteArray.chop(n);
		
		return thisObject();
	}
	
	QScriptValue RawData::clear()
	{
		mByteArray.clear();
		
		return thisObject();
	}
	
	bool RawData::contains(const QVariant &data)
	{
		return mByteArray.contains(data.toByteArray());
	}
	
	int RawData::count(const QVariant &data) const
	{
		return mByteArray.count(data.toByteArray());
	}
	
	bool RawData::endsWith(const QVariant &data) const
	{
		return mByteArray.endsWith(data.toByteArray());
	}
	
	int RawData::indexOf(const QVariant &data, int from) const
	{
		return mByteArray.indexOf(data.toByteArray(), from);
	}
	
	bool RawData::isEmpty() const
	{
		return mByteArray.isEmpty();
	}
	
	int RawData::lastIndexOf(const QVariant &data) const
	{
		return mByteArray.lastIndexOf(data.toByteArray());
	}
	
	QScriptValue RawData::left(int len) const
	{
		return RawData::constructor(mByteArray.left(len), engine());
	}
	
	int RawData::length() const
	{
		return mByteArray.length();
	}
	
	QScriptValue RawData::mid(int pos, int len) const
	{
		return RawData::constructor(mByteArray.mid(pos, len), engine());
	}
	
	QScriptValue RawData::prepend(const QVariant &data)
	{
		mByteArray.prepend(data.toByteArray());
		
		return thisObject();
	}
	
	QScriptValue RawData::remove(int pos, int len)
	{
		mByteArray.remove(pos, len);
		
		return thisObject();
	}
	
	QScriptValue RawData::replace(const QString &before, const QString &after)
	{
		mByteArray.replace(before.toLatin1(), after.toLatin1());
		
		return thisObject();
	}
	
	QScriptValue RawData::resize(int size)
	{
		mByteArray.resize(size);
		
		return thisObject();
	}
	
	QScriptValue RawData::right(int len) const
	{
		return RawData::constructor(mByteArray.right(len), engine());
	}
	
	QScriptValue RawData::setData(const QVariant &data)
	{
		mByteArray = data.toByteArray();
		
		return thisObject();
	}
	
	int RawData::size() const
	{
		return mByteArray.size();
	}
	
	bool RawData::startsWith(const QVariant &data)
	{
		return mByteArray.startsWith(data.toByteArray());
	}
	
	double RawData::convertToNumber() const
	{
		return mByteArray.toDouble();
	}
	
	QString RawData::convertToString(Encoding encoding) const
	{
		return fromEncoding(mByteArray, encoding);
	}
	
	QScriptValue RawData::truncate(int pos)
	{
		mByteArray.truncate(pos);
		
		return thisObject();
	}
}
