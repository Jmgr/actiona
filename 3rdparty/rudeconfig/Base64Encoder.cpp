// Base64Encoder.cpp
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

#include "Base64Encoder.h"

#ifndef INCLUDED_CSTRING
#include <string.h>
#define INCLUDED_CSTRING
#endif

namespace rude{
namespace config{

// translates a 6 bit char into a base-64 character
//
inline char Base64Encoder::c_encode(char uc)
{
	if (uc < 26)
	{
		return 'A'+uc;
	}
	if (uc < 52)
	{
		return 'a'+(uc-26);
	}
    if (uc < 62)
	{
		return '0'+(uc-52);
	}
	if (uc == 62)
	{
		return '+';
	}
	return '/';
}

// translates one base-64 character into a six-bit pattern
//
inline unsigned char Base64Encoder::c_decode(char c)
{
      if (c >= 'A' && c <= 'Z')
      {
            return c - 'A';
      }
      if (c >= 'a' && c <= 'z')
      {
            return c - 'a' + 26;
      }
      if (c >= '0' && c <= '9')
      {
            return c - '0' + 52;
      }
      if (c == '+')
      {
            return 62;
      };
      return 63;
}


char * Base64Encoder::encode(const char *data, int datalength, int &outlength)
{
      
	
      int linelength=0;
      outlength=0;
 
      if (data == (char*) 0 || datalength == 0)
      {
            return (char*) 0;
      };

		char *retval=new char[datalength * 2];
		const char *crlf="\n";
		int crlflength=strlen(crlf);
		int maxlinelength=76;
		
      for (int i=0; i<datalength; i+=3)
      {
				unsigned char by1, by2, by3, by4, by5, by6, by7;
            by1 = data[i];
				
            if (i+1 < datalength)
            {
                  by2 = data[i+1];
            }
				else
				{
	            by2=0;
				}
				
            if (i+2 < datalength)
            {
                  by3 = data[i+2];
            }
				else
				{
					by3=0;
				}

            by4 = by1>>2;
            by5 = ((by1 & 0x3)<<4)|( by2>>4 );
            by6 = ((by2 & 0xf)<<2)|( by3>>6 );
            by7 = by3 & 0x3f;



            retval[outlength++] = c_encode(by4);
            linelength++;
				
            retval[outlength++] = c_encode(by5);
            linelength++;
 
            if (i+1 < datalength)
            {
                  retval[outlength++]= c_encode(by6);
                  linelength++;
            }
            else
            {
                  retval[outlength++]= '=';
                  linelength++;
            }

            if (i+2<datalength)
            {
                  retval[outlength++]= c_encode(by7);
                  linelength++;
            }
            else
            {
                  retval[outlength++]= '=';
                  linelength++;
            };

				// only set linelength if maxlinelength > 0
				//
            if (maxlinelength != 0 && linelength >= maxlinelength)
            {
						for(int y=0; y< crlflength; y++)
						{
							retval[outlength++]= crlf[y];
						}
						
						// we just created a line - reset current linelength
						//
                  linelength=0;
            }
      };
      retval[outlength]=(char) NULL;
      return retval;
}


char * Base64Encoder::decode(const char *data, int datalength, int &outlength)
{
		// Error if incoming data is NULL
		// Error if nothing to decode
		//
		if(data == (char *) 0 || datalength == 0)
		{
			return (char*) 0;
		}

	
      outlength=0;

		// rawlength is the length of the encoded data excluding
		// any non-base64 characters
		//
      int rawlength=0;


		// create return buffer
		//
		char *outbuffer=new char[datalength];

      // copy all base64 characters into outbuffer,
      // in other words, strip away CRLF's and non-b64 characters...
		//
      for (int j=0;j < datalength; j++)
      {
            if (IsBase64(data[j]))
            {
                  outbuffer[rawlength++]= data[j];
            }
      }

		// there's no base64 characters to decode
		//
      if (rawlength == 0 || datalength == 0)
      {
				delete [] outbuffer;
            return (char*) NULL;
      }

 

      for (int i=0; i<rawlength; i+=4)

      {

            

            char c1 = outbuffer[i];
				char c2='A',c3='A',c4='A';

            if (i+1 < rawlength)

            {

                  c2 = outbuffer[i+1];

            };

            if (i+2 < rawlength)

            {

                  c3 = outbuffer[i+2];

            };

            if (i+3 < rawlength)

            {

                  c4 = outbuffer[i+3];

            };

 

            unsigned char by1,by2,by3,by4;

            by1 = c_decode(c1);

            by2 = c_decode(c2);

            by3 = c_decode(c3);

            by4 = c_decode(c4);

 

            outbuffer[outlength]=( (by1<<2)|(by2>>4) );
            outlength++;
 

            if (c3 != '=')

            {

                  outbuffer[outlength]=( ((by2&0xf)<<4)|(by3>>2) );
                  outlength++;
            }

 

            if (c4 != '=')

            {

                  outbuffer[outlength]=( ((by3&0x3)<<6)|by4 );
                  outlength++;
            };

      };
		// NULL terminate decoded data
		// in case caller ignores outlength
		// for text data
		//
      outbuffer[outlength]=(char) 0;
      return outbuffer;
}

//The last helper function returns true is a character is a valid
//base-64 character and false otherwise.
inline bool Base64Encoder::IsBase64(char c)
{
      if (c >= 'A' && c <= 'Z')
      {
            return true;
      }
      if (c >= 'a' && c <= 'z')
      {
            return true;
      }
      if (c >= '0' && c <= '9')
      {
            return true;
      }
      if (c == '+')
      {
            return true;
      };
      if (c == '/')
      {
            return true;
      };
      if (c == '=')
      {
            return true;
      };
      return false;
}

}}

