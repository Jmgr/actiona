// File.h
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


#ifndef INCLUDED_File_H
#define INCLUDED_File_H

#ifndef INCLUDED_AbstractData_H
#include "AbstractData.h"
#endif

#ifndef INCLUDED_VECTOR
#include <vector>
#define INCLUDED_VECTOR
#endif

#ifndef INCLUDED_MAP
#include <map>
#define INCLUDED_MAP
#endif

#ifndef INCLUDED_STRING
#include <string>
#define INCLUDED_STRING
#endif

using namespace rude::config;

namespace rude{
namespace config{

class Section;

class File: public AbstractData{

private:
	Section *d_currentSection;
	mutable std::vector<Section*> d_sections;
	mutable std::map<std::string, Section*> d_sectionmap;

public:

	File();
	~File();
	void acceptWriter(AbstractWriter& writer) const;
	void clear();
	const char * getStringValue(const char *name) const;
	bool deleteData(const char *name);
	void setStringValue(const char *name, const char *value);
	int getNumDataMembers() const;
	const char *getDataNameAt(int index) const;
	const char *getDataValueAt(int index) const;
	bool exists(const char *name) const;
	int getNumSections() const;
	const char *getSectionNameAt(int index) const;
	bool setSection(const char *sectionname, bool shouldCreate);
	bool deleteSection(const char *sectionname);

	Section *getSection(const char *sectionname);

};

}} // end namespace rude::config

#endif

