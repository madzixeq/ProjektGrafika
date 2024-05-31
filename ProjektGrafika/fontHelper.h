#pragma once
#include <iostream>
#include "ShaderProgram.h"
#include <ft2build.h>
#include FT_FREETYPE_H  

class FontHelper
{
	FT_Library ft;
	FT_Face face;
	unsigned int VAO;
	unsigned int VBO;
public:
	bool init();
	void dispose();
	void render(ShaderProgram* sp, std::string text, float posX, float posY, float screenX, float screenY, float scale);
};

