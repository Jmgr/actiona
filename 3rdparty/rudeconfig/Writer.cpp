// Writer.cpp
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

#include "Writer.h"

#ifndef INCLUDED_File_H
#include "File.h"
#endif

#ifndef INCLUDED_Section_H
#include "Section.h"
#endif

#ifndef INCLUDED_KeyValue_H
#include "KeyValue.h"
#endif

#ifndef INCLUDED_Comment_H
#include "Comment.h"
#endif

#ifndef INCLUDED_WhiteSpace_H
#include "WhiteSpace.h"
#endif

#ifndef INCLUDED_CSTDLIB
#include <cstdlib>
#define INCLUDED_CSTDLIB
#endif

#ifndef INCLUDED_CCTYPE
#include <cctype>
#define INCLUDED_CCTYPE
#endif

#ifndef INCLUDED_IOSTREAM
#include <iostream>
#define INCLUDED_IOSTREAM
#endif

#ifndef INCLUDED_STRING
#include <string>
#define INCLUDED_STRING
#endif

using namespace std;

namespace rude{
namespace config{


Writer::Writer()
{

}

Writer::~Writer()
{

}


void Writer::visitFile(const File& configfile) const
{

}

void Writer::visitSection(const Section& configsection) const
{
		std::string name = configsection.getSectionName();
		
		// if there is a section name, we print it out [between brackets]
		//
		if(name != "")
		{
			if( configsection.isDeleted() )
			{
				if ( d_commentchar && d_preservedeleted )
				{
					*d_outputstream << d_commentchar << " ";
				}
				else
				{
					return;
				}
			}
		
			int position = 0;
			size_t location;

			// escape all backslashes
			//
			while( (location = name.find("\\", position)) != string::npos )
			{
				// location points right at the '\'
				name.insert(location,  "\\");
				position = (int) location + 2;
			}

			// escape all ']''s
			//
			while( (location = name.find("]", position)) != string::npos )
			{
				// location points right at the ']'
				name.insert(location,  "\\");
				position = (int) location + 2;
			}
			
			*d_outputstream << "[" << name << "]";
			
			// If the section name has a comment, we print it out after the section name
			//
			if(configsection.getSectionComment()[0] != 0 && d_commentchar)
			{
				*d_outputstream << "\t" << d_commentchar << configsection.getSectionComment();
			}

			// newline ends the section header
			//
			*d_outputstream << "\n";
		}
}

void Writer::visitComment(const Comment& comment) const
{
	if(d_preservecomments && d_commentchar && ( !comment.isDeleted() || d_preservedeleted ))
	{
			*d_outputstream << d_commentchar << comment.getComment() << "\n";
	}
}

void Writer::visitWhiteSpace(const WhiteSpace& whitespace) const
{
	if(d_preservewhitespace && ( !whitespace.isDeleted() || d_preservedeleted ))
	{
			*d_outputstream << whitespace.getWhiteSpace();
	}
}


void Writer::visitKeyValue(const KeyValue& dataline) const
{
	if(dataline.isDeleted() && ( !d_commentchar || !d_preservedeleted))
	{
		// Don't preserve deleted data when comments are null
		// or we don't want to preserve them
		return;
	}
	else
	{
		string key = dataline.getName();
		string value = dataline.getValue();
		string comment = dataline.getComment();

		string commentchar(1,d_commentchar);
		

		if(dataline.isDeleted())
		{
			// print the comment character
			//
			*d_outputstream << d_commentchar << " ";
		}

		if(key != "")
		{


			int position = 0;
			size_t location;

			// escape all backslashes
			//
			while( (location = key.find("\\", position)) != string::npos )
			{
				// location points right at the '\'
				key.insert(location,  "\\");
				position = (int) location + 2;
			}


			// escape comment characters
			// unless the comment character is a backslash, which we've already escaped....
			//
			if(d_commentchar && d_commentchar != '\\')
			{
				position = 0;

				while( (location = key.find(commentchar, position)) != string::npos )
				{
					// location points right at the '"'
					key.insert(location,  "\\");
					position = (int) location + 2;
				}
			}

			// escape all delimiters, unless the delimiter is a backslash or the same as the comment, god forbid.
			//
			//
			if(d_delimiter != '\\' && d_delimiter != d_commentchar)
			{
				if(d_delimiter)
				{
					string delimiter(1, d_delimiter);
					position = 0;

					while( (location = key.find(delimiter, position)) != string::npos )
					{
						// location points right at the '"'
						key.insert(location,  "\\");
						position = (int) location + 2;
					}
				}
				else
				{
					position = 0;

					while( (location = key.find("\t", position)) != string::npos )
					{
						// location points right at the '"'
						key.insert(location,  "\\");
						position = (int) location + 2;
					}
					
					position = 0;

					while( (location = key.find(" ", position)) != string::npos )
					{
						// location points right at the '"'
						key.insert(location,  "\\");
						position = (int) location + 2;
					}					

				}				
			}

			// print the key
			//
			*d_outputstream << key;
		}

		if(value != "")
		{
			// print out the delimiter
			//
			*d_outputstream  << " " << ( d_delimiter ? d_delimiter : '\t' ) << " ";

			int position = 0;
			size_t location;

			// escape all backslashes
			//
			string backslash(1,'\\');

			while( (location = value.find("\\", position)) != string::npos )
			{
				// location points right at the '\'
				value.insert(location,  "\\");
				position = (int) location + 2;
			}

			// escape all quotes
			//
			string quote(1,'"');
			position = 0;

			while( (location = value.find("\"", position)) != string::npos )
			{
				// location points right at the '"'
				value.insert(location,  "\\");
				position = (int) location + 2;
			}

			// escape comment characters
			// unless the comment character is a backslash or a quote, which we've already escaped....
			//
			if(d_commentchar && d_commentchar != '\\' && d_commentchar != '"')
			{
				position = 0;

				while( (location = value.find(commentchar, position)) != string::npos )
				{
					// location points right at the '"'
					value.insert(location,  "\\");
					position = (int) location + 2;
				}
			}





			// if value starts with whitespace, ends with whitespace or contains the comment character or CRLF's, quote the value, 
			//
			int size = value.size();

			if(		isspace(value[0]) || 
						isspace(value[size-1]) || 
						(value.find("\r", 0) != string::npos) || 
						(value.find("\f", 0) != string::npos) || 
						(value.find("\n", 0)!= string::npos) 
			)
			{


				value.insert(0, "\"");
				value += "\"";
			}

			// if data is deleted and there are CRLFS
			// then each line must start with a comment character.
			// The safest way is just to follow every newline character with a comment
			//
			if(		dataline.isDeleted() && ( 
						(value.find("\r", 0) != string::npos) || 
						(value.find("\f", 0) != string::npos) || 
						(value.find("\n", 0)!= string::npos) )
			  )
			{	
				position = 0;				

				while( (location = value.find("\r", position)) != string::npos )
				{
					// location points right at the '"'
					value.insert(location + 1,  commentchar);
					position = (int) location + 2;
				}
				position = 0;				
				while( (location = value.find("\f", position)) != string::npos )
				{
					// location points right at the '"'
					value.insert(location + 1,  commentchar);
					position = (int) location + 2;
				}

				position = 0;				
				while( (location = value.find("\n", position)) != string::npos )
				{
					// location points right at the '"'
					value.insert(location + 1,  commentchar);
					position = (int) location + 2;
				}
			}			

			*d_outputstream << value;

		}

		if( comment != "" && d_commentchar && d_preservecomments)
		{
			// add value's comment at end if it exists
			// and there is a comment character
			//
					*d_outputstream << "\t " << d_commentchar << comment;
		}

		// end the entry with a newline
		//
		*d_outputstream << "\n";
	}
}


}} // end namespaces

