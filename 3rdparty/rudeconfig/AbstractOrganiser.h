// AbstractOrganiser.h
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

#ifndef INCLUDED_AbstractOrganiser_h
#define INCLUDED_AbstractOrganiser_h

namespace rude{
namespace config{

class AbstractOrganiser
{

public:

	AbstractOrganiser();
	virtual ~AbstractOrganiser();

	virtual void foundSection(const char *sectionName, const char *comment) = 0;
	virtual void foundComment(const char *comment) = 0;
	virtual void foundWhiteSpace(const char *whitespace) = 0;
	virtual void foundData(const char *key, const char *value, const char *comment) = 0;
	

};

}} // end namespaces

#endif

