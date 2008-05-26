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

#include <client/ScorchedClient.h>
#include <GLEXT/GLState.h>
#include <GLEXT/GLMenuEntry.h>
#include <GLEXT/GLTexture.h>
#include <image/Image.h>
#include <GLW/GLWidget.h>
#include <GLW/GLWFont.h>

static Vector color(0.9f, 0.9f, 1.0f);
static Vector itemcolor(0.1f, 0.1f, 0.4f);
static const float menuItemHeight = 20.0f;

GLMenuEntry::GLMenuEntry(
	char *menuName, 
	const char *menuDescription,
	float width, 
	unsigned int state,
	GLMenuI *callback,
	Image *icon,
	unsigned int flags) :
	left_(0.0f), width_(width), height_(0.0f),
	callback_(callback),
	menuName_(menuName), menuDescription_(menuDescription),
	state_(state), selected_(false),
	texture_(0), icon_(icon), flags_(flags),
	toolTip_(ToolTip::ToolTipHelp, menuName, menuDescription)
{
}

GLMenuEntry::~GLMenuEntry()
{
	delete icon_;
	delete texture_;
}

void GLMenuEntry::draw(float currentTop, float currentLeft)
{
	left_ = currentLeft;
	top_ = currentTop;
	height_ = 0.0f;

	if (icon_)
	{
		drawIcon();
	}
	else
	{
		drawText();
	}
}

void GLMenuEntry::drawIcon()
{
	if (!texture_)
	{
		texture_ = new GLTexture();
		texture_->create(*icon_, false);
	}

	GLState state(GLState::TEXTURE_ON | GLState::BLEND_ON | GLState::DEPTH_OFF);

	height_ = 32.0f;
	glColor4f(1.0f, 1.0f, 1.0f, selected_?1.0f:0.5f);
	texture_->draw();
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(left_, top_ - 32.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(left_ + 32.0f, top_ - 32.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(left_ + 32.0f, top_);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(left_, top_);
	glEnd();
}

void GLMenuEntry::drawText()
{

	// Draw the menu backdrops
	height_ = menuItemHeight;
	{
		GLState currentStateBlend(GLState::BLEND_ON);
		glColor4f(0.4f, 0.6f, 0.8f, 0.6f);
		glBegin(GL_TRIANGLE_FAN);
			glVertex2f(left_ + 10.0f, top_ - 10.0f);
			glVertex2f(left_ + 10.0f, top_ - menuItemHeight);
			GLWidget::drawRoundBox(left_, top_ - menuItemHeight, 
				width_, menuItemHeight, 10.0f);
			glVertex2f(left_ + 10.0f, top_ - menuItemHeight);
		glEnd();

		glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
		glLineWidth(2.0f);
		glBegin(GL_LINE_LOOP);
			GLWidget::drawRoundBox(left_, top_ - menuItemHeight, 
				width_, menuItemHeight, 10.0f);
		glEnd();
		glLineWidth(1.0f);
	}

	// Get and print the menu title text
	char *menuTitle = (char *) callback_->getMenuText(menuName_.c_str());
	if (!menuTitle)
	{
		// Else default to the name of the menu
		menuTitle = (char *) menuName_.c_str();
	}

	GLWFont::instance()->getGameFont()->
		draw((selected_?color:itemcolor), 12, left_ + 5.0f, 
			top_ - 15.0f, 0.0f, menuTitle);
}

bool GLMenuEntry::inMenu(float currentTop, int x, int y)
{
	float height = menuItemHeight;
	if (icon_) height = 32.0f;
	if (y > currentTop - height &&
		x>left_ && x<left_ + width_) 
	{
		return true;
	}
	return false;
}

bool GLMenuEntry::click(float currentTop, int x, int y)
{
	float height = menuItemHeight;
	if (icon_) height = 32.0f;

	if (y > currentTop - height &&
		x>left_ && x<left_ + width_) 
	{
		if (callback_->menuOpened(menuName_.c_str()))
		{
			selected_ = true;
			
			// Get the contents of the menu
			std::list<GLMenuItem> tmpMenuItems;
			if (callback_->getMenuItems(menuName_.c_str(), tmpMenuItems))
			{
				menuItems_ = tmpMenuItems;
			}
		
			// Show the menu
			std::list<GLWSelectorEntry> entries;
			std::list<GLMenuItem>::iterator itor;
			for (itor = menuItems_.begin();
				itor != menuItems_.end();
				itor++)
			{
				GLMenuItem &item = (*itor);
				entries.push_back(
					GLWSelectorEntry(
						item.getText(),
						item.getToolTip(),
						item.getSelected(),
						item.getTexture(),
						item.getUserData()
						)
					);
				if (item.getSeperator()) entries.back().setSeperator();
			}
			GLWSelector::instance()->showSelector(
				this, 
				left_, currentTop - (height + 10.0f), 
				entries,
				state_);
		}
		return true;
	}

	return false;
}

void GLMenuEntry::addMenuItem(GLMenuItem &item)
{
	menuItems_.push_back(item);
}

void GLMenuEntry::noItemSelected()
{
	selected_ = false;
}

void GLMenuEntry::itemSelected(GLWSelectorEntry *entry, int position)
{
	selected_ = false;
	GLMenuItem item("None");

	int pos = 0;
	std::list<GLMenuItem>::iterator itor;
	for (itor = menuItems_.begin();
		itor != menuItems_.end();
		itor++, pos++)
	{
		if (pos == position)
		{
			item = (*itor);
		}
	}

	callback_->menuSelection(menuName_.c_str(), position, item);
}
