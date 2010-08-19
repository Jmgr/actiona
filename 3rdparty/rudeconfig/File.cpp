// File.cpp
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


#include "File.h"

#ifndef INCLUDED_Section_H
#include "Section.h"
#endif

#ifndef INCLUDED_AbstractWriter_H
#include "AbstractWriter.h"
#endif

using namespace std;

namespace rude{
namespace config{

File::File()
{
	d_currentSection = new Section("", 0);
	d_sections.push_back(d_currentSection);
	std::string empty = "";
	d_sectionmap[empty] = d_currentSection;
}


File::~File()
{
	std::vector<Section*>::iterator iter;
	for(iter = d_sections.begin(); iter != d_sections.end(); iter++)
	{
		delete (*iter);
	}
}

void File::acceptWriter(AbstractWriter& writer) const
{
	writer.visitFile(*this);

	// All the data members are responsible for printing themselves out.
	// They are also responsible for knowing what to do when they are
	// marked as deleted and when commentchar / delimiter are 0
	//
	std::vector<Section*>::iterator iter;
	for(iter = d_sections.begin(); iter != d_sections.end(); iter++)
	{
		(*iter)->acceptWriter(writer);
	}
}
void File::clear()
{
	std::vector<Section*>::iterator iter;
	for(iter = d_sections.begin(); iter != d_sections.end(); iter++)
	{
		delete (*iter);
	}
	d_sections.clear();
	d_sectionmap.clear();


	// We create one to begin with, swo that we always
	// have a valid d_currentSection to work with.
	d_currentSection = new Section("", 0);
	d_sections.push_back(d_currentSection);
	std::string empty = "";
	d_sectionmap[empty] = d_currentSection;

}
const char * File::getStringValue(const char *name) const
{
	if(d_currentSection)
	{
		return d_currentSection->getValue(name);
	}
	return 0;
}

bool File::deleteData(const char *name)
{
	if(d_currentSection)
	{
		return d_currentSection->deleteData(name);
	}
	return false;
}

void File::setStringValue(const char *name, const char *value)
{
	if(d_currentSection)
	{
		d_currentSection->setValue(name, value);
	}
}

int File::getNumDataMembers() const
{
	if(d_currentSection)
	{
		return d_currentSection->getNumDataMembers();
	}
	return 0;
}

const char *File::getDataNameAt(int index) const
{
	if(d_currentSection)
	{
		return d_currentSection->getDataNameAt(index);
	}
	return 0;
}

const char *File::getDataValueAt(int index) const
{
	if(d_currentSection)
	{
		return d_currentSection->getDataValueAt(index);
	}
	return 0;
}


bool File::exists(const char *name) const
{
	if(d_currentSection)
	{
		return d_currentSection->exists(name);
	}
	return 0;
}

int File::getNumSections() const
{
	int len = d_sections.size();
	int count = 0;
	for(int x=0; x< len; x++)
	{
		Section *theSection = d_sections[x];
		if(!theSection->isDeleted())
		{
			count++;
		}
	}
	return count;
}

const char *File::getSectionNameAt(int index) const
{
	int len = d_sections.size();
	int position = 0;
	for(int x=0; x< len; x++)
	{
		Section *theSection = d_sections[x];
		if(!theSection->isDeleted())
		{
			if(position == index)
			{
				return theSection->getSectionName();
			}
			position++;
		}
	}
	return 0;
}

bool File::setSection(const char *sectionname, bool shouldCreate)
{
	if(sectionname)
	{
		std::string name=sectionname;
		Section *thesection = d_sectionmap[name];
		if(thesection)
		{
			if(!thesection->isDeleted())
			{
				d_currentSection = thesection;
				return true;
			}
			else
			{
				if(shouldCreate)
				{
					// revive the deleted section
					// 
					thesection->isDeleted(false);
					d_currentSection = thesection;
					return true;
				}
				return false;
			}
		}
		else
		{
			// The section could not be found.....
			// 
			if(shouldCreate)
			{
				d_currentSection = new Section(sectionname, 0);
				std::string stringname = sectionname;
				d_sections.push_back(d_currentSection);
				d_sectionmap[stringname] = d_currentSection;
				return true;
			}
		}
	}
	return false;
}

Section *File::getSection(const char *sectionname)
{
	setSection(sectionname, true);
	return d_currentSection;
}

bool File::deleteSection(const char *sectionname)
{
	if(sectionname)
	{
		std::string name=sectionname;
		Section *thesection = d_sectionmap[name];
		if(thesection)
		{
				thesection->isDeleted(true);
				// Here is the interesting part, 
				// By deleting the current section, we have effectively
				// deleted all of its data members...
				// but if we are deleting the current section, 
				// then we need to change the current section to
				// the default one - the "" section, 
				// and make sure that that section is undeleted....
				// because we always need a current section.
				if(thesection == d_currentSection)
				{
					return setSection("", true);
				}
				return true;

		}
		// The section could not be found.....
	}
	return false;	
}



}} // end namespace rude::config

