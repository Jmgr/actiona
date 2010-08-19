// ConfigImpl.h
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

#ifndef INCLUDED_RUDE_CONFIGIMPL_H
#define INCLUDED_RUDE_CONFIGIMPL_H

#ifndef INCLUDED_STRING
#include <string>
#define INCLUDED_STRING
#endif

#ifndef INCLUDED_IOSTREAM
#include <iostream>
#define INCLUDED_IOSTREAM
#endif

namespace rude{
namespace config{

class Section;
class File;
class AbstractWriter;
class AbstractParser;
class AbstractOrganiser;

class ConfigImpl{

	static std::string s_defaultConfigFile;
	static char s_defaultCommentChar;
	static char s_defaultDelimiter;
	static bool s_defaultPreserveDeleted;
	static bool s_defaultAllowDuplicate;
	static bool s_defaultIgnoreCase;
	static std::string s_returnValue;

	AbstractWriter *d_writer;
	AbstractOrganiser *d_organiser;
	AbstractParser *d_parser;
	
	File *d_file;
	std::string d_error;
	std::string d_errorcode;

	std::string d_configfile;
	char d_commentcharacter;
	char d_delimiter;
	bool d_preserveDeleted;
	bool d_allowDuplicate;
	bool d_ignoreCase;

protected:

	void setError(const char *errorcode, const char *errorstring);

public:

	//////////////////////////////////////
	// STATIC METHODS
	//////////////////////////////////////
	static const char *version();
	
	// DEFAULT BEHAVIOR
	//
	static void setDefaultConfigFile(const char *filepath);
	static const char *getDefaultConfigFile();
			
	static void setDefaultCommentCharacter(char c);
	static char getDefaultCommentCharacter();

	static void setDefaultDelimiter(char c);
	static char getDefaultDelimiter();
	
	static void setDefaultPreserveDeleted(bool shouldPreserve);
	static bool getDefaultPreserveDeleted();

	// The internals are on the drawing board!!
	//
	static void setDefaultIgnoreCase(bool shouldIgnore);
	static void setDefaultAllowDuplicateKeys(bool shouldAllow);

			
	// DATA CONVERSION
	//
	static bool stringToBool(const char *string);
	static const char *boolToString(bool value);
	static int stringToInt(const char *string);
	static const char *intToString(int value);
	static double stringToDouble(const char *string);
	static const char *doubleToString(double value);
	static char * stringToBinary(const char *value, int &outlength);
	static const char *binaryToString(const char *value, int length); 


	//////////////////////////////////////
	// INSTANCE METHODS
	//////////////////////////////////////

	// CONSTRUCTOR & DESTRUCTOR
	//
	ConfigImpl();
	~ConfigImpl();

	// STATUS
	//
	const char *getError() const;
	const char *getErrorCode() const;

	// INSTANCE BEHAVIOR
	//
	void setConfigFile(const char *filepath);
	const char *getConfigFile();
	void setCommentCharacter(char c);
	void setDelimiter(char c);
	void preserveDeletedData(bool shouldPreserve);
	void ignoreCase(bool shouldIgnore);
	void allowDuplicateKeys(bool shouldAllow);

	// LOADING & SAVING & CLEARING
	//	
	bool load();
	bool load(const char *filepath);
	bool load(std::istream&);
	
	bool save();
	bool save(const char *filepath);
	bool save(std::ostream&);

	void clear();


	// SECTION METHODS
	//
	int getNumSections() const;
	const char *getSectionNameAt(int index) const;
	bool setSection(const char *sectionname, bool shouldCreate);
	bool setSection(const char *sectionname);
	bool deleteSection(const char *sectionname);


	// DATA METHODS
	//
	int getNumDataMembers() const;
	const char *getDataNameAt(int index) const;
	const char *getDataValueAt(int index) const;
	bool exists(const char *name) const;
	
	const char * getStringValue(const char *name) const;
	void setStringValue(const char *name, const char *value);
	bool deleteData(const char *name);

	// Working with Duplicate Keys
	//
	int getNumDataMembers(const char *key) const;
	const char * getStringValue(const char *name, int index) const;
	void addStringValue(const char *name, const char *value);
	bool deleteData(const char *name, int index);



};
}} // end namespace rude::config
#endif

