#pragma once
#include <raylib.h>
#include <map>
#include <string>

class TextureManager
{
protected:

	std::map<unsigned int, Texture2D> textureMap;

	const std::string folderRoot = "res/";

protected:

	unsigned int GenerateHash(std::string fileName);

public:

	const Texture2D GetTexture(std::string fileName);

	void LoadNewTexture(std::string fileName);

};