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
		std::vector<float> vertices;	//������� ��������� � RAM ��� �������������
		unsigned int depth;				//TODO: ������� ����� ��� �����, ����� ������ ����� �������������� ���������� � ��� ������ ����� � ������� ����(���� ����� ���� ��� ����������?)

		BinTree<triangle> lFace;
		BinTree<triangle> rFace;
	};

}