// AbstractWriter.h
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

#ifndef INCLUDED_AbstractWriter_H
#define INCLUDED_AbstractWriter_H

#ifndef INCLUDED_STRING
#include <string>
#define INCLUDED_STRING
#endif

namespace rude{
namespace config{

class File;
class Section;
class KeyValue;
class Comment;
class WhiteSpace;

class AbstractWriter{


protected:

	char d_commentchar;
	char d_delimiter;
	bool d_preservedeleted;
	bool d_preservecomments;
	bool d_preservewhitespace;
	
	std::ostream *d_outputstream;

	AbstractWriter();

public:

	virtual void visitFile(const File& configfile) const = 0;
	virtual void visitSection(const Section& configsection) const = 0;
	virtual void visitKeyValue(const KeyValue& dataline) const = 0;
	virtual void visitComment(const Comment& comment) const = 0;
	virtual void visitWhiteSpace(const WhiteSpace& whitespace) const = 0;

	void setOutputStream( std::ostream& stream);
	void setCommentChar(char);
	void setDelimiter(char);
	void preserveDeleted(bool doit);
	void preserveComments(bool doit);
	void preserveWhiteSpace(bool doit);

	virtual ~AbstractWriter();

};


}} // end namespaces

#endif

