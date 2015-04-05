#include "Landscape.h"

#include "ResMan.h"

#include "Renderer.h"

#include <Windows.h>

namespace Comet
{


	Landscape::Landscape(std::string path)
	{
		//Метод quad tree
		meshData = new MeshData();
		ResMan::GetInstance()->SetResource(path + "_Mesh", meshData);

		rootQuad = new Quad();
		depth = 5;
		
		meshData->SetPrimitiveType(MeshData::PrimitiveType::TRIANGLE);
	}


	Landscape::~Landscape()
	{
		delete rootQuad;
	}

	//Метод quad tree
	void Landscape::Update()
	{
		cam = Renderer::instance->GetCurrentCamera();

		//Подготовка к дроблению---------
		uvw.clear();
		indices.clear();
		vertices.clear();

		rootQuad->Reset();
		_InitQuad(rootQuad, glm::vec2(0, 0), glm::vec2(256.0f, 256.0f), 0);

		
		//--------------------------------
		curDepth = 0;
		_ConstructQuadTree(rootQuad); //Здесь строится дерево и заполняется массив вершин
		_CollectIndices(rootQuad);		

		//Заполняем
		meshData->FillPosition(vertices);
		meshData->FillUVW(uvw);
		meshData->FillIndices(indices);
	}

	void Landscape::_ConstructQuadTree(Quad* quad)
	{
		//float dis = glm::distance(cam->GetNode()->GetPosition(), glm::vec3(vertices[quad->verts[0] * 3], vertices[quad->verts[0] * 3 + 1], vertices[quad->verts[0] * 3 + 2]));
		float dis = glm::distance(glm::vec3(128,128,0), glm::vec3(vertices[quad->verts[0] * 3], vertices[quad->verts[0] * 3 + 1], vertices[quad->verts[0] * 3 + 2]));
		if ((dis < quad->threshold*200.0f) && (quad->depth < depth))
		{
			_Split(quad);
		}

		//printf("Distance: %f\n", dis);
	}

	void Landscape::_Split(Quad* quad)
	{
		for (int i = 0; i < 4; i++)
		{
			quad->quads[i] = new Quad(quad->threshold*0.85f, quad->depth++);
			glm::vec2 a, b;
			switch (i)
			{
			case 0:
				a = glm::vec2(vertices[quad->verts[1] * 3], vertices[quad->verts[1] * 3 + 1]);
				b = glm::vec2(vertices[quad->verts[0] * 3], vertices[quad->verts[0] * 3 + 1]);
				break;
			case 1:
				a = glm::vec2(vertices[quad->verts[2] * 3], vertices[quad->verts[2] * 3 + 1]);
				b = glm::vec2(vertices[quad->verts[4] * 3], vertices[quad->verts[4] * 3 + 1]);
				break;
			case 2:
				a = glm::vec2(vertices[quad->verts[0] * 3], vertices[quad->verts[0] * 3 + 1]);
				b = glm::vec2(vertices[quad->verts[5] * 3], vertices[quad->verts[5] * 3 + 1]);
				break;
			case 3:
				a = glm::vec2(vertices[quad->verts[8] * 3], vertices[quad->verts[8] * 3 + 1]);
				b = glm::vec2(vertices[quad->verts[6] * 3], vertices[quad->verts[6] * 3 + 1]);
				break;
			}
			_InitQuad(quad->quads[i], a, b, i);
			_ConstructQuadTree(quad->quads[i]);
		}
	}

	void Landscape::_CollectIndices(Quad* quad)
	{
		if (quad->quads[0]!=0) //
		{
			for (int i = 0; i < 4; i++)
			{
				_CollectIndices(quad->quads[i]);
			}
		}
		else
		{
			indices.push_back(quad->verts[1]); indices.push_back(quad->verts[3]); indices.push_back(quad->verts[5]);
			indices.push_back(quad->verts[1]); indices.push_back(quad->verts[5]); indices.push_back(quad->verts[7]);
		}
	}

	void Landscape::_InitQuad(Quad* quad, glm::vec2 bl, glm::vec2 tr, char dir)
	{
		unsigned short indOffset = vertices.size() / 3;

		quad->verts[0] = 0 + indOffset;
		quad->verts[1] = 1 + indOffset; quad->verts[3] = 3 + indOffset; quad->verts[5] = 5 + indOffset; quad->verts[7] = 7 + indOffset;
		quad->verts[2] = 2 + indOffset; quad->verts[4] = 4 + indOffset; quad->verts[6] = 6 + indOffset; quad->verts[8] = 8 + indOffset;

		vertices.push_back(bl.x + (tr.x - bl.x) * 0.5f);	vertices.push_back(bl.y + (tr.y - bl.y) * 0.5f);		vertices.push_back(0.0f);
		vertices.push_back(bl.x);							vertices.push_back(bl.y);					vertices.push_back(0.0f);
		vertices.push_back(bl.x + (tr.x - bl.x) * 0.5f);	vertices.push_back(bl.y);					vertices.push_back(0.0f);
		vertices.push_back(tr.x);							vertices.push_back(bl.y);					vertices.push_back(0.0f);
		vertices.push_back(tr.x);							vertices.push_back(bl.y + (tr.y - bl.y) * 0.5f);		vertices.push_back(0.0f);
		vertices.push_back(tr.x);							vertices.push_back(tr.y);			vertices.push_back(0.0f);
		vertices.push_back(bl.x + (tr.x - bl.x) * 0.5f);	vertices.push_back(tr.y);			vertices.push_back(0.0f);
		vertices.push_back(bl.x);							vertices.push_back(tr.y);			vertices.push_back(0.0f);
		vertices.push_back(bl.x);							vertices.push_back(bl.y + (tr.y - bl.y) * 0.5f);		vertices.push_back(0.0f);
		uvw.push_back(0.5f); uvw.push_back(0.5f); uvw.push_back(0.0f);
		uvw.push_back(0.0f); uvw.push_back(0.0f); uvw.push_back(0.0f);
		uvw.push_back(0.5f); uvw.push_back(0.0f); uvw.push_back(0.0f);
		uvw.push_back(1.0f); uvw.push_back(0.0f); uvw.push_back(0.0f);
		uvw.push_back(1.0f); uvw.push_back(0.5f); uvw.push_back(0.0f);
		uvw.push_back(1.0f); uvw.push_back(1.0f); uvw.push_back(0.0f);
		uvw.push_back(0.5f); uvw.push_back(1.0f); uvw.push_back(0.0f);
		uvw.push_back(0.0f); uvw.push_back(1.0f); uvw.push_back(0.0f);
		uvw.push_back(0.0f); uvw.push_back(0.5f); uvw.push_back(0.0f);
	}

	void Landscape::SetTileSize(unsigned char sz)
	{
		this->tileSz = sz;
	}

	void Landscape::SetLandscapeSize(unsigned int w, unsigned int h)
	{
		this->w = w;
		this->h = h;
	}

	void Landscape::Rebuild()
	{
		
	}

	//TODO: А теперь все то же самое, только без рекурсии
	void Landscape::SplitNode(BinTree<triangle> *tree)
	{
		std::vector<BinNode<triangle>>* data = tree->Data();
		for (unsigned int i = 0; i < data->size(); i++)
		{
			BinNode<triangle>* l = tree->LeftChild(i);
			BinNode<triangle>* r = tree->RightChild(i);

			if (l == 0 || r == 0)
				break;

			unsigned int x0, y0, x1, y1;
			unsigned int xN, yN;
			unsigned int a = data->at(i).value.a;
			unsigned int b = data->at(i).value.b;

			x0 = a - (a / depth)*(depth);
			y0 = (a / depth);
			x1 = b - (b / depth)*(depth);
			y1 = (b / depth);

			xN = (x0 + x1) * 0.5f;
			yN = (y0 + y1) * 0.5f;

			unsigned int index = xN + depth * yN;

			r->SetValue(triangle(data->at(i).value.b, data->at(i).value.c, index));
			l->SetValue(triangle(data->at(i).value.c, data->at(i).value.a, index));
		}
	}
}