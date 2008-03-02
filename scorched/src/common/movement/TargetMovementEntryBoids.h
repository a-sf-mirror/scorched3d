////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2004
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

#if !defined(__INCLUDE_TargetMovementEntryBoidsh_INCLUDE__)
#define __INCLUDE_TargetMovementEntryBoidsh_INCLUDE__

#include <common/FixedVector.h>
#include <movement/TargetMovementEntry.h>
#include <landscapemap/TargetGroupsSetEntry.h>
#include <map>
#include <vector>

class Obstacle;
class Boid2;
class TargetMovementEntryBoids : public TargetMovementEntry
{
public:
	TargetMovementEntryBoids();
	virtual ~TargetMovementEntryBoids();

	fixed getCruiseDistance() { return cruiseDistance_; }
	fixed getCruiseVelocity() { return cruiseVelocity_; }
	fixed getMaxVelocity() { return maxVelocity_; }
	fixed getMaxAcceleration() { return maxAcceleration_; }
	FixedVector &getMinBounds() { return minBounds_; }
	FixedVector &getMaxBounds() { return maxBounds_; }

	// Overridden from TargetMovementEntry
	virtual void generate(ScorchedContext &context, 
		RandomGenerator &random, 
		LandscapeMovementType *movementType);
	virtual void simulate(fixed frameTime);
	virtual void draw();
	virtual bool writeMessage(NetBuffer &buffer);
	virtual bool readMessage(NetBufferReader &reader);

	std::map<unsigned int, TargetGroup *> &getTargets() { return groupEntry_->getObjects(); }
	std::map<unsigned int, Boid2 *> &getBoidsMap() { return boidsMap_; }

protected:
	TargetGroupsSetEntry *groupEntry_;

	FixedVector minBounds_, maxBounds_;
	fixed cruiseDistance_;
	fixed maxVelocity_, cruiseVelocity_;
	fixed maxAcceleration_;

	// All boids have access to this list, and use it to 
	// determine where all the other boids are. 
	std::map<unsigned int, Boid2 *> boidsMap_;

	void makeBoids(ScorchedContext &context, RandomGenerator &random,
		FixedVector &maxBounds, FixedVector &minBounds);
};

#endif // __INCLUDE_TargetMovementEntryBoidsh_INCLUDE__