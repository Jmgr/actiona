#ifndef SEVENZIPFILE_H
#define SEVENZIPFILE_H

#include "tools_global.h"

#include <QObject>
#include <QSharedData>

namespace Tools
{
	class TOOLSSHARED_EXPORT SevenZipFileData : public QSharedData
	{
	public:
		SevenZipFileData();


	};

	class TOOLSSHARED_EXPORT SevenZipFile : public QObject
	{
		Q_OBJECT

	public:
		explicit SevenZipFile(QObject *parent = 0);

	};
}

#endif // SEVENZIPFILE_H
