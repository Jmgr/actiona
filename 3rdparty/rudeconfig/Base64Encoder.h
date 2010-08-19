// Base64Encoder.h
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

#ifndef INCLUDED_BASE64ENCODER_H
#define INCLUDED_BASE64ENCODER_H


namespace rude{
namespace config{

class Base64Encoder{

private:
      static char c_encode(char uc);
      static unsigned char c_decode(char c);
      static bool IsBase64(char c);

public:

		// datalength does not need to include the NULL terminator for strings
		//
		// a NULL terminator is appended to result to make it string friendly
		// but outlength does not include the
		// appended NULL terminator in length calculation
		//
		// CALLER RESPONSIBLE FOR DELETING RETURNED char * if it is not NULL.
		//
		static char * encode(const char *data, int datalength, int &outlength);


		// datalength does not need to include the NULL terminator for strings
		// NULL Terminator is appended to result, but outlength does not include the
		// appended NULL terminator in length calculation
		//
		// CALLER RESPONSIBLE FOR DELETING RETURNED char * if it is not NULL.
		//
		static char * decode(const char *data, int datalength, int &outlength);

};
}}

#endif


