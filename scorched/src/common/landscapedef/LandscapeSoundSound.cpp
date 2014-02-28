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

#include <landscapedef/LandscapeSoundSound.h>
#include <landscapemap/LandscapeMaps.h>
#ifndef S3D_SERVER
	#include <client/ScorchedClient.h>
#endif
#include <engine/ObjectGroups.h>
#include <engine/ObjectGroup.h>
#include <engine/ObjectGroupEntry.h>
#include <target/Target.h>
#include <target/TargetLife.h>
#include <target/TargetSpace.h>
#include <target/TargetGroup.h>
#include <common/Defines.h>
#include <math.h>

LandscapeSoundSoundChoice::LandscapeSoundSoundChoice() :
	XMLEntryTypeChoice<LandscapeSoundSound>("LandscapeSoundSoundChoice",
		"Associates sounds with a given position")
{
}

LandscapeSoundSoundChoice::~LandscapeSoundSoundChoice()
{
}

LandscapeSoundSound *LandscapeSoundSoundChoice::createXMLEntry(const std::string &type)
{
	if (0 == strcmp(type.c_str(), "file")) return new LandscapeSoundSoundFile;
	S3D::dialogMessage("LandscapeSoundSoundChoice", S3D::formatStringBuffer("Unknown sound type %s", type));
	return 0;
}

void LandscapeSoundSoundChoice::getAllTypes(std::set<std::string> &allTypes)
{
	allTypes.insert("file");
}

LandscapeSoundSound::LandscapeSoundSound(const char *type, const char *description) :
	XMLEntryContainer(type, description)
{
}

LandscapeSoundSound::~LandscapeSoundSound()
{
}

LandscapeSoundSoundFileList::LandscapeSoundSoundFileList() :
	XMLEntryList<XMLEntryString>("A list of files to load sounds from, a random file will be chosen each time a sound is played.", 1)
{
}

LandscapeSoundSoundFileList::~LandscapeSoundSoundFileList()
{
}

XMLEntryString *LandscapeSoundSoundFileList::createXMLEntry()
{
	return new XMLEntryString("A file to load sounds from");
}

LandscapeSoundSoundFile::LandscapeSoundSoundFile() :
	LandscapeSoundSound("LandscapeSoundSoundFile", 
		"Loads a sound file from a file on the file system"),
	gain("The amound of extra gain (volume) to apply to the sound, 1.0 = no gain", 0, 1),
	files(),
	referencedistance("", 0, 75),
	rolloff("", 0, 1)
{
	addChildXMLEntry("file", &files, "gain", &gain,"referencedistance",  &referencedistance, "rolloff", &rolloff);
}

LandscapeSoundSoundFile::~LandscapeSoundSoundFile()
{
}

bool LandscapeSoundSoundFile::play(VirtualSoundSource *source, float ambientGain)
{
#ifndef S3D_SERVER
	/*
	std::string &file = files[rand() % files.size()];
	SoundBuffer *buffer = 
		Sound::instance()->fetchOrCreateBuffer(file.c_str());
	if (!buffer) return false;

	source->setGain(gain * ambientGain);
	source->setRolloff(rolloff);
	source->setReferenceDistance(referencedistance);
	source->play(buffer);
	*/
#endif

	return true;
}
