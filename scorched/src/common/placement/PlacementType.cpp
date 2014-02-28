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

#include <placement/PlacementType.h>
#include <placement/PlacementObject.h>
#include <placement/PlacementTypeMask.h>
#include <placement/PlacementTypeDirect.h>
#include <placement/PlacementTypeTree.h>
#include <placement/PlacementTypeCount.h>
#include <placement/PlacementTypeBounds.h>
#include <placement/PlacementTypeTankStart.h>
#include <landscapemap/LandscapeMaps.h>
#include <target/Target.h>
#include <target/TargetContainer.h>
#include <target/TargetLife.h>
#include <engine/ScorchedContext.h>
#include <common/DefinesString.h>
#include <XML/XMLParser.h>

PlacementType::PlacementType(const char *typeName, const char *description) : 
	XMLEntryContainer(typeName, description),
	placementobject(0)
{
}

PlacementType::~PlacementType()
{
	delete placementobject;
	placementobject = 0;
}

void PlacementType::createObjects(ScorchedContext &context,
	RandomGenerator &generator,
	unsigned int &playerId,
	ProgressCounter *counter)
{
	std::list<Position> returnPositions;
	getPositions(context, generator, returnPositions, counter);

	std::list<Position>::iterator itor;
	for (itor = returnPositions.begin();
		itor != returnPositions.end();
		++itor)
	{
		Position &position = *itor;
		placementobject->createObject(
			context, generator, playerId, position);
	}
}

bool PlacementType::checkCloseness(FixedVector &position,
	ScorchedContext &context,
	std::list<Position> &returnPositions,
	fixed mincloseness)
{
	// Check for border closeness for targets
	std::map<unsigned int, Target *> &targets = 
		context.getTargetContainer().getTargets();
	std::map<unsigned int, Target *>::iterator targetsitor;
	for (targetsitor = targets.begin();
		targetsitor != targets.end();
		++targetsitor)
	{
		Target *target = (*targetsitor).second;
		if (target->getType() == Target::TypeTank) continue;

		fixed distx = target->getLife().getTargetPosition()[0] - position[0];
		fixed disty = target->getLife().getTargetPosition()[1] - position[1];
		fixed closeness = mincloseness + target->getBorder();

		if (closeness > 0)
		{
			if (distx.abs() < closeness &&
				disty.abs() < closeness)
			{
				return false;
			}
		}
	}

	// Check for mincloseness closeness for objects in the current group
	if (mincloseness > 0)
	{
		std::list<Position>::iterator currentItor;
		for (currentItor = returnPositions.begin();
			currentItor != returnPositions.begin();
			++currentItor)
		{
			Position *object = &(*currentItor);

			fixed distx = object->position[0] - position[0];
			fixed disty = object->position[1] - position[1];

			if (distx.abs() < mincloseness &&
				disty.abs() < mincloseness)
			{
				return false;
			}	
		}
	}

	return true;
}

PlacementTypeChoice::PlacementTypeChoice() :
	XMLEntryTypeChoice<PlacementType>("PlacementTypeChoice", 
		"A choice of placement types that define the starting locations for objects placed on the landscape.")
{
}

PlacementTypeChoice::~PlacementTypeChoice()
{
}

PlacementType *PlacementTypeChoice::createXMLEntry(const std::string &type)
{
	if (0 == strcmp(type.c_str(), "trees")) return new PlacementTypeTree;
	if (0 == strcmp(type.c_str(), "mask")) return new PlacementTypeMask;
	if (0 == strcmp(type.c_str(), "direct")) return new PlacementTypeDirect;
	if (0 == strcmp(type.c_str(), "count")) return new PlacementTypeCount;
	if (0 == strcmp(type.c_str(), "bounds")) return new PlacementTypeBounds;
	if (0 == strcmp(type.c_str(), "tankstart")) return new PlacementTypeTankStart;
	S3D::dialogExit("PlacementType", S3D::formatStringBuffer("Unknown placement type %s", type));
	return 0;
}

PlacementTypeList::PlacementTypeList() :
	 XMLEntryList<PlacementTypeChoice>("PlacementTypeList",
		 "Defines the starting locations for objects placed on the landscape.")
{
}

PlacementTypeList::~PlacementTypeList()
{
}

PlacementTypeChoice *PlacementTypeList::createXMLEntry()
{
	return new PlacementTypeChoice();
}
