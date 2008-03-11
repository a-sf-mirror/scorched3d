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

#if !defined(__INCLUDE_SkyDomeh_INCLUDE__)
#define __INCLUDE_SkyDomeh_INCLUDE__

#include <landscape/LargeHemisphere.h>
#include <landscape/SkyLine.h>
#include <GLEXT/GLTexture.h>
#include <image/ImageHandle.h>

class SkyDome
{
public:
	SkyDome();
	virtual ~SkyDome();

	void simulate(float frameTime);
	void drawBackdrop();
	void drawLayers();
	void generate();
	void flash();

protected:
	float xy_;
	float cloudSpeed_;
	Vector cloudDirection_;
	float flashTime_;
	GLTexture cloudTexture_;
	GLTexture starTexture_;
	GLTexture skyLineTexture_;
	ImageHandle skyColorsMap_;
	bool useStarTexture_;
	bool noSunFog_;
	bool horizonGlow_;
	bool useSkyLine_;

	SkyLine skyLine1_;
	LargeHemisphere clouds1_;
	LargeHemisphere clouds2_;
	LargeHemisphere colors_;
	LargeHemisphere stars_;
};

#endif // __INCLUDE_SkyDomeh_INCLUDE__