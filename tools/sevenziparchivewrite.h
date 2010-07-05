#ifndef SEVENZIPARCHIVEWRITE_H
#define SEVENZIPARCHIVEWRITE_H

#include "tools_global.h"

#include <QObject>
#include <QString>

namespace Tools
{
	class TOOLSSHARED_EXPORT SevenZipArchiveWrite : QObject
	{
	public:
		SevenZipArchiveWrite(const QString &filename, QObject *parent = 0);

		bool addFile(const QString &filename);

	private:
		QString mFilename;
	};
}

#endif // SEVENZIPARCHIVEWRITE_H
