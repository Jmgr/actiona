// Writer.h
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


#ifndef INCLUDED_Writer_H
#define INCLUDED_Writer_H

#include "AbstractWriter.h"
#include <iostream>

namespace rude{
namespace config{

class File;
class Section;
class KeyValue;
class Comment;
class WhiteSpace;


class Writer: public AbstractWriter{

public:

	Writer();
		
	virtual void visitFile(const File& configfile) const;
	virtual void visitSection(const Section& configsection) const;
	virtual void visitKeyValue(const KeyValue& keyvalue) const;
	virtual void visitComment(const Comment& comment) const;
	virtual void visitWhiteSpace(const WhiteSpace& whitespace) const;

	virtual ~Writer();

	//=
	// Writes the data member to an output stream
	// 
	// For the following specification examples, assume an object with the following properties:
	// 
	// <b>name</b> = "color"
	// <b>value</b>="blue"
	// <b>comment</b>="Color of the background"
	// 
	// <ul>
	// <li>If comment char is null, comments/deleted items will not be written
	// 
	// <b>Example:</b> object->write(stdout, 0, "=")
	// <b>Results:</b>
	// <font color=red><code>color = blue</code></font>	 
	// 
	// <li>If delimiter is null, name / value will be separated by whitespace
	// 
	// <b>Example:</b> object->write(stdout, "#", 0)
	// <b>Results:</b>
	// <font color=red><code>color  blue  # Color of the background</code></font>	 
	// 
	// <li>Undefined results if outputstream is null
	// 
	// <b>Example:</b> object->write(0, "#", "=")
	// <b>Results:</b>
	// <font color=red><code>??????????</code></font>	 
	// 
	// <li>If the data member is a comment, then only the comment data is written - any values
	// for the name / value are discarded.
	// 
	// <b>Example:</b> object->write(stdout, "#", "=")
	// <b>Results:</b>
	// <font color=red><code># Color of the background</code></font>	 
	// 
	// <li>If the data member has been (flagged as) deleted, then the name, value and original comment are preserved, 
	// preceded by a comment character.
	// 
	// <b>Example:</b> object->write(stdout, "#", "=")
	// <b>Results:</b>
	// <font color=red><code># color = blue  # Color of the background</code></font>	 
	// 
	// </ul>
	//=


};

}} // end namespaces

#endif

