// KeyValue.h
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


#ifndef INCLUDED_KeyValue_H
#define INCLUDED_KeyValue_H

#ifndef INCLUDED_DataLine_H
#include "DataLine.h"
#endif

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
//=
// KeyValue represents a single line/entry in a configuration file.
// It can represent whitespace, a comment, or key=value data.
// 
// <B>Current Limitations</B>
// <ul>
// <li> <b>Data Names:</b> it is assumed that the names for entries are valid.  Invalid names are not detected.
//						A name containing the delimiter or comment character is invalid.
//						A name containing newline information is invalid.
// Future releases should either detect and reject invalid names, or use an escape mechanism such as backslashing
// to allow invalid characters to appear within a name.
// 
// <li> <b>Data Values:</b> Same as data names. (see above)
// </ul>
//=
class KeyValue: public DataLine{


	std::string d_name;
	std::string d_value;
	std::string d_comment;

public:

	// default constructor
	KeyValue();
	KeyValue(const char *name, const char *value, const char *comment);
	
	void acceptWriter(AbstractWriter& writer) const;

	//= 
	// Returns the name of the data member
	// Will return the name even if the data member is flagged as being deleted or a comment
	// Always returns at least the empty string, will never return null.
	//=
	const char *getName() const;

	//= 
	// Returns the string value of the data member
	// Will return the value even if the data member is flagged as being deleted or a comment
	// Always returns at least the empty string, will never return null.
	//=
	const char *getValue() const;

	//= 
	// Returns the comment associated with the data member
	// Will return the comment even if the data member is flagged as being deleted
	// Always returns at least the empty string, will never return null.
	//=
	const char *getComment() const;

	//= 
	// Sets the name of the data member
	// Will set the name even if the data member is flagged as being deleted or a comment
	// Accepts null
	//=
	void setName(const char *name);

	//= 
	// Sets the string value of the data member
	// Will set the value even if the data member is flagged as being deleted or a comment
	// Accepts null
	//=
	void setValue(const char *value); // un-deletes if previously deleted.....

	//= 
	// Sets the comment associated with the data member
	// Will set the comment even if the data member is flagged as being deleted
	// Accepts null
	//=
	void setComment(const char *comment);
	
	~KeyValue();
	
};

}} // end namespace rude::config

#endif

