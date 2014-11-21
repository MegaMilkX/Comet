#include "Node.h"
#include "RenderObject.h"
#include "Renderer.h"

namespace Comet
{

	Node::Node()
	{
		parent = 0;
		drt = true;

		position = glm::vec3(0.0f, 0.0f, 0.0f);
		rotation = glm::quat();
		scale = glm::vec3(1.0f, 1.0f, 1.0f);
	}

	Node::~Node()
	{
		std::set<Node*>::iterator it;
		for (it = nodes.begin(); it != nodes.end(); it++)
		{
			delete (*it);
		}
		nodes.clear();

		//Удаляем все объекты привязаные к ноде. (Может не стоит ей давать столько власти?)
		//(Но что тогда будет с объектами чьи ноды были удалены? Будут болтаться до полной очистки?)
		std::set<RenderObject*>::iterator ito;
		for (ito = objects.begin(); ito != objects.end(); ito++)
		{
			delete (*ito);
		}
		objects.clear();
	}

	Node* Node::CreateNode()
	{
		Node* node = new Node();
		node->parent = this;
		nodes.insert(node);
		return node;
	}
	void Node::Attach(Node* n)
	{
		n->parent = this;
	}
	void Node::Attach(RenderObject* obj)
	{
		if (obj->node)
			obj->node->Detach(obj);
		obj->node = this;
		objects.insert(obj);
	}

	void Node::DestroyNode(Node* n)
	{
		delete n;
		nodes.erase(n);
	}
	void Node::Detach(Node* n)
	{
		n->parent = 0;
		nodes.erase(n);
	}
	void Node::Detach(RenderObject* obj)
	{
		obj->node = 0;
		objects.erase(obj);
	}
	void Node::SetParent(Node* n)
	{
		parent = n;
	}

	void Node::SetPosition(glm::vec3 pos)
	{
		position = pos;
		dirty();
	}
	void Node::SetRotation(glm::quat rot)
	{
		rotation = rot;
		dirty();
	}
	void Node::Translate(glm::vec3 vec, int space) //Должно работать правильно, хорошо бы проверить
	{
		if (space == 0)					//Local
		{
			vec = glm::toMat3(rotation) * vec;
			position = position + vec;
		}
		else							//World
		{
			position = position + vec;
		}
		dirty();
	}
	void Node::Rotate(float angle, glm::vec3 vec, int space)	//Обязательно проверить и скорее всего убедиться в том что оно не работает правильно
	{
		if (space == 0)					//Local
		{
			glm::quat q = glm::angleAxis(angle, vec);
			rotation = glm::cross(q, rotation);
			rotation = glm::normalize(rotation);
		}
		else							//World
		{
			glm::quat q = glm::angleAxis(angle, vec);
			rotation = glm::cross(rotation, q);
			rotation = glm::normalize(rotation);
		}
		dirty();
	}
	void Node::Scale(glm::vec3 vec)
	{
		scale = vec;
		dirty();
	}

	glm::mat4 Node::GetTransform() //Нужно проверить
	{
		if (IsDirty())
		{
			if (parent)
				transform = parent->GetTransform() * (glm::translate(glm::mat4(), this->position)*glm::toMat4(rotation)*glm::scale(glm::mat4(), this->scale));
			else
				transform = glm::translate(glm::mat4(), this->position)*glm::toMat4(rotation)*glm::scale(glm::mat4(), this->scale);
			drt = false;
		}
		return transform;
	}

	void Node::dirty()
	{drt = true;}
	bool Node::IsDirty()
	{return drt;}

};