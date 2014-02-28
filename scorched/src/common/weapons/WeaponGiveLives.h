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

#if !defined(__INCLUDE_WeaponGiveLivesh_INCLUDE__)
#define __INCLUDE_WeaponGiveLivesh_INCLUDE__

#include <actions/CallbackWeapon.h>
#include <XML/XMLEntrySimpleTypes.h>

class WeaponGiveLives  : public WeaponCallback
{
public:
	WeaponGiveLives();
	virtual ~WeaponGiveLives();

	// Inherited from Weapon
	void fireWeapon(ScorchedContext &context,
		WeaponFireContext &weaponContext, FixedVector &position, FixedVector &velocity);

	// Inherited from WeaponPowerUp
	virtual void weaponCallback(
		ScorchedContext &context,
		WeaponFireContext &weaponContext, FixedVector &position, FixedVector &velocity,
		unsigned int userData);

	REGISTER_ACCESSORY_HEADER(WeaponGiveLives, AccessoryPart::AccessoryWeapon);

protected:
	XMLEntryInt lives_;

};

#endif // __INCLUDE_WeaponGiveLivesh_INCLUDE__
