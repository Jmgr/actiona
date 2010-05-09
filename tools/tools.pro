include(../common.pri)
QT += network
TEMPLATE = lib
CONFIG += dll \
	qxt
QXT += core \
	gui \
	network
DEFINES += TOOLS_LIBRARY
TARGET = tools
DESTDIR = ..
SOURCES += version.cpp \
	actioninfo.cpp \
	7z/LzmaDec.c \
	7z/Bra86.c \
	7z/Bcj2.c \
	7z/7zStream.c \
	7z/7zItem.c \
	7z/7zIn.c \
	7z/7zHeader.c \
	7z/7zFile.c \
	7z/7zExtract.c \
	7z/7zDecode.c \
	7z/7zCrc.c \
	7z/7zBuf.c \
	7z/7zAlloc.c \
	sevenziparchive.cpp \
	updateclient.cpp
HEADERS += version.h \
	tools_global.h \
	actioninfo.h \
	7z/LzmaDec.h \
	7z/CpuArch.h \
	7z/Bra.h \
	7z/Bcj2.h \
	7z/7zVersion.h \
	7z/7zItem.h \
	7z/7zIn.h \
	7z/7zHeader.h \
	7z/7zFile.h \
	7z/7zExtract.h \
	7z/7zDecode.h \
	7z/7zCrc.h \
	7z/7zBuf.h \
	7z/7zAlloc.h \
	7z/Types.h \
	sevenziparchive.h \
	updateclient.h
INCLUDEPATH += src
UI_DIR = generated
MOC_DIR = generated
RCC_DIR = generated
OBJECTS_DIR = generated
TRANSLATIONS = locale/tools_en.ts \
	locale/tools_fr.ts
