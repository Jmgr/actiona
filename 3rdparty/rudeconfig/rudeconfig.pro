QT -= gui
TEMPLATE = lib
CONFIG += dll
DEFINES += RUDECONFIG_LIBRARY
win32:DEFINES += _CRT_SECURE_NO_WARNINGS
TARGET = rudeconfig
DESTDIR = ../..
SOURCES += \
	Writer.cpp \
	WhiteSpace.cpp \
	Section.cpp \
	RealOrganiser.cpp \
	ParserJuly2004.cpp \
	KeyValue.cpp \
	File.cpp \
	DataLine.cpp \
	ConfigImpl.cpp \
	config.cpp \
	Comment.cpp \
	Base64Encoder.cpp \
	AbstractWriter.cpp \
	AbstractParser.cpp \
	AbstractOrganiser.cpp \
	AbstractData.cpp
HEADERS += \
	Writer.h \
	WhiteSpace.h \
	Section.h \
	RealOrganiser.h \
	ParserJuly2004.h \
	KeyValue.h \
	File.h \
	DataLine.h \
	ConfigImpl.h \
	config.h \
	Comment.h \
	Base64Encoder.h \
	AbstractWriter.h \
	AbstractParser.h \
	AbstractOrganiser.h \
	AbstractData.h
INCLUDEPATH += src
UI_DIR = generated
MOC_DIR = generated
RCC_DIR = generated
OBJECTS_DIR = generated
