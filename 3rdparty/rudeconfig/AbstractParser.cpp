// AbstractParser.cc
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

#include "AbstractParser.h"

using namespace rude::config;
using namespace std;

namespace rude{
namespace config{


AbstractParser::AbstractParser()
{
	d_error = "";
	d_errorcode = "";
	d_commentchar = '#';
	d_delimiter = '=';
}

AbstractParser::~AbstractParser()
{

}

void AbstractParser::setCommentCharacter(char c)
{
	d_commentchar = c;
}

void AbstractParser::setDelimiter(char c)
{
	d_delimiter = c;
}

const char *AbstractParser::getErrorCode() const
{
	return d_errorcode.c_str();
}

const char *AbstractParser::getError() const
{
	return d_error.c_str();
}

void AbstractParser::setError(const char *code, const char *message)
{
	d_errorcode = code ? code : "";
	d_error = message ? message : "";
}

}} // end namespaces




