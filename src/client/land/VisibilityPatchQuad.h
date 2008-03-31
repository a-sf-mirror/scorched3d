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

#if !defined(__INCLUDE_VisibilityPatchQuadh_INCLUDE__)
#define __INCLUDE_VisibilityPatchQuadh_INCLUDE__

#include <land/VisibilityPatchInfo.h>
#include <common/Vector.h>

class VisibilityPatchGrid;
class VisibilityPatchQuad
{
public:
	VisibilityPatchQuad();
	~VisibilityPatchQuad();

	void setLocation(VisibilityPatchGrid *patchGrid, int x, int y, int size, 
		int mapwidth, int mapheight);
	void calculateVisibility(VisibilityPatchInfo &patchInfo, Vector &cameraPos);

protected:
	int x_, y_;
	int size_;
	Vector position_;

	WaterVisibilityPatch *waterVisibilityPatch_;
	LandVisibilityPatch *landVisibilityPatch_;
	VisibilityPatchQuad *topLeft_, *topRight_;
	VisibilityPatchQuad *botLeft_, *botRight_;

	void setNotVisible(VisibilityPatchInfo &patchInfo, Vector &cameraPos);
	void setVisible(VisibilityPatchInfo &patchInfo, Vector &cameraPos);

};

#endif // __INCLUDE_VisibilityPatchQuadh_INCLUDE__
