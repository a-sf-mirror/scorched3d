////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2011
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

#if !defined(__INCLUDE_TanketShotInfoh_INCLUDE__)
#define __INCLUDE_TanketShotInfoh_INCLUDE__

#include <net/NetBuffer.h>
#include <vector>

class Tanket;
class ScorchedContext;
class TanketShotInfo
{
public:
	TanketShotInfo(ScorchedContext &context);
	virtual ~TanketShotInfo();

	void setTanket(Tanket *tanket) { tanket_ = tanket; }

	// State change
	void newGame();

	// Gun/Turret Rotation
	fixed rotateGunXY(fixed angle, bool diff=true);
	fixed rotateGunYZ(fixed angle, bool diff=true);
	fixed getRotationGunXY() { return turretRotXY_; }
	fixed getRotationGunYZ() { return turretRotYZ_; }

	// Select position
	int getSelectPositionX() { return selectPositionX_; }
	int getSelectPositionY() { return selectPositionY_; }
	void setSelectPosition(int x, int y) { 
		selectPositionX_ = x; selectPositionY_ = y; }

	// Power of gun
	fixed getPower() { return power_; }
	fixed changePower(fixed power, bool diff=true);

	fixed getMaxPower() { return maxPower_; }
	void setMaxPower(fixed power) { maxPower_ = power; }

	// Position
	FixedVector &getVelocityVector();
	FixedVector &getTankPosition(); // Position of center bottom of tank
	FixedVector &getTankTurretPosition(); // Position of center of turret
	FixedVector &getTankGunPosition(); // Position of end of gun

	const char *getRotationString();
	const char *getElevationString();
	const char *getPowerString();

	// Serialize the tank
	bool writeMessage(NamedNetBuffer &buffer);
	bool readMessage(NetBufferReader &reader);

protected:
	ScorchedContext &context_;
	Tanket *tanket_;

	// Turret angles
	fixed turretRotXY_, turretRotYZ_, power_;
	fixed maxPower_;
	int selectPositionX_, selectPositionY_;
};

#endif
