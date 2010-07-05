#include "sevenziparchivewrite.h"

#include <QMessageBox>
#include <QProcess>
#include <QDir>
#include <QDebug>

namespace Tools
{
	SevenZipArchiveWrite::SevenZipArchiveWrite(const QString &filename, QObject *parent)
		: QObject(parent),
		mFilename(filename)
	{
	}

	bool SevenZipArchiveWrite::addFile(const QString &filename)
	{
		QStringList arguments;
		arguments << "a" << mFilename << filename;

		return (QProcess::execute(QDir::current().filePath("sfx/7zr.exe"), arguments) == 0);
	}
}
