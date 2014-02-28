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

#include <uiactions/UILandscapeDeformAction.h>
#include <scorched3dc/ScorchedUI.h>
#include <scorched3dc/UIState.h>
#include <scorched3dc/UIStatePlaying.h>

UILandscapeDeformAction::UILandscapeDeformAction(int x, int y, int w, int h) :
	x_(x), y_(y), w_(w), h_(h)
{
}

UILandscapeDeformAction::~UILandscapeDeformAction()
{
}

void UILandscapeDeformAction::performUIAction()
{
	UIStateI *currentState = ScorchedUI::instance()->getUIState().getCurrentState();
	if (currentState->getState() == UIState::StatePlaying)
	{
		((UIStatePlaying *) currentState)->updateHeight(x_, y_, w_, h_);
	}
	delete this;
}