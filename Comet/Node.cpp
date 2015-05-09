#include "Node.h"
#include "RenderObject.h"
#include "Renderer.h"

namespace Comet
{

	Node::Node()
	{
		parent = 0;
		dirty();
		scale = vec3f(1.0f, 1.0f, 1.0f);
		rotation = quat(0, 0, 0, 1.0f); //That's really important
		layer = 1;	//Default rendering group
	}

	Node::~Node()
	{
		std::set<Node*>::iterator it;
		for (it = nodes.begin(); it != nodes.end(); it++)
		{
			delete (*it);
		}
		nodes.clear();

		//TODO
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
	
	void Node::SetParent(Node* n)
	{
		parent = n;
	}

	//Math stuff
	void Node::SetPosition(vec3f pos)
	{
		position = pos;
		dirty();
	}
	void Node::SetRotation(quat rot)
	{
		rotation = rot;
		dirty();
	}

	//TODO
	void Node::Translate(vec3f vec, TSPACE space) //Должно работать правильно, хорошо бы проверить
	{
		if (space == LOCAL)
		{
			vec = toMat3(rotation) * vec;
			position = position + vec;
		}
		else if(space == PARENT)
		{
			if (parent)
				vec = parent->GetTransform() * vec; //This silently takes vec3 as vec4 for multiplication with mat4x4
			position = position + vec;
		}
		else if (space == WORLD)
		{
			position = position + vec;
		}

		dirty();
	}

	void Node::Rotate(float angle, vec3f vec, TSPACE space)	//Обязательно проверить и скорее всего убедиться в том что оно не работает правильно
																//Да вроде нормально все
	{
		if (space == LOCAL)
		{
			quat q = angleAxis(angle, vec);
			rotation = q * rotation;
			//rotation = normalize(rotation);
		}
		else if (space == PARENT)
		{
			//TODO	
		}
		else if (space == WORLD)
		{
			quat q = angleAxis(angle, vec);
			rotation = rotation * q;
			//rotation = normalize(rotation);
		}
		dirty();
	}
	void Node::Scale(vec3f vec)
	{
		scale = vec;
		dirty();
	}

	vec3f Node::GetPosition()
	{ 
		mat4f transform = GetTransform();
		return transform[3];
	}

	vec3f Node::GetLocalPos()
	{
		return position;
	}

	//TODO
	mat4f Node::GetTransform() //Нужно проверить
	{
		if (IsDirty())
		{
			if (parent)
				transform = parent->GetTransform() * ((translate(mat4f(1.0f), this->position) * toMat4(rotation) * ::scale(mat4f(1.0f), this->scale)));
			else
				transform = translate(mat4f(1), this->position)*toMat4(rotation)*::scale(mat4f(1), this->scale);
			drt = false;
		}
		return transform;
	}

	//TODO
	mat4f Node::GetLocalTransform()
	{
		return (translate(mat4f(1), this->position) * toMat4(rotation) * ::scale(mat4f(1), this->scale));
	}

	vec3f Node::GetUp()
	{
		mat4f t = GetTransform();
		return vec3f(t[1][0], t[1][1], t[1][2]);
	}
	vec3f Node::GetBack()
	{
		mat4f t = GetTransform();
		return vec3f(t[2][0], t[2][1], t[2][2]);
	}
	vec3f Node::GetRight()
	{
		mat4f t = GetTransform();
		return vec3f(t[0][0], t[0][1], t[0][2]);
	}

	void Node::dirty()
	{
		drt = true;
		std::set<Node*>::iterator it;
		for (it = nodes.begin(); it != nodes.end(); it++)
		{
			(*it)->dirty(); 
		}
	}
	bool Node::IsDirty()
	{return drt;}

};