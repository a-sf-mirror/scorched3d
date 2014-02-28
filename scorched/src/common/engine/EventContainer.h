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

#if !defined(__INCLUDE_EventContainerh_INCLUDE__)
#define __INCLUDE_EventContainerh_INCLUDE__

#include <landscapedef/LandscapeInclude.h>
#include <landscapedef/LandscapeEvent.h>

class EventContainer
{
public:
	EventContainer();
	virtual ~EventContainer();

	void clear();
	void initialize(ScorchedContext &context);
	void simulate(fixed frameTime, ScorchedContext &context);

protected:
	struct EventEntry
	{
		LandscapeEvent *event;
		fixed eventTime;
		int eventNumber;
	};

	std::vector<EventEntry> events_;

	void addEvent(ScorchedContext &context, 
		std::list<LandscapeEvent *> &events);
	void addEvents(ScorchedContext &context, 
		std::list<LandscapeInclude *> &events);
};

#endif // __INCLUDE_EventContainerh_INCLUDE__
