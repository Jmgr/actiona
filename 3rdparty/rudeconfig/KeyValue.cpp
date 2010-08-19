// KeyValue.cpp
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


#include "KeyValue.h"

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

KeyValue::KeyValue()
{
	d_name = "";
	d_value = "";
	d_comment = "";
}

KeyValue::KeyValue(const char *name, const char *value, const char *comment)
{
	d_name = name ? name : "";
	d_value = value ? value : "";
	d_comment = comment ? comment : "";
}

void KeyValue::acceptWriter(AbstractWriter& writer) const
{
	writer.visitKeyValue(*this);
}

const char *KeyValue::getName() const
{
	return d_name.c_str();
}

const char *KeyValue::getValue() const
{
	return d_value.c_str();
}

const char *KeyValue::getComment() const
{
	return d_comment.c_str();
}

void KeyValue::setName(const char *name)
{
	d_name = name ? name : "";
}

void KeyValue::setValue(const char *value)
{
	d_value = value ? value : "";
}

void KeyValue::setComment(const char *comment)
{
	d_comment = comment ? comment : "";
}

KeyValue::~KeyValue()
{

}
}} // end namespace rude::config

