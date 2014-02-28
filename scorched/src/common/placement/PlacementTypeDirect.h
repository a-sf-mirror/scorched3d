////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2013
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
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
////////////////////////////////////////////////////////////////////////////////

#if !defined(__INCLUDE_PlacementTypeDirecth_INCLUDE__)
#define __INCLUDE_PlacementTypeDirecth_INCLUDE__

#include <placement/PlacementType.h>

class PlacementPosition : public XMLEntryContainer
{
public:
	PlacementPosition();
	virtual ~PlacementPosition();

	XMLEntryFixedVector position;
};

class PlacementPositionList : public XMLEntryList<PlacementPosition>
{
public:
	PlacementPositionList();
	virtual ~PlacementPositionList();

	PlacementPosition *createXMLEntry();
};

class PlacementTypeDirect : public PlacementType
{
public:
	PlacementTypeDirect();
	virtual ~PlacementTypeDirect();

	virtual void getPositions(ScorchedContext &context,
		RandomGenerator &generator,
		std::list<Position> &returnPositions,
		ProgressCounter *counter = 0);

protected:
	PlacementPositionList positions;
};

#endif // __INCLUDE_PlacementTypeDirecth_INCLUDE__
