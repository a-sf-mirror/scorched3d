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

#include <lang/LangImpl.h>
#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>

static SDL_mutex *langMutex = 0;

LangImpl::LangImpl()
{
	langMutex = SDL_CreateMutex();
	init();
}

void LangImpl::init()
{
	SDL_LockMutex(langMutex);
	bundles_.push_back(&defaultBundle_);
	SDL_UnlockMutex(langMutex);
}

ResourceBundleEntry *LangImpl::getEntry(const std::string &key)
{
	SDL_LockMutex(langMutex);

	ResourceBundleEntry *entry = 0;
	std::vector<ResourceBundle *>::iterator itor;
	for (itor = bundles_.begin();
		itor != bundles_.end();
		itor++)
	{
		ResourceBundle *bundle = *itor;
		entry = bundle->getEntry(key);
		if (entry) break;
	}
	if (!entry) 
	{
		entry = new ResourceBundleEntry(key);
		defaultBundle_.addEntry(entry);
	}

	SDL_UnlockMutex(langMutex);
	return entry;
}