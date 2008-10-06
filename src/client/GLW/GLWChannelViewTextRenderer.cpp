////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2003
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

#include <GLW/GLWChannelViewTextRenderer.h>
#include <GLW/GLWChannelView.h>
#include <GLW/GLWTranslate.h>
#include <tank/TankContainer.h>
#include <tankgraph/TargetRendererImplTank.h>
#include <client/ScorchedClient.h>
#include <lang/LangResource.h>

GLWChannelViewTextRenderer::GLWChannelViewTextRenderer(GLWChannelView *channelView) :
	channelView_(channelView)
{
}

GLWChannelViewTextRenderer::~GLWChannelViewTextRenderer()
{
}

void GLWChannelViewTextRenderer::drawCharacter(
	int charPosition, Vector &position, 
	GLFont2d::CharEntry &charEntry, Vector4 &color)
{
	ChannelTextEntry *textEntry = getEntry(charPosition);
	if (!textEntry) return;

	// Set the appropriate tool tip
	if (GLWToolTip::instance()->addToolTip(
		&toolTip_,
		GLWTranslate::getPosX() + position[0], 
		GLWTranslate::getPosY() + position[1],
		12.0f, 18.0f))
	{
		switch (textEntry->type)
		{
		case ePlayerEntry:
			{
				TargetRendererImplTank *renderer = 0;
				Tank *tank = ScorchedClient::instance()->getTankContainer().getTankById(
					textEntry->data);
				if (tank) renderer = (TargetRendererImplTank *) tank->getRenderer();
				if (renderer)
				{
					GLWToolTip::instance()->addToolTip(
							&renderer->getTips()->tankTip,
							GLWTranslate::getPosX() + position[0], 
							GLWTranslate::getPosY() + position[1],
							12.0f, 18.0f);
				}
				else
				{
					toolTip_.setText(ToolTip::ToolTipInfo, 
						LANG_RESOURCE("PLAYER_DISCONECTED", "Player disconnected"),
						LANG_STRING(textEntry->part));		
				}
			}
			break;
		case eChannelEntry:
			toolTip_.setText(ToolTip::ToolTipHelp, 
				LANG_RESOURCE("CHANNEL", "Channel"),
				LANG_STRING(textEntry->part));		
			break;
		case eWeaponEntry:
			toolTip_.setText(ToolTip::ToolTipHelp, 
				LANG_RESOURCE("WEAPON", "Weapon"),
				LANG_STRING(textEntry->part));	
			break;
		case eAdminEntry:
			toolTip_.setText(ToolTip::ToolTipHelp, 
				LANG_RESOURCE("ADMIN", "Admin"),
				LANG_STRING(textEntry->part));	
			break;
		}
	}

	// Set the appropriate text colours etc.
	switch (textEntry->type)
	{
	case eChannelEntry:
		break;
	default:
		color[0] = textEntry->color[0];
		color[1] = textEntry->color[1];
		color[2] = textEntry->color[2];
		break;
	}
}
