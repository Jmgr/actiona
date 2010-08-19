// ParserJuly2004.h
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

#ifndef INPUT_ParserJuly2004_h
#define INPUT_ParserJuly2004_h

#ifndef INPUT_AbstractParser_h
#include "AbstractParser.h"
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

class ParserJuly2004: public AbstractParser
{
	
protected:

	void stripTrailing(std::string&);
	inline bool isEOL(char c);
	inline bool chompEOL(std::istream& inputstream);
	
public:
	
	bool parse(std::istream& inputstream, AbstractOrganiser& organiser);

};

}} // end namespaces

#endif

