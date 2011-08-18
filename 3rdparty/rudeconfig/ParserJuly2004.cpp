// ParserJuly2004.cc
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

/*
 Edited on 19/08/2010 by Jonathan Mercier-Ganady for Actionaz
*/

#include "ParserJuly2004.h"

#ifndef INCLUDED_AbstractOrganiser_h
#include "AbstractOrganiser.h"
#endif

#ifndef INCLUDED_CCTYPE
#include <cctype>
#define INCLUDED_CCTYPE
#endif

#ifndef INCLUDED_IOSTREAM
#include <iostream>
#define INCLUDED_IOSTREAM
#endif

#ifndef INCLUDED_CSTDLIB
#include <cstdlib>
#define INCLUDED_CSTDLIB
#endif

#ifndef INCLUDED_CSTDIO
#include <cstdio>
#define INCLUDED_CSTDIO
#endif

#ifndef INCLUDED_LOCALE
#include <locale>
#define INCLUDED_LOCALE
#endif

using namespace rude::config;
using namespace std;

namespace rude{
namespace config{

enum SectionState{ STARTSECTION, SECTIONID, ESCAPEID, ENDSECTIONID, SECTIONCOMMENT, FOUNDIDONLY, FOUNDIDCOMMENT, SECTIONERROR, ENDSECTION };
enum KeyValueState{ KEY, KEYESCAPE, STARTVALUE, COMMENT, FINDCOMMENT, KVERROR, ENDKV, VALUE, QUOTEVALUE, NONQUOTEVALUE, QUOTEESCAPE, NONQUOTEESCAPE, ENDKEYVALUE};

void ParserJuly2004::stripTrailing(std::string& buffer)
{
	size_t bufferLength = buffer.size();

	locale loc;

	for (size_t x = bufferLength - 1; x >= 0; x--)
	{
		char c = buffer[x];

		if (isspace(c, loc))
		{
			buffer.erase(x);
		}
		else
		{
			break;
		}
	}
}


bool ParserJuly2004::isEOL(char c)
{
	return (c == '\r' || c == '\f' || c == '\n');
}

bool ParserJuly2004::chompEOL(std::istream& inputstream)
{
		char c = inputstream.peek();
		if(isEOL(c))
		{
			inputstream.get();
			char next_c = inputstream.peek();
			if( (c != next_c) && isEOL(next_c) )
			{
				inputstream.get();
			}
		}
		return true;
}

bool ParserJuly2004::parse(std::istream& infile, AbstractOrganiser& organiser)
{
	locale loc;

	if (d_delimiter == '\\' || isEOL(d_delimiter) || d_delimiter == d_commentchar || d_delimiter == '[')
	{
		setError("110", "Illegal delimiter.");
		return false;
	}

	if (d_commentchar == '\\' || d_commentchar == '"' || isspace(d_commentchar, loc))
	{
		setError("111", "Illegal comment character.");
		return false;
	}

	register char c;

	// eof only gets set when error_flag is set on previous operation
	// as such, you need to peek() at the end ot the while loop
	// in order for eof to happen when you want it to!!
	// one peek() triggers infile.eof(), but it does not reveal it!!!!
	// you gotta call peek() twice.
	//
	while( (c = infile.peek()) != EOF)
	{
		// We're looking for:
		// '[' beggining of section
		// '#' (d_commentchar) comment character
		// any non-whitespace character

		if(isspace(c, loc))
		{
			std::string whitespace = "";

			while(c != EOF && isspace(c, loc))
			{
				whitespace += infile.get();
				c = infile.peek();
			}
			organiser.foundWhiteSpace(whitespace.c_str());
		}
		else if(c == '[')
		{
			// discard '[' character
			//
			infile.get();

			register SectionState sectionState = STARTSECTION;

			std::string sectionID = "";
			std::string comment = "";

			while (sectionState != ENDSECTION)
			{
				switch (sectionState)
				{
					case STARTSECTION:
					{
							c = infile.peek();
							if(c == EOF)
							{
								setError("102", "End of stream found before section ID");
								sectionState = SECTIONERROR;
							}
							else if(isEOL(c))
							{
								setError("101", "End of line found before section ID");
								sectionState = SECTIONERROR;
							}
							else if(c == ' ' || c == '\t')
							{
								// discard whitespace
								//
								infile.get();

								// LOOP
							}
							else if(c == ']')
							{
								// discard ']'
								//
								infile.get();

								sectionState = ENDSECTIONID;
							}
							else
							{
								sectionState = SECTIONID;
							}
							break;
					}
					case SECTIONID:
					{
							c = infile.peek();
							if(c == EOF)
							{
								setError("104", "End of stream found before end-of-section marker");
								sectionState = SECTIONERROR;
							}
							else if(isEOL(c))
							{
								setError("103", "End of line found before end-of-section marker");
								sectionState = SECTIONERROR;
							}
							else if(c == '\\')
							{
								// discard backslash
								//
								infile.get();

								sectionState = ESCAPEID;
							}
							else if(c == ']')
							{
								// discard ']'
								//
								infile.get();

								// Strip Trailing Whitespace from ID
								//
								stripTrailing(sectionID);

								sectionState = ENDSECTIONID;
							}
							else
							{
								sectionID += infile.get();

								// LOOP
							}
							break;
					}
					case ESCAPEID:
					{
							c = infile.peek();
							if(c == EOF)
							{
								setError("107", "End of stream found after un-escaped backslash");
								sectionState = SECTIONERROR;
							}
							else if(isEOL(c))
							{
								setError("108", "Escaped new-line is not allowed in section ID or key");
								sectionState = SECTIONERROR;
							}
							else
							{
								sectionID += infile.get();
								sectionState = SECTIONID;
							}
							break;
					}
					case ENDSECTIONID:
					{
							c = infile.peek();
							if(c == EOF || isEOL(c))
							{
								sectionState = FOUNDIDONLY;
							}
							else if(d_commentchar != 0 && c == d_commentchar)
							{
								// discard '#'
								//
								infile.get();

								sectionState = SECTIONCOMMENT;
							}
							else if(c == ' ' || c == '\t')
							{
								// discard whitespace
								//
								infile.get();

								// LOOP
							}
							else
							{
								setError("105", "Illegal character found after end-of-section marker");
								sectionState = SECTIONERROR;
							}
							break;
					}
					case SECTIONCOMMENT:
					{
							c = infile.peek();
							if(c == EOF || isEOL(c))
							{
								sectionState = FOUNDIDCOMMENT;
								stripTrailing(comment);
							}
							else
							{
								// append to comment
								//
								comment += infile.get();

								// LOOP
							}
							break;
					}
					case SECTIONERROR:
					{
						return false;
					}
					case FOUNDIDONLY:
					{
						organiser.foundSection(sectionID.c_str(), 0);
						chompEOL(infile);
						sectionState = ENDSECTION;
						break;
					}
					case FOUNDIDCOMMENT:
					{
						organiser.foundSection(sectionID.c_str(), comment.c_str());
						chompEOL(infile);
						sectionState = ENDSECTION;
						break;
					}
				default:
					break;
				}
			}
		}
		else if(c == d_commentchar)
		{
			// found a comment line
			// discard the comment character
			//
			infile.get();

			// put the rest of the line into a string
			//
			std::string line="";
			while(infile.good())
			{
				c=infile.get();
				if( isEOL(c))
				{
					break;
				}
				line += c;
			}

			chompEOL(infile);

			// PROCESS THE COMMENT LINE
			//
			stripTrailing(line);
			organiser.foundComment(line.c_str());
		}
		else
		{
			register KeyValueState kvState = KEY;
			std::string key = "";
			std::string value = "";
			std::string comment = "";

			while (kvState != ENDKEYVALUE)
			{

				switch (kvState)
				{
					case KEY:
					{
							char c = infile.peek();

							if(c == EOF || isEOL(c))
							{
								kvState = ENDKV;
							}
							else if(d_delimiter && c == d_delimiter)
							{
								// discard '='
								//
								infile.get();

								kvState = STARTVALUE;
							}
							else if(!d_delimiter && (c == ' ' || c == '\t'))
							{
								// discard whitespace
								//
								infile.get();
								kvState = STARTVALUE;
							}
							else if(d_commentchar != 0 && c == d_commentchar)
							{
								// discard '#'
								//
								infile.get();

								kvState = COMMENT;
							}
							else if(c == '\\')
							{
								// discard '\\'
								//
								infile.get();

								kvState = KEYESCAPE;
							}
							else
							{
								// append to key
								//
								key += infile.get();

								// LOOP
							}
							break;
					}
					case KEYESCAPE:
					{
							char c = infile.peek();
							if(c == EOF)
							{
								setError("107", "End of stream found after un-escaped backslash");
								kvState = KVERROR;
							}
							else if(isEOL(c))
							{
								setError("108", "Escaped new-line is not allowed in key");
								kvState = KVERROR;
							}
							else
							{
								// append to key
								//
								key += infile.get();

								kvState = KEY;
							}
							break;
					}
					case STARTVALUE:
					{
							char c = infile.peek();

							if(c == EOF || isEOL(c))
							{
								kvState = ENDKV;
							}
							else if(d_commentchar != 0 && c == d_commentchar)
							{
								// discard '#'
								//
								infile.get();

								kvState = COMMENT;
							}
							else if(c == ' ' || c == '\t')
							{
								// discard whitespace
								//
								infile.get();

								// LOOP
							}
							else
							{
								kvState = VALUE;
							}
							break;
					}
					case KVERROR:
					{
							return false;
					}
					case FINDCOMMENT:
					{
							char c = infile.peek();

							if(c == EOF || isEOL(c))
							{
								kvState = ENDKV;
							}
							else if(d_commentchar != 0 && c == d_commentchar)
							{
								// discard '#'
								//
								infile.get();

								kvState = COMMENT;
							}
							else if(c == ' ' || c == '\t')
							{
								// discard whitespace
								//
								infile.get();

								// LOOP
							}
							else
							{
								setError("109", "Illegal Character Found after quoted value.");
								kvState = KVERROR;
							}
							break;
					}
					case COMMENT:
					{
							char c = infile.peek();

							if(c == EOF || isEOL(c))
							{
								stripTrailing(comment);
								kvState = ENDKV;
							}
							else
							{
								// Append to comment
								//
								comment += infile.get();

								// LOOP
							}
							break;
					}
					case VALUE:
					{
						char c = infile.peek();
						if(c == '"')
						{
							// discard '"'
							//
							infile.get();

							kvState = QUOTEVALUE;
						}
						else
						{
							kvState = NONQUOTEVALUE;
						}
						break;
					}
					case QUOTEVALUE:
					{
							char c = infile.peek();
							if(c == EOF)
							{
								setError("106", "End of stream found before final quote (\") in value");
								kvState = KVERROR;
							}
							else if(c == '"')
							{
								// discard '"'
								//
								infile.get();

								kvState = FINDCOMMENT;
							}
							else if(c == '\\')
							{
								// discard backslash
								//
								infile.get();

								kvState = QUOTEESCAPE;
							}
							else
							{
								// append to value
								//
								value += infile.get();

								// LOOP
							}
							break;
					}
					case QUOTEESCAPE:
					{
							char c = infile.peek();
							if(c == EOF)
							{
								setError("107", "End of stream found after un-escaped backslash");
								kvState = KVERROR;
							}
							else
							{
								// append to value
								//
								value += infile.get();

								kvState = QUOTEVALUE;
							}
							break;
					}
					case NONQUOTEVALUE:
					{
							char c = infile.peek();

							if(c == EOF || isEOL(c))
							{
								stripTrailing(value);

								kvState = ENDKV;
							}
							else if(d_commentchar != 0 && c == d_commentchar)
							{
								// discard '#'
								//
								infile.get();

								stripTrailing(value);

								kvState = COMMENT;
							}
							else if(c == '\\')
							{
								// discard backslash
								//
								infile.get();

								kvState = NONQUOTEESCAPE;
							}
							else
							{
								// append to value
								//
								value += infile.get();

								// LOOP
							}
							break;
					}
					case NONQUOTEESCAPE:
					{
							char c = infile.peek();
							if(c == EOF)
							{
								setError("107", "End of stream found after un-escaped backslash");
								kvState = KVERROR;
							}
							else
							{
								// append to value
								//
								c = infile.get();
								value += c;

								// SPECIAL CASE FOR ESCAPED CRLFs:
								//
								// if c is newline and next character is also newline,
								// we keep both of them if they are different forms of newline
								//
								char next_c = infile.peek();
								if(isEOL(c) && isEOL(next_c) && (c != next_c))
								{
									value += infile.get();
								}
								kvState = NONQUOTEVALUE;
							}
							break;
					}
					case ENDKV:
					{
							chompEOL(infile);

							stripTrailing(key);
							organiser.foundData(key.c_str(), value.c_str(), comment.c_str());

							kvState = ENDKEYVALUE;
							break;
					}
					default:
					break;
				} // end switch
			} // end while
		}
	}
	return true;
}

}} // end namespaces




