////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2009
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

#include <engine/ModFiles.h>
#include <common/Defines.h>
#include <common/Logger.h>
#include <common/FileList.h>
#include <lang/LangResource.h>
#include <stdio.h>

ModFiles::ModFiles()
{
}

ModFiles::~ModFiles()
{
	std::map<std::string, ModFileEntry *>::iterator itor;
	for (itor = files_.begin();
		itor != files_.end();
		itor++)
	{
		delete (*itor).second;
	}
	files_.clear();
}

bool ModFiles::fileEnding(const std::string &file, const std::string &ext)
{
	int fileLen = file.size();
	int extLen = ext.size();
	if (fileLen < extLen) return false;

	for (int i=0; i<extLen; i++)
	{
		if (file.c_str()[fileLen - i - 1] != ext.c_str()[extLen - i - 1])
		{
			return false;
		}
	}
	return true;
}

bool ModFiles::excludeFile(const std::string &file)
{
	if (file == "Thumbs.db") return true;
	if (file == "CVS") return true;
	if (file == "..") return true;

	if (!fileEnding(file, ".xml") &&
		!fileEnding(file, ".bmp") &&
		!fileEnding(file, ".txt") &&
		!fileEnding(file, ".png") &&
		!fileEnding(file, ".jpg") &&
		!fileEnding(file, ".ico") &&
		!fileEnding(file, ".ase") &&
		!fileEnding(file, ".wav") &&
		!fileEnding(file, ".ogg"))
	{
		Logger::log(S3D::formatStringBuffer("Excluding mod file \"%s\"", file.c_str()));
		return true;
	}

	return false;
}

bool ModFiles::loadModFiles(const std::string &mod, 
	bool createDir, ProgressCounter *counter)
{
	{
		// Get and check the user mod directory exists
		std::string modDir = S3D::getModFile(mod);
		if (S3D::dirExists(modDir.c_str()))
		{
			if (counter) counter->setNewOp(LANG_RESOURCE("LOADING_USER_MODS", "Loading User Mods"));
			if (!loadModDir(modDir, mod, counter)) return false;
		}
		else
		{
			if (createDir) S3D::dirMake(modDir);
		}
	}

	{
		// Get and check global mod directory
		std::string modDir = S3D::getGlobalModFile(mod);
		if (S3D::dirExists(modDir))
		{
			if (counter) counter->setNewOp(LANG_RESOURCE("LOADING_GLOBAL_MODS", "Loading Global Mods"));
			if (!loadModDir(modDir, mod, counter)) return false;
		}
	}

	// For the default "none" mod load some files that can
	// be downloaded
	if (mod == "none")
	{
		loadLocalModFile("data/accessories.xml", mod);
		loadLocalModFile("data/modinfo.xml", mod);
		loadLocalModFile("data/landscapes.xml", mod);

		const char *landscapesBase = "data/landscapes";
		std::string dir = S3D::getDataFile(landscapesBase);
		FileList fList(dir.c_str(), "*.xml", true);
		std::list<std::string> &files = fList.getFiles();
		std::list<std::string>::iterator itor;
		for (itor = files.begin();
			itor != files.end();
			itor++)
		{
			char *file = (char *) (*itor).c_str();
			file += dir.size() - strlen(landscapesBase);
			loadLocalModFile(file, mod);
		}
	}
	
	{
		unsigned int totalCompSize = 0, totalSize = 0;
		std::map<std::string, ModFileEntry *>::iterator itor;
		for (itor = files_.begin();
			itor != files_.end();
			itor++)
		{
			ModFileEntry *entry = (*itor).second;
			totalCompSize += entry->getCompressedSize();
			totalSize += entry->getUncompressedSize();
		}

		Logger::log(S3D::formatStringBuffer("Loaded mod \"%s\", %u files, space required %u (%u) bytes", 
			mod.c_str(), files_.size(), totalCompSize, totalSize));

		if (!createDir && files_.empty())
		{
			std::string modFile = S3D::getModFile(mod);
			std::string globalModFile = S3D::getGlobalModFile(mod);
			S3D::dialogMessage("Mod", S3D::formatStringBuffer(
				"Failed to find \"%s\" mod files in directories \"%s\" \"%s\"",
				mod.c_str(),
				modFile.c_str(),
				globalModFile.c_str()));
			return false;
		}
	}

	return true;
}

bool ModFiles::loadLocalModFile(const std::string &local, 
	const std::string &mod)
{
	std::string dataFile = S3D::getDataFile(local);
	std::string modDirStr(dataFile);
	char *modDir = (char *) modDirStr.c_str();
	modDir[dataFile.size() - local.size()] = '\0';

	return loadModFile(dataFile, modDir, mod);
}

bool ModFiles::loadModDir(const std::string &modDir, 
	const std::string &mod, ProgressCounter *counter)
{
	// Load all files contained in this directory
	FileList allfiles(modDir.c_str(), "*", true, true);
	FileList::ListType &files = allfiles.getFiles();
	FileList::ListType::iterator itor;

	int i = 0;
	for (itor = files.begin();
		itor != files.end();
		itor++, i++)
	{
		if (counter) counter->setNewPercentage(float(i) 
			/ float(files.size()) * 100.0f);

		// Load the file
		std::string &fileName = (*itor);
		if (!loadModFile(fileName, modDir, mod))
		{
			return false;
		}
	}

	return true;
}

bool ModFiles::loadModFile(const std::string &fullFileName,
	const std::string &modDir, const std::string &mod)
{
	std::string shortFileNameStr(fullFileName);
	S3D::fileDos2Unix(shortFileNameStr);
	const char *shortFileName = shortFileNameStr.c_str();

	// Check to see if we ignore this file
	if (excludeFile(fullFileName)) return true;

	// Turn it into a unix style path and remove the 
	// name of the directory that contains it
	int modDirLen = modDir.size();
	shortFileName += modDirLen;
	while (shortFileName[0] == '/') shortFileName++;

	// Check that all files are lower case
	std::string oldFileName = shortFileName;
	_strlwr((char *) shortFileName);
	if (strcmp(oldFileName.c_str(), shortFileName) != 0)
	{
		S3D::dialogMessage("Mod", S3D::formatStringBuffer(
			"ERROR: All mod files must have lower case filenames.\n"
			"File \"%s,%s\" has upper case charaters in it",
			oldFileName.c_str(),
			shortFileName));
		return false;
	}

	// Check we don't have this file already
	if (files_.find(shortFileName) != files_.end()) return true;

	// Create the new mod file and load the file
	ModFileEntry *file = new ModFileEntry();
	if (!file->loadModFile(fullFileName))
	{
		S3D::dialogMessage("Mod", S3D::formatStringBuffer(
			"Error: Failed to load file \"%s\" mod directory \"%s\" in the \"%s\" mod",
			fullFileName.c_str(),
			modDir.c_str(),
			mod.c_str()));
		return false;
	}

	// Store for future
	file->setFileName(shortFileName);
	files_[shortFileName] = file;
	return true;
}

bool ModFiles::writeModFiles(const std::string &mod)
{
	std::string modDir = S3D::getModFile(mod);
	if (!S3D::dirExists(modDir))
	{
		S3D::dirMake(modDir);
	}

	std::map<std::string, ModFileEntry *>::iterator itor;
	for (itor = files_.begin();
		itor != files_.end();
		itor++)
	{
		ModFileEntry *entry = (*itor).second;
		if (!entry->writeModFile(entry->getFileName(), mod)) return false;
	}
	return true;
}

void ModFiles::clearData()
{
	 std::map<std::string, ModFileEntry *>::iterator itor;
	 for (itor = files_.begin();
	 	itor != files_.end();
		itor++)
	{
		 ModFileEntry *entry = (*itor).second;
		 entry->getCompressedBuffer().clear();
	}
}

bool ModFiles::exportModFiles(const std::string &mod, const std::string &fileName)
{
	FILE *out = fopen(fileName.c_str(), "wb");
	if (!out) return false;

	// Mod Name
	NetBuffer tmpBuffer;
	tmpBuffer.reset();
	tmpBuffer.addToBuffer(S3D::ScorchedProtocolVersion);
	tmpBuffer.addToBuffer(mod);
	fwrite(tmpBuffer.getBuffer(),
		sizeof(unsigned char),
		tmpBuffer.getBufferUsed(), 
		out);	

	// Mod Files
	std::map<std::string, ModFileEntry *>::iterator itor;
	for (itor = files_.begin();
		itor != files_.end();
		itor++)
	{
		ModFileEntry *entry = (*itor).second;
	
		tmpBuffer.reset();
		tmpBuffer.addToBuffer(entry->getFileName());
		tmpBuffer.addToBuffer(entry->getUncompressedSize());
		tmpBuffer.addToBuffer(entry->getCompressedBuffer().getBufferUsed());
		tmpBuffer.addToBuffer(entry->getCompressedCrc());
		tmpBuffer.addDataToBuffer(entry->getCompressedBuffer().getBuffer(),
			entry->getCompressedBuffer().getBufferUsed());

		fwrite(tmpBuffer.getBuffer(),
			sizeof(unsigned char),
			tmpBuffer.getBufferUsed(), 
			out);
	}

	// End of Mod Files
	tmpBuffer.reset();
	tmpBuffer.addToBuffer("*");
	fwrite(tmpBuffer.getBuffer(),
		sizeof(unsigned char),
		tmpBuffer.getBufferUsed(), 
		out);

	fclose(out);
	return true;
}

bool ModFiles::importModFiles(std::string &mod, const std::string &fileName)
{
	FILE *in = fopen(fileName.c_str(), "rb");
	if (!in) return false;

	// Read Buffer
	NetBuffer tmpBuffer;
	unsigned char readBuf[512];
	while (unsigned int size = fread(readBuf, sizeof(unsigned char), 512, in))
	{
		tmpBuffer.addDataToBuffer(readBuf, size);
	}
	fclose(in);
	return importModFiles(mod, tmpBuffer);
}

bool ModFiles::importModFiles(std::string &mod, NetBuffer &tmpBuffer)
{
	// Mod Name
	static std::string modName;
	std::string version;
	NetBufferReader tmpReader(tmpBuffer);
	if (!tmpReader.getFromBuffer(version)) return false;
	if (!tmpReader.getFromBuffer(modName)) return false;
	mod = modName;

	if (version != S3D::ScorchedProtocolVersion)
	{
		S3D::dialogMessage("Scorched3D", S3D::formatStringBuffer(
			"Failed to import mod, scorched version differs.\n"
			"Please obtain a newer version of this mod.\n"
			"Import version = %s\n"
			"Current version = %s\n",
			version.c_str(),
			S3D::ScorchedProtocolVersion.c_str()));
		return false;
	}

	for (;;)
	{
		// File Name
		std::string name;
		if (!tmpReader.getFromBuffer(name)) return false;
		if (0 == strcmp(name.c_str(), "*")) break;

		// File Header
		unsigned int unCompressedSize;
		unsigned int compressedSize;
		unsigned int compressedCrc;
		tmpReader.getFromBuffer(unCompressedSize);
		tmpReader.getFromBuffer(compressedSize);
		tmpReader.getFromBuffer(compressedCrc);

		// File
		ModFileEntry *entry = new ModFileEntry;
		entry->setFileName(name.c_str());
		entry->setCompressedCrc(compressedCrc);
		entry->setUncompressedSize(unCompressedSize);
		entry->getCompressedBuffer().allocate(compressedSize);
		entry->getCompressedBuffer().setBufferUsed(compressedSize);
		tmpReader.getDataFromBuffer(
			entry->getCompressedBuffer().getBuffer(), 
			compressedSize);
		files_[name] = entry;
	}

	return true;
}