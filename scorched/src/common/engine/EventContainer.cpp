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

#include <engine/EventContainer.h>
#include <engine/ScorchedContext.h>
#include <landscapedef/LandscapeDefn.h>
#include <landscapedef/LandscapeTex.h>
#include <landscapedef/LandscapeEvent.h>
#include <landscapemap/LandscapeMaps.h>

EventContainer::EventContainer()
{
}

EventContainer::~EventContainer()
{
}

void EventContainer::clear()
{
	events_.clear();
}

void EventContainer::initialize(ScorchedContext &context)
{
	clear();

	LandscapeTex &tex = 
		*context.getLandscapeMaps().getDescriptions().getTex();
	LandscapeDefn &defn = 
		*context.getLandscapeMaps().getDescriptions().getDefn();

	addEvents(context, tex.includes);
	addEvents(context, defn.includes);
}

void EventContainer::addEvents(ScorchedContext &context, 
	std::list<LandscapeInclude *> &events)
{
	std::list<LandscapeInclude *>::iterator itor;
	for (itor = events.begin();
		itor != events.end();
		++itor)
	{
		LandscapeInclude *evt = (*itor);
		addEvent(context, evt->events.getChildren());
	}
}

void EventContainer::addEvent(ScorchedContext &context, 
	std::list<LandscapeEvent *> &events)
{
	std::list<LandscapeEvent *>::iterator itor;
	for (itor = events.begin(); itor != events.end(); ++itor)
	{
		LandscapeEvent *evt = (*itor);
		LandscapeEventCondition *condition = (LandscapeEventCondition *)
			evt->condition.getValue();

		EventEntry entry;
		entry.eventNumber = 0;
		entry.eventTime = 
			condition->getNextEventTime(context, ++entry.eventNumber);
		entry.event = evt;
		events_.push_back(entry);
	}
}

void EventContainer::simulate(fixed frameTime, ScorchedContext &context)
{
	std::vector<EventEntry>::iterator itor;
	for (itor = events_.begin();
		itor != events_.end();
		++itor)
	{
		EventEntry &entry = (*itor);
		
		LandscapeEvent *evt = entry.event;
		LandscapeEventCondition *condition = (LandscapeEventCondition *)
			evt->condition.getValue();

		entry.eventTime -= frameTime;
		if (condition->fireEvent(context, 
			entry.eventTime, entry.eventNumber))
		{
			LandscapeEventAction *action = (LandscapeEventAction *)
				evt->action.getValue();
			action->fireAction(context);
			entry.eventTime = 
				condition->getNextEventTime(
					context, ++entry.eventNumber);
		}
	}	
}
