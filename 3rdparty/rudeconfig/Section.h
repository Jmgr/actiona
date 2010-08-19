// Section.h
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

#ifndef INCLUDED_Section_H
#define INCLUDED_Section_H

#ifndef INCLUDED_STRING
#include <string>
#define INCLUDED_STRING
#endif

#ifndef INCLUDED_VECTOR
#include <vector>
#define INCLUDED_VECTOR
#endif

#ifndef INCLUDED_MAP
#include <map>
#define INCLUDED_MAP
#endif

#ifndef INCLUDED_IOSTREAM
#include <iostream>
#define INCLUDED_IOSTREAM
#endif


#ifndef INCLUDED_STRING
#include <string>
#define INCLUDED_STRING
#endif

namespace rude{
namespace config{

class DataLine;
class KeyValue;
class AbstractWriter;


//=
// Section represents a single section within a configuration file.
// It is identified by its section name, which can include whitespace, 
// and manages the set of all data and/or comments associated with that section.
// 
// Each piece of data is identified by its name.  Currently, only one piece of data
// is recognized for any given name.  In the case where two or more values exist for the
// same name (within a section), only the last value is recognized - any previous values are discarded.
//=
class Section{

	bool d_isDeleted;
	std::string d_sectionname;
	std::string d_sectioncomment;
	mutable std::vector<DataLine*> d_allDataVector;
	mutable std::vector<KeyValue*> d_kv_vector;
	mutable std::map<std::string, KeyValue*> d_kv_map;
public:

	Section(const char *sectionname, const char *sectioncomment=0);
	void acceptWriter(AbstractWriter& writer) const;

	//=
	// Writes the section and all associated data to the outputstream
	//
	// <b>If the section is marked as deleted:</b>
	// The entire section is commented out.  This means that it is still printed out,
	// but each line is begun with the comment character.  If the comment character is
	// set to null, then the section is not printed out at all - it is essentially discarded.
	//
	// In normal circumstances, the '=' is the most appropriate delimiter to use.
	// If delimiter is null, then whie
	//=
	void write(std::ostream& outputstream, char commentchar, char delimiter, bool preserveDeleted);
	
	//=
	// Returns this section's name, identified by [somename] in the config/ini file
	//=
	const char *getSectionName() const;


	//=
	// Returns the section-comment (found after the section name)
	// 
	// An example of a section-comment is:
	//
	// [Dogs]  # this section is about dogs
	//
	//=
	const char *getSectionComment() const;


	//=
	// Sets the section comment (found after the section name)
	//=
	void setSectionComment(const char *newcomment);


	//=
	// Returns whether or not this section is flagged as deleted...
	//=
	bool isDeleted() const;


	//=
	// (Un)marks this section as deleted
	// When marked as deleted, all data members are marked as deleted
	// When undeleted, data members remain marked as deleted....
	//=
	void isDeleted(bool is_it_or_not);


	//=
	// Returns the number of (non-deleted) data members
	// Deleted data, comments and blank lines are not included in the count.
	//=
	int getNumDataMembers() const;


	//=
	// Returns the name of the (undeleted) data member at the specified index
	// Returns NULL if the index is out of range
	//=
	const char *getDataNameAt(int index) const;

	//=
	// Returns the name of the (undeleted) data member at the specified index
	// Returns NULL if the index is out of range
	//=
	const char *getDataValueAt(int index) const;


	//=
	// Returns true if data name exists (and is not marked as deleted), false otherwise
	//=
	bool exists(const char *name) const;

	
	//=
	// Returns the value associated with the data identified by <i>name</i>
	// Returns NULL if data name does not exist (or is marked as deleted)
	//=
	const char *getValue(const char *name) const;


	//=
	// Returns the comment associated with the data identified by <i>name</i>
	// Returns NULL if data name does not exist (or is marked as deleted)
	//=
	const char *getComment(const char *name) const;

	
	//=
	// Sets the <b>value</b> for the data member identified by <i>name</i>
	// If a data member already exists with the given <i>name</i>, then the value is replaced. 
	// Otherwise a new data item is created..
	// If the data item already exists, but has been marked as deleted,
	// it will be revived (unmarked) and its value updated.
	// Existing comments associated with the data member are not altered.
	//=
	void setValue(const char *name, const char *value);


	//=
	// Replaces the comment for the data member identified by <i>name</i>
	// Sets the comment whether or not the data is marked as deleted.
	// If the data identified by <i>name</i> does not exist, then nothing happens
	//=
	void setComment(const char *name, const char *comment);

	
	//=
	// Sets the <b>value</b> and the <b>comment</b> for the data member identified by <i>name</i>
	// If a data member already exists with the given <i>name</i>, then the value is replaced. 
	// Otherwise a new data item is created..
	// If the data item already exists, but has been marked as deleted,
	// it will be revived (unmarked) and updated.
	//=
	void setValue(const char *name, const char *value, const char *comment);


	//=
	// Adds a comment line to the end of the section
	//=	
	void addComment(const char *comment);
	
	//=
	// Adds whitespace line to the end of the section
	//=	
	void addWhiteSpace(const char *whitespace);

	//=
	// Marks the data identified by name as deleted
	// Returns false if data does not exist within this 
	// section (or has already been marked as deleted)
	//=
	bool deleteData(const char *name);


	~Section();
	
};
}} // end namespace rude::config
#endif

