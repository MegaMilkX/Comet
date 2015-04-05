#pragma once

#include <vector>
#include "BinTree.h"

namespace Comet
{
	struct triangle
	{
		triangle(){ a = 0; b = 0; c = 0; }
		triangle(unsigned int a, unsigned int b, unsigned int c){ this->a = a; this->b = b; this->c = c; }
		unsigned int a, b, c;
	};

	class LandscapeTile
	{
	public:
		LandscapeTile(unsigned int sz);
		~LandscapeTile();



	private:
		std::vector<float> vertices;	//Вершины ландшафта в RAM для инициализации
		unsigned int depth;				//TODO: глубина тайла для лодов, позже тайлов будет неограниченное количество и эти данные будут у каждого свои(хотя зачем если они одинаковые?)

		BinTree<triangle> lFace;
		BinTree<triangle> rFace;
	};

}