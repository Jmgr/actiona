// AbstractWriter.cc
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

#include "AbstractWriter.h"

#ifndef INCLUDED_IOSTREAM
#include <iostream>
#define INCLUDED_IOSTREAM
#endif

namespace rude{
namespace config{

AbstractWriter::AbstractWriter()
{
	d_commentchar = '#';
	d_delimiter = '=';
	d_preservedeleted = false;
	d_preservecomments = true;
	d_preservewhitespace = true;
	d_outputstream = &std::cout;
}

AbstractWriter::~AbstractWriter()
{

}


void AbstractWriter::setOutputStream( std::ostream& stream)
{
	d_outputstream = &stream;
}

void AbstractWriter::setCommentChar(char c)
{
	d_commentchar = c;
}

void AbstractWriter::setDelimiter(char c)
{
	d_delimiter = c;
}

void AbstractWriter::preserveDeleted(bool doit)
{
	d_preservedeleted = doit;
}

void AbstractWriter::preserveComments(bool doit)
{
	d_preservecomments = doit;
}

void AbstractWriter::preserveWhiteSpace(bool doit)
{
	d_preservewhitespace = doit;
}

}} // end namespaces

