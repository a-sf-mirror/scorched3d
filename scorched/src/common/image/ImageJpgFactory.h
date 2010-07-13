////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2009
//
//    This file is part of Scorched3D.
//
//    Scorched3D is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    Scorched3D is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with Scorched3D; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ImageJpgFactory_H__315BF771_5E56_4C78_9D9F_51608D8CB3F0__INCLUDED_)
#define AFX_ImageJpgFactory_H__315BF771_5E56_4C78_9D9F_51608D8CB3F0__INCLUDED_

#include <image/Image.h>
#include <net/NetBuffer.h>

class ImageJpgFactory
{
public:

	/**
	Overwrite the bitmap with the contents of the given file.
	If alpha is true then any black areas in the bitmap have full
	opacity.  When creating an alpha bitmap the bitmap is created
	in RGBA format, otherwise in RGB format.
	*/
	static Image loadFromFile(const char *filename, bool readalpha = false);
	static Image loadFromFile(const char * filename, const char *alphafilename, bool invert);
	static Image loadFromBuffer(NetBuffer &buffer, bool readalpha);
};

#endif // !defined(AFX_ImageJpgFactory_H__315BF771_5E56_4C78_9D9F_51608D8CB3F0__INCLUDED_)