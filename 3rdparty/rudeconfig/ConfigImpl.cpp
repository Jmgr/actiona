// configimpl.cpp
//
// Copyright (C) 2000, 2001, 2002, 2003, 2004, 2005 Matthew Flood
// See file AUTHORS for contact information
//
// This file is part of RudeConfig.
//
// RudeConfig is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.
// 
// RudeConfig is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with RudeConfig; (see COPYING) if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
// 02111-1307, USA.
//------------------------------------------------------------------------
#include "ConfigImpl.h"

#ifndef INCLUDED_Section_H
#include "Section.h"
#endif

#ifndef INCLUDED_File_H
#include "File.h"
#endif

#ifndef INCLUDED_Writer_H
#include "Writer.h"
#endif

#ifndef INPUT_RealOrganiser_h
#include "RealOrganiser.h"
#endif

#ifndef INPUT_ParserJuly2004_h
#include "ParserJuly2004.h"
#endif

#ifndef INCLUDED_BASE64ENCODER_H
#include "Base64Encoder.h"
#endif


#ifndef INCLUDED_CSTDIO
#include <cstdio>
#define INCLUDED_CSTDIO
#endif

#ifndef INCLUDED_CSTDLIB
#include <cstdlib>
#define INCLUDED_CSTDLIB
#endif

#ifndef INCLUDED_IOSTREAM
#include <iostream>
#define INCLUDED_IOSTREAM
#endif

#ifndef INCLUDED_FSTREAM
#include <fstream>
#define INCLUDED_FSTREAM
#endif

#ifndef INCLUDED_CSTRING
#include <cstring>
#define INCLUDED_CSTRING
#endif


using namespace std;

namespace rude{
namespace config{

// STATIC DATA
//

std::string ConfigImpl::s_defaultConfigFile = "default.ini";
char ConfigImpl::s_defaultCommentChar = '#';
char ConfigImpl::s_defaultDelimiter = '=';
bool ConfigImpl::s_defaultPreserveDeleted = false;
bool ConfigImpl::s_defaultAllowDuplicate = false;
bool ConfigImpl::s_defaultIgnoreCase = false;
std::string ConfigImpl::s_returnValue="";


//////////////////////////////////////
// STATIC METHODS
//////////////////////////////////////

const char *ConfigImpl::version()
{
	return "0";
}
	
void ConfigImpl::setDefaultConfigFile(const char *filepath)
{
	s_defaultConfigFile = filepath ? filepath : "default.ini";
}
const char *ConfigImpl::getDefaultConfigFile()
{
	return s_defaultConfigFile.c_str();
}


void ConfigImpl::setDefaultCommentCharacter(char c)
{
	s_defaultCommentChar = c;
}

char ConfigImpl::getDefaultCommentCharacter()
{
	return s_defaultCommentChar;
}


void ConfigImpl::setDefaultDelimiter(char c)
{
	s_defaultDelimiter = c;
}

char ConfigImpl::getDefaultDelimiter()
{
	return s_defaultDelimiter;
}

void ConfigImpl::setDefaultPreserveDeleted(bool shouldPreserve)
{
	s_defaultPreserveDeleted = shouldPreserve;
}

bool ConfigImpl::getDefaultPreserveDeleted()
{
	return s_defaultPreserveDeleted;
}


void ConfigImpl::setDefaultIgnoreCase(bool shouldIgnore)
{
	s_defaultIgnoreCase = shouldIgnore;
}

void ConfigImpl::setDefaultAllowDuplicateKeys(bool shouldAllow)
{
	s_defaultAllowDuplicate = shouldAllow;
}

// STATIC DATA CONVERSION
//
bool ConfigImpl::stringToBool(const char *value)
{
	if(value)
	{
		// Values that mean true:
		// yes, on, true, 1
		// [yY]*, [oO][nN]*, O*, [Tt]*, 1
		// Values that mean false:
		// no, off, false, 0

		switch(value[0])
		{
			case 't':
				return true;
			case 'T':
				return true;
			case 'y':
				return true;
			case 'Y':
				return true;
			case '1':
				return true;
			case 'o':
				if(value[1] == 'n' || value[1] == 'N')
				{
					// since value[0] was not null, we can rest assured that there is a value[1] -
					// it is at least the null string terminator!
					return true;
				}
				break;
			case 'O':
				if(value[1] == 'n' || value[1] == 'N')
				{
					return true;
				}
				break;
			default:
				return false;
		}
	}
	return false;
}

const char *ConfigImpl::boolToString(bool value)
{
	return (value ? "true" : "false");
}

int ConfigImpl::stringToInt(const char *string)
{
	if(string)
	{
		return atoi(string);
	}
	return 0;
}

const char *ConfigImpl::intToString(int value)
{
	char myint[25];
	sprintf(myint, "%i", value);
	s_returnValue = myint;
	return s_returnValue.c_str();
}

double ConfigImpl::stringToDouble(const char *string)
{
	if(string)
	{
		return atof(string);
	}
	return 0;
}

const char *ConfigImpl::doubleToString(double value)
{
	char mydouble[25];
	sprintf(mydouble, "%g", value);
	s_returnValue = mydouble;
	return s_returnValue.c_str();
}

char * ConfigImpl::stringToBinary(const char *value, int &outlength)
{
	// Base64Encoder USAGE:
	// static char * decode(const char *data, int datalength, int &outlength);

	if(value)
	{
		int datalength=std::strlen(value);
		return Base64Encoder::decode(value, datalength, outlength);
	}
	outlength=0;
	return 0;
}

const char *ConfigImpl::binaryToString(const char *value, int length)
{
	
	// Base64Encoder USAGE:
	// static char * encode(const char *data, int datalength, int &outlength);

	if(value)
	{
		int outlength;
		s_returnValue = Base64Encoder::encode(value, length, outlength);
	}
	else
	{
		s_returnValue = "";
	}
	return s_returnValue.c_str();
}


//////////////////////////////////////
// INSTANCE METHODS
//////////////////////////////////////


	///////////////////////////
	// CONSTRUCTOR & DESTRUCTOR
	///////////////////////////

ConfigImpl::ConfigImpl()
{
	d_file = new File();

	// Create the appropriate Objects here, 
	// all other methods interact with
	// the base class interfaces
	// 
	d_writer = new Writer();
	
	d_parser = new ParserJuly2004();

	d_error="";
	d_errorcode="";
	
	d_commentcharacter = s_defaultCommentChar;
	d_delimiter = s_defaultDelimiter;
	d_configfile = s_defaultConfigFile;
	d_preserveDeleted = s_defaultPreserveDeleted;
	d_allowDuplicate = s_defaultAllowDuplicate;
	d_ignoreCase = s_defaultIgnoreCase;


} 

ConfigImpl::~ConfigImpl()
{
	delete d_writer;
	delete d_parser;
	delete d_file;
}

	////////////
	// STATUS
	////////////

void ConfigImpl::setError(const char *errorcode, const char *errorstring)
{
	d_errorcode = errorcode ? errorcode : "";
	d_error = errorstring ? errorstring : "";
}

const char *ConfigImpl::getError() const
{
	return d_error.c_str();
}

const char *ConfigImpl::getErrorCode() const
{
	return d_errorcode.c_str();
}

	//////////////////////
	// INSTANCE BEHAVIOR
	//////////////////////

void ConfigImpl::setConfigFile(const char *filepath)
{
	d_configfile = filepath ? filepath : s_defaultConfigFile.c_str();
}
const char * ConfigImpl::getConfigFile()
{
	return d_configfile.c_str();
}

void ConfigImpl::setCommentCharacter(char c)
{
	d_commentcharacter = c;
}

void ConfigImpl::setDelimiter(char c)
{
	d_delimiter = c;
}

void ConfigImpl::preserveDeletedData(bool shouldPreserve)
{
	d_preserveDeleted = shouldPreserve;
}

void ConfigImpl::ignoreCase(bool shouldIgnore)
{
	d_ignoreCase = shouldIgnore;
}

void ConfigImpl::allowDuplicateKeys(bool shouldAllow)
{
	d_allowDuplicate = shouldAllow;
}

	//////////////////////////////
	// LOADING & SAVING & CLEARING
	//////////////////////////////

bool ConfigImpl::load()
{
	// forward to load(filepath)
	//
	return load(d_configfile.c_str());
}

bool ConfigImpl::load(const char *filepath)
{

	if(filepath && filepath[0])
	{
		d_configfile=filepath;
		std::ifstream infile(filepath);

		if(infile)
		{
			// forward to load(std::istream&)
			bool retval = load(infile);
			infile.close();
			return retval;
		}
		else
		{
			setError("2001", "Error opening config file for reading");
			return false;
		}
	}
	else
	{
		return load(std::cin);
	}
}

bool ConfigImpl::load(std::istream& inputstream)
{	
	d_parser->setCommentCharacter(d_commentcharacter);
	
	d_parser->setDelimiter(d_delimiter);

	RealOrganiser organiser(d_file);
	
	if(d_parser->parse(inputstream, organiser))
	{
		setSection("");
		return true;
	}
	else
	{	
		setSection("");
		setError(d_parser->getErrorCode(), d_parser->getError());
		return false;
	}
}
	
bool ConfigImpl::save()
{
	// forward to save(filepath)
	//
	return save(d_configfile.c_str());
}

bool ConfigImpl::save(const char *filepath)
{
	if(filepath && filepath[0] != 0)
	{
		ofstream outstream(filepath);
		if(outstream)
		{
			// forward to save(std::ostream&)
			//
			bool retval = save(outstream);
			outstream.close();
			return retval;
		}
		else
		{
			setError("2002", "Error opening config file for writing");
			return false;
		}
	}
	else
	{
		return save(std::cout);
	}
}

bool ConfigImpl::save(std::ostream& outstream)
{
	d_writer->setOutputStream(outstream);
	d_writer->setCommentChar(d_commentcharacter);
	d_writer->setDelimiter(d_delimiter);
	d_writer->preserveDeleted(d_preserveDeleted);

	d_file->acceptWriter(*d_writer);

	return true;
}

void ConfigImpl::clear()
{
	d_file->clear();
}


	//////////////////
	// SECTION METHODS
	//////////////////


int ConfigImpl::getNumSections() const
{
	return d_file->getNumSections();
}

const char *ConfigImpl::getSectionNameAt(int index) const
{
	return d_file->getSectionNameAt(index);
}

bool ConfigImpl::setSection(const char *sectionname, bool shouldCreate)
{
	return d_file->setSection(sectionname, shouldCreate);
}

bool ConfigImpl::setSection(const char *sectionname)
{
	return d_file->setSection(sectionname, true);
}

bool ConfigImpl::deleteSection(const char *sectionname)
{
	return d_file->deleteSection(sectionname);
}

	////////////////
	// DATA METHODS
	////////////////


int ConfigImpl::getNumDataMembers() const
{
		return d_file->getNumDataMembers();
}

const char *ConfigImpl::getDataNameAt(int index) const
{
		return d_file->getDataNameAt(index);
}

const char *ConfigImpl::getDataValueAt(int index) const
{
		return d_file->getDataValueAt(index);
}

bool ConfigImpl::exists(const char *name) const
{
		return d_file->exists(name);
}
	
const char * ConfigImpl::getStringValue(const char *name) const
{
	return d_file->getStringValue(name);
}

void ConfigImpl::setStringValue(const char *name, const char *value)
{
	d_file->setStringValue(name, value);
}

bool ConfigImpl::deleteData(const char *name)
{
	return d_file->deleteData(name);
}

	///////////////////////////////
	// Working with Duplicate Keys
	// NOT IMPLEMENTED YET
	///////////////////////////////

int ConfigImpl::getNumDataMembers(const char *key) const
{
	if(exists(key))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

const char * ConfigImpl::getStringValue(const char *name, int index) const
{
	return d_file->getStringValue(name);
}

void ConfigImpl::addStringValue(const char *name, const char *value)
{
	d_file->setStringValue(name, value);
}

bool ConfigImpl::deleteData(const char *name, int index)
{
	return d_file->deleteData(name);
}

}}// end namespace rude::config
