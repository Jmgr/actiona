// AbstractData.h
// Acts as the base Element that accepts visitors in the Visitor design pattern
//
// This file is part of RudeConfig.
//
// Copyright (C) 2000, 2001, 2002, 2003, 2004, 2005 Matthew Flood
// See file AUTHORS for contact information
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

#ifndef INCLUDED_AbstractData_H
#define INCLUDED_AbstractData_H

namespace rude{
namespace config{

class AbstractWriter;

class AbstractData{

protected:
	AbstractData();

public:
	virtual ~AbstractData();
	virtual void acceptWriter(AbstractWriter& writer) const = 0;


};


}} // end namespaces

#endif

