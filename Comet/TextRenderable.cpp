#include "TextRenderable.h"
#include "Core.h"

#include <Windows.h>

namespace Comet
{

	TextRenderable::TextRenderable()
	{
		mat = 0;
	}

	TextRenderable::TextRenderable(std::string str)
	{
		this->font = ResMan::GetInstance()->GetResource<Font>("data\\fonts\\Magic.ttf");
		textures.push_back(this->font->GetTexture());
		SetText(str);
	}

	TextRenderable::TextRenderable(std::string str, int sz)
	{
		this->font = ResMan::GetInstance()->GetResource<Font>("data\\fonts\\Magic.ttf");
		textures.push_back(this->font->GetTexture());
		SetText(str);
	}

	TextRenderable::TextRenderable(std::string str, std::string fontpath, int sz) : TextRenderable(str, fontpath, sz, "ShaderFontScreenDefault")
	{
		
	}

	TextRenderable::TextRenderable(std::string str, std::string fontpath, int sz, std::string shader)
	{
		ResMan::GetInstance()->GetParamStack()->push(sz);
		this->font = ResMan::GetInstance()->GetResource<Font>(fontpath);
		textures.push_back(this->font->GetTexture());
		
		mat = new Material();
		mat->SetTexture2D(this->font->GetTexture(), 0);
		mat->SetShader(ResMan::GetInstance()->GetResource<Shader>(shader));
		mat->SetParameter("ScreenWidth", (float)(Core::GetInstance()->GetRenderer()->GetWindowWidth()));
		mat->SetParameter("ScreenHeight", (float)(Core::GetInstance()->GetRenderer()->GetWindowHeight()));
		
		SetText(str);
	}

	TextRenderable::~TextRenderable()
	{
		if (meshData)
			delete meshData;

		if (mat)
			delete mat;
	}

	void TextRenderable::SetText(std::string str)
	{
		std::vector<float> vertices;
		std::vector<float> uvw;
		std::vector<float> color;
		std::vector<unsigned short> faces;

		//Вся инфа для формирования меша и соответствующая фонту текстурка находятся в объекте типа Comet::Font

		char cutf8[4];
		unsigned int iutf8 = 0;
		std::vector<unsigned int> characters;
		std::string::iterator it = str.begin();
		while (it != str.end())
		{
			cutf8[0] = (*it);
			cutf8[1] = (*(it + 1));
			cutf8[2] = (*(it + 2));
			cutf8[3] = (*(it + 3));
			if (cutf8[0] <= 127)
			{
				characters.push_back(cutf8[0]);
				it++;
			}
			else if (cutf8[0] >= 194 && cutf8[0] <= 223)
			{
				iutf8 = 0;
				iutf8 |= cutf8[0];
				iutf8 |= cutf8[1] << 4;
				characters.push_back(iutf8);
				it += 2;
			}
			else if (cutf8[0] >= 224 && cutf8[0] <= 239)
			{
				iutf8 = 0;
				iutf8 |= cutf8[0];
				iutf8 |= cutf8[1] << 4;
				iutf8 |= cutf8[2] << 8;
				characters.push_back(iutf8);
				it += 3;
			}
			else if (cutf8[0] >= 240 && cutf8[0] <= 255)
			{
				iutf8 = 0;
				iutf8 |= cutf8[0];
				iutf8 |= cutf8[1] << 4;
				iutf8 |= cutf8[2] << 8;
				iutf8 |= cutf8[3] << 12;
				characters.push_back(iutf8);
				it += 4;
			}
		}

		if (meshData)
			delete meshData;
		meshData = new Comet::MeshData();

		//Для каждого символа в строке (UTF8!)
		float xAccAdv = 0;
		unsigned int li = 0;
		for (int i = 0; i < characters.size(); i++)
		{
			//записать вертексы, текстурные координаты и цвет в массивы
			GlyphInfo* gi = font->GetGlyph(characters[i]);
			if (gi)
			{
				
				vertices.push_back(xAccAdv);					vertices.push_back(-(gi->h - gi->hBearingY));					vertices.push_back(0.0f);
				vertices.push_back(gi->w + xAccAdv);			vertices.push_back(-(gi->h - gi->hBearingY));					vertices.push_back(0.0f);
				vertices.push_back(gi->w + xAccAdv);			vertices.push_back(gi->h - (gi->h - gi->hBearingY));	vertices.push_back(0.0f);
				vertices.push_back(xAccAdv);					vertices.push_back(gi->h - (gi->h - gi->hBearingY));	vertices.push_back(0.0f);

				//printf("%c, w: %f h: %f adv: %f\n", characters[i], gi->w, gi->h, xAccAdv);
				
				uvw.push_back(gi->bmpRect.x);						uvw.push_back(gi->bmpRect.y + gi->bmpRect.h);						uvw.push_back(0.0f);
				uvw.push_back(gi->bmpRect.x + gi->bmpRect.w);		uvw.push_back(gi->bmpRect.y + gi->bmpRect.h);						uvw.push_back(0.0f);
				uvw.push_back(gi->bmpRect.x + gi->bmpRect.w);		uvw.push_back(gi->bmpRect.y);										uvw.push_back(0.0f);
				uvw.push_back(gi->bmpRect.x);						uvw.push_back(gi->bmpRect.y);										uvw.push_back(0.0f);

				faces.push_back(0 + li); faces.push_back(1 + li); faces.push_back(3 + li);
				faces.push_back(1 + li); faces.push_back(2 + li); faces.push_back(3 + li);
				li += 4;

				xAccAdv += gi->advanceX;
			}
		}
		//Сохранить меш дату
		meshData->FillPosition(vertices);
		meshData->FillUVW(uvw);
		meshData->FillColor(color);
		meshData->FillIndices(faces);

		bndRect.x = 0; bndRect.y = 0;
		bndRect.z = xAccAdv;
		bndRect.w = font->LineHeight();
	}

	vec4f TextRenderable::GetBoundingRect()
	{
		return bndRect;
	}
}