// WhiteSpace.cpp
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


#include "WhiteSpace.h"

#ifndef INCLUDED_AbstractWriter_H
#include "AbstractWriter.h"
#endif

#ifndef INCLUDED_CSTDIO
#include <cstdio>
#define INCLUDED_CSTDIO
#endif

using namespace std;
namespace rude{
namespace config{

WhiteSpace::WhiteSpace(const char *whitespace)
{
	d_whitespace = whitespace ? whitespace : "";
}

void WhiteSpace::acceptWriter(AbstractWriter& writer) const
{
	writer.visitWhiteSpace(*this);
}

const char *WhiteSpace::getWhiteSpace() const
{
	return d_whitespace.c_str();
}

}} // end namespace rude::config

