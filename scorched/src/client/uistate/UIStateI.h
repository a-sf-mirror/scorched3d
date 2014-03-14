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

#if !defined(__INCLUDE_UIStateIh_INCLUDE__)
#define __INCLUDE_UIStateIh_INCLUDE__

#include <uistate/UIState.h>

class UIStateI
{
public:
	UIStateI(UIState::State thisState);
	virtual ~UIStateI();

	virtual void createState() = 0;
	virtual void destroyState() = 0;
	virtual void updateState(float frameTime) {}

	UIState::State getState() { return thisState_; }

protected:
	UIState::State thisState_;
};

#endif // __INCLUDE_UIStateIh_INCLUDE__