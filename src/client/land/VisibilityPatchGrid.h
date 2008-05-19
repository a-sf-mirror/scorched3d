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

#if !defined(__INCLUDE_VisibilityPatchGridh_INCLUDE__)
#define __INCLUDE_VisibilityPatchGridh_INCLUDE__

#include <land/VisibilityPatchQuad.h>
#include <land/VisibilityPatchInfo.h>
#include <land/LandSurround.h>
#include <geomipmap/MipMapPatchIndexs.h>

class GLSLShaderSetup;
class WaterVisibilityPatch;
class Water2Patches;
class VisibilityPatchGrid
{
public:
	static VisibilityPatchGrid *instance();

	void generate(float *waterIndexErrors);

	void calculateVisibility();
	void recalculateErrors(FixedVector &position, fixed size);

	void drawLand(int addIndex = 0, bool simple = false);
	void drawLandLODLevels();
	void drawSurround();
	void drawWater(Water2Patches &patches, 
		MipMapPatchIndexs &indexes, Vector &cameraPosition, 
		Vector landscapeSize,
		GLSLShaderSetup *waterShader);

	int getVisibleLandPatchesCount() { 
		return patchInfo_.getVisibleLandPatchesCount(); }
	int getVisibleWaterPatchesCount() {
		return patchInfo_.getVisibleWaterPatchesCount(); }

	LandVisibilityPatch *getLandVisibilityPatch(int x, int y);
	TargetVisibilityPatch *getTargetVisibilityPatch(int x, int y);
	WaterVisibilityPatch *getWaterVisibilityPatch(int x, int y);
protected:
	LandSurround surround_;
	MipMapPatchIndexs landIndexs_;

	// All the visibility patches
	VisibilityPatchInfo patchInfo_;
	LandVisibilityPatch *landPatches_;
	TargetVisibilityPatch *targetPatches_;
	WaterVisibilityPatch *waterPatches_;

	// The visibility data that decides if a visibility patch is visible or not
	VisibilityPatchQuad *visibilityPatches_;

	// The size of the patches
	int midX_, midY_;
	int landWidth_, landHeight_;
	int waterWidth_, waterHeight_;
	int targetWidth_, targetHeight_;
	int visibilityWidth_, visibilityHeight_;

	void clear();
	void drawLandPatches();

private:
	VisibilityPatchGrid();
	~VisibilityPatchGrid();
};

#endif // __INCLUDE_VisibilityPatchGridh_INCLUDE__
