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

#if !defined(__INCLUDE_ServerNewGameStateh_INCLUDE__)
#define __INCLUDE_ServerNewGameStateh_INCLUDE__

#include <engine/GameStateI.h>
#include <common/FixedVector.h>

// Sends out the new game message
class RandomGenerator;
class Tank;
class ScorchedContext;
class LandscapeDefnType;
class ServerNewGameState : 
	public GameStateI
{
public:
	ServerNewGameState();
	virtual ~ServerNewGameState();

	virtual void enterState(const unsigned state);

	static int addTanksToGame(const unsigned state,
		bool addState = false);
	static void checkTeams();
	static void checkBots(bool removeBots);
	static void removeTargets();

protected:
	static void checkTeamsAuto();
	static void checkTeamsBotsVs();

	static std::list<Tank *> resetTankStates(unsigned int state);

};

#endif