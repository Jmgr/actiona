// DataLine.h
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

#ifndef INCLUDED_DataLine_H
#define INCLUDED_DataLine_H

#ifndef INCLUDED_AbstractData_H
#include "AbstractData.h"
#endif

namespace rude{
namespace config{

class DataLine: public AbstractData{

	bool d_isDeleted;
	
protected:

	DataLine();

public:
	
	void acceptWriter(AbstractWriter& writer) const = 0;

	//=
	// Returns true if the data member has been deleted
	//=
	bool isDeleted() const;

	//=
	// Sets / unsets the deleted status of the data member
	//=
	void isDeleted(bool isit);

	virtual ~DataLine();
	
};

}} // end namespace rude::config

#endif

