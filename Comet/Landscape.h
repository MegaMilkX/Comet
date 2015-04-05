#pragma once
#include "Renderable.h"

#include "BinTree.h"

#include "LandscapeTile.h"

#include "Camera.h"

namespace Comet
{
	class Landscape :
		public Renderable
	{
	public:
		Landscape(std::string path);
		~Landscape();

		//??????
		void SetTileSize(unsigned char sz);
		void SetLandscapeSize(unsigned int w, unsigned int h);

		void Rebuild();

		//Метод Quad tree
		class Quad
		{
		public:
			Quad(){ for (int i = 0; i < 4; i++) quads[i] = 0; threshold = 1.0f; }
			Quad(float t, int d){ for (int i = 0; i < 4; i++) quads[i] = 0; threshold = t; depth = d; }
			~Quad(){ for (int i = 0; i < 4; i++) if (quads[i]) delete quads[i]; }

			void Reset(){ for (int i = 0; i < 4; i++) if (quads[i]) delete quads[i]; depth = 0; }

			unsigned short verts[9];
			Quad* quads[4];
			float threshold;
			int depth;
		};

		void Update();

	private:
		//Нормальный метод
		void _ConstructQuadTree(Quad* quad);
		void _CollectIndices(Quad* quad);
		void _Split(Quad* quad);
		void _InitQuad(Quad* quad, glm::vec2 bl, glm::vec2 tr, char dir);
		Camera* cam; //ландшафту нужно знать где находится камера и ее фрустум
		unsigned char tileSz;
		unsigned int w, h;
		Quad* rootQuad;
		std::vector<unsigned short> indices;
		std::vector<float> vertices;
		std::vector<float> uvw;
	
		unsigned int curDepth;
		unsigned int depth;				//TODO: глубина тайла для лодов, позже тайлов будет неограниченное количество и эти данные будут у каждого свои(хотя зачем если они одинаковые?)

		std::vector<Texture2D*> textures;
		BinTree<triangle> lFace;
		BinTree<triangle> rFace;

		void SplitNode(BinTree<triangle> *tree);
	};

}