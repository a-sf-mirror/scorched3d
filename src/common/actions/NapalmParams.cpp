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

#include <actions/NapalmParams.h>
#include <LUA/LUAUtil.h>
#include <XML/XMLNode.h>

NapalmParams::NapalmParams() : 
	napalmTime_(8),
	napalmHeight_(2),
	stepTime_(fixed(true, 1000)),
	hurtStepTime_(2),
	hurtPerSecond_(1),
	groundScorchPer_(fixed(true, 2000)),
	effectRadius_(5),
	noSmoke_(false),
	noObjectDamage_(false),
	allowUnderWater_(false),
	luminance_(true),
	napalmTexture_("flames"),
	deformTexture_("")
{
}

NapalmParams::~NapalmParams()
{
}

bool NapalmParams::parseXML(XMLNode *accessoryNode)
{
	// Mandatory Attributes
	if (!accessoryNode->getNamedChild("effectradius", effectRadius_)) return false;
	if (!accessoryNode->getNamedChild("napalmtexture", napalmTexture_)) return false;
	if (!accessoryNode->getNamedChild("allowunderwater", allowUnderWater_)) return false;

	// Get the optional luminance node
	XMLNode *noLuminanceNode = 0; luminance_ = true;
	accessoryNode->getNamedChild("noluminance", noLuminanceNode, false);
	if (noLuminanceNode) luminance_ = false;

	// Optional deform texture
	if (accessoryNode->getNamedChild("deformtexture", deformTexture_, false))
	{
		if (!S3D::checkDataFile(getDeformTexture())) return false;
	}

	// Optional Attributes
	XMLNode *noSmokeNode = 0, *noObjectDamageNode = 0;
	accessoryNode->getNamedChild("groundscorchper", groundScorchPer_, false);
	accessoryNode->getNamedChild("nosmoke", noSmokeNode, false);
	accessoryNode->getNamedChild("noobjectdamage", noObjectDamageNode, false);
	if (noSmokeNode) noSmoke_ = true;
	if (noObjectDamageNode) noObjectDamage_ = true;

	return true;
}

void NapalmParams::parseLUA(lua_State *L, int position)
{
	luaL_checktype(L, position, LUA_TTABLE);

	napalmTime_ = LUAUtil::getNumberFromTable(L, position, "napalmtime", napalmTime_);
	napalmHeight_ = LUAUtil::getNumberFromTable(L, position, "napalmheight", napalmHeight_);
	stepTime_ = LUAUtil::getNumberFromTable(L, position, "steptime", stepTime_);
	hurtStepTime_ = LUAUtil::getNumberFromTable(L, position, "hurtsteptime", hurtStepTime_);
	hurtPerSecond_ = LUAUtil::getNumberFromTable(L, position, "hurtpersecond", hurtPerSecond_);
	groundScorchPer_ = LUAUtil::getNumberFromTable(L, position, "groundscorchper", groundScorchPer_);
	effectRadius_ = LUAUtil::getIntFromTable(L, position, "effectradius", effectRadius_);
	noSmoke_ = LUAUtil::getBoolFromTable(L, position, "nosmoke", noSmoke_);
	noObjectDamage_ = LUAUtil::getBoolFromTable(L, position, "noobjectdamage", noObjectDamage_);
	allowUnderWater_ = LUAUtil::getBoolFromTable(L, position, "allowunderwater", allowUnderWater_);
	luminance_ = LUAUtil::getBoolFromTable(L, position, "luminance", luminance_);
	napalmTexture_ = LUAUtil::getStringFromTable(L, position, "napalmtexture", napalmTexture_);
	deformTexture_ = LUAUtil::getStringFromTable(L, position, "deformtexture", deformTexture_);
}
