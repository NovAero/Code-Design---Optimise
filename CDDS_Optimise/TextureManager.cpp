#include "TextureManager.h"


Texture2D TextureManager::GetTexture(std::string fileName)
{
	if (textureMap.find(GenerateHash(fileName)) != textureMap.end()) {
		return textureMap.find(GenerateHash(fileName))->second;
	}
	else {
		LoadNewTexture(fileName);
		return textureMap.find(GenerateHash(fileName))->second;
	}
}

void TextureManager::LoadNewTexture(std::string fileName)
{

	Texture2D temp = LoadTexture((folderRoot+fileName).c_str());

	if (temp.id <= 0) return;

	textureMap.insert({ GenerateHash(fileName), temp });

}

void TextureManager::Empty()
{
	textureMap.clear();
}

unsigned int TextureManager::GenerateHash(std::string fileName)
{
	const int seed = 237;
	unsigned long hash = 0;

	for (int i = 0; i < fileName.length(); ++i) {
		hash = (hash * seed) + fileName[i];
	}
	return hash;
}