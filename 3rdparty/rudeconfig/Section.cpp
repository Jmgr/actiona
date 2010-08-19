// Section.cpp
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

#include "Section.h"

#ifndef INCLUDED_RUDE_DataLine_H
#include "DataLine.h"
#endif

#ifndef INCLUDED_KeyValue_H
#include "KeyValue.h"
#endif

#ifndef INCLUDED_WhiteSpace_H
#include "WhiteSpace.h"
#endif

#ifndef INCLUDED_Comment_H
#include "Comment.h"
#endif

#ifndef INCLUDED_AbstractWriter_H
#include "AbstractWriter.h"
#endif

using namespace rude::config;

namespace rude{
namespace config{

Section::Section(const char *sectionname, const char *sectioncomment)
{
	d_isDeleted = false;
	d_sectionname = sectionname ? sectionname: "";
	d_sectioncomment = sectioncomment ? sectioncomment: "";
}

Section::~Section()
{
	std::vector<DataLine*>::iterator iter;
	for(iter = d_allDataVector.begin(); iter != d_allDataVector.end(); iter++)
	{
			delete (*iter);
	}
}

void Section::acceptWriter(AbstractWriter& writer) const
{
	writer.visitSection(*this);

	// Send the writer to all the children.
	// The writer will know what to do.
	//
	std::vector<DataLine*>::iterator iter;
	for(iter = d_allDataVector.begin(); iter != d_allDataVector.end(); iter++)
	{
		(*iter)->acceptWriter(writer);
	}
}

const char *Section::getSectionName() const
{
	return d_sectionname.c_str();
}

const char *Section::getSectionComment() const
{
	return d_sectioncomment.c_str();
}

void Section::setSectionComment(const char * newcomment)
{
	d_sectioncomment = newcomment ? newcomment : "";
}

bool Section::isDeleted() const
{
	return d_isDeleted;
}

void Section::isDeleted(bool is_it_or_not)
{
	d_isDeleted = is_it_or_not;
	if(d_isDeleted)
	{
		// Delete all data members
		//
		std::vector<DataLine*>::iterator iter;
		for(iter = d_allDataVector.begin(); iter != d_allDataVector.end(); iter++)
		{
			(*iter)->isDeleted(true);
		}
		d_kv_vector.clear();
		d_kv_map.clear();
	}
}

int Section::getNumDataMembers() const
{
	
	return d_kv_vector.size();
}

const char *Section::getDataNameAt(int index) const
{
	KeyValue *kv = d_kv_vector[index];
	if(kv)
	{
		return kv->getName();
	}
	return "";
}

const char *Section::getDataValueAt(int index) const
{
	KeyValue *kv = d_kv_vector[index];
	if(kv)
	{
		return kv->getValue();
	}
	return "";
}

// MAPPED
//
bool Section::exists(const char *name) const
{
	if(name)
	{
		std::string myname = name;
		KeyValue *mydata = d_kv_map[myname];
		if(mydata)
		{
			return true;
		}	
	}
	return false;
}

// MAPPED
//
const char * Section::getValue(const char *name) const
{
	if(name)
	{
		std::string myname=name;
		KeyValue *mydata = d_kv_map[myname];
		if(mydata)
		{
				return mydata->getValue();
		}
	}
	return "";
}

// MAPPED
//
void Section::setValue(const char *name, const char *value)
{
	if(name)
	{
		std::string myname=name;
		KeyValue *mydata = d_kv_map[myname];
		if(mydata)
		{
			mydata->setValue(value);
			mydata->isDeleted(false);
			
		} 
		else
		{	
			KeyValue *newdata = new KeyValue();
			newdata->setName(name);
			newdata->setValue(value);
			d_allDataVector.push_back(newdata);
			d_kv_vector.push_back(newdata);
			d_kv_map[myname] = newdata;
		}
	}
}

// MAPPED
//
const char * Section::getComment(const char *name) const
{
	if(name)
	{
		std::string myname=name;
		KeyValue *mydata = d_kv_map[myname];
		if(mydata)
		{
			return mydata->getComment();
		}
	}
	return "";
}

// MAPPED
//
void Section::setComment(const char *name, const char *comment)
{
	if(name)
	{
		std::string myname=name;
		KeyValue *mydata = d_kv_map[myname];
		if(mydata)
		{
			mydata->setComment(comment);
		} 
	}
}

// MAPPED
//
void Section::setValue(const char *name, const char *value, const char *comment)
{
	
	if(name)
	{
		std::string myname=name;
		KeyValue *mydata = d_kv_map[myname];
		if(mydata)
		{
			mydata->setValue(value);
			mydata->setComment(comment);
			mydata->isDeleted(false);
		} 
		else
		{	
			KeyValue *newdata = new KeyValue(name, value, comment);

			d_allDataVector.push_back(newdata);
			d_kv_vector.push_back(newdata);
			d_kv_map[myname] = newdata;
		}
	}
}

// MAPPED
//
void Section::addComment(const char *comment)
{
	Comment *newdata = new Comment(comment);
	d_allDataVector.push_back(newdata);
}

// MAPPED
//
void Section::addWhiteSpace(const char *whitespace)
{
	WhiteSpace *newdata = new WhiteSpace(whitespace);
	d_allDataVector.push_back(newdata);
}

// MAPPED
//
bool Section::deleteData(const char *name)
{
	// Since we are keeping deleted data in the d_allDataVector,
	// multiple set/deletes will require lots of memory!!
	//
	if(name)
	{
		std::string myname=name;
		KeyValue *mydata = d_kv_map[myname];
		if(mydata)
		{
			// Remain in d_allDataVector, but as deleted
			//
			mydata->isDeleted(true);

			// remove from keyvalue vector and map
			//
			std::vector<KeyValue*>::iterator iter;
			for(iter = d_kv_vector.begin(); iter != d_kv_vector.end(); iter ++)
			{
				if( *iter == mydata )
				{
					d_kv_vector.erase(iter);
					break;
				}
			}
			
			d_kv_map.erase(myname);

			return true;
		}
	}
	return false;
}

}} // end namespace rude::config


