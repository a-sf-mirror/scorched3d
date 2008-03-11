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

#if !defined(AFX_GLWTEXTBUTTON_H__36D49253_FFE6_4E93_89FC_F2CAC2DC3336__INCLUDED_)
#define AFX_GLWTEXTBUTTON_H__36D49253_FFE6_4E93_89FC_F2CAC2DC3336__INCLUDED_

#include <GLW/GLWButton.h>
#include <GLW/GLWLabel.h>

class GLWTextButton : public GLWButton
{
public:
	GLWTextButton(const std::string &buttonText = "", 
		float x = 0.0f, float y = 0.0f, float w = 0.0f, 
		GLWButtonI *handler = 0,
		unsigned flags = 0,
		float size = 14.0f);
	virtual ~GLWTextButton();

	virtual void draw();

	void setText(const std::string &text) 
		{ label_.setText(text); setName(text); }
	const char *getText() { return label_.getText(); }
	void setColor(const Vector &color) { color_ = color; }

	REGISTER_CLASS_HEADER(GLWTextButton);

protected:
	GLWLabel label_;
	Vector color_;

};

#endif // !defined(AFX_GLWTEXTBUTTON_H__36D49253_FFE6_4E93_89FC_F2CAC2DC3336__INCLUDED_)