// AbstractParser.h
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

#ifndef INPUT_AbstractParser_h
#define INPUT_AbstractParser_h

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

class ConfigInput;
class AbstractOrganiser;


class AbstractParser
{
	std::string d_error;
	std::string d_errorcode;
	
protected:

	char d_commentchar;
	char d_delimiter;
	void setError(const char *code, const char *message);

public:

	AbstractParser();
	virtual ~AbstractParser();

	void setCommentCharacter(char c);
	void setDelimiter(char c);

	const char *getErrorCode() const;
	const char *getError() const;
	
	virtual bool parse(std::istream& inputstream, AbstractOrganiser& organiser) = 0;

};

}} // end namespaces

#endif

